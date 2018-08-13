//
// NAME: Zhengyuan Liu
// EMAIL: zhengyuanliu@ucla.edu
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <poll.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <zlib.h>

struct termios normal_termios;  // normal terminal modes
struct termios nc_termios;      // non-canonical terminal modes
int log_opt = 0;  // option of log
int log_fd = -1;  // fd of the log

// print an error message on stderr when system call fails and exit with a return code of 1
void system_call_fail(char *name) {
    fprintf(stderr, "Error: system call %s fails, info: %s\n", name, strerror(errno));
    exit(1);
}

// save normal terminal modes
void save_normal_modes() {
    int flag = tcgetattr(STDIN_FILENO, &normal_termios);
    if (flag == -1)
        system_call_fail("tcgetattr");
}

// set terminal modes into non-canonical input mode
void set_non_canonical_modes() {
    nc_termios = normal_termios;
    nc_termios.c_iflag = ISTRIP;  /* only lower 7 bits    */
    nc_termios.c_oflag = 0;       /* no processing    */
    nc_termios.c_lflag = 0;       /* no processing    */
    int flag = tcsetattr(STDIN_FILENO, TCSANOW, &nc_termios);
    if (flag == -1)
        system_call_fail("tcsetattr");
}

// restore normal terminal modes
void restore_normal_modes() {
    int flag = tcsetattr(STDIN_FILENO, TCSANOW, &normal_termios);
    if (flag == -1)
        system_call_fail("tcsetattr");
}

// read system call and check for errors after the system call
ssize_t read_check_error(int fd, void *buf, size_t count) {
    ssize_t n_bytes = read(fd, buf, count);
    if (n_bytes == -1)
        system_call_fail("read");
    return n_bytes;
}

// write system call and check for errors after the system call
ssize_t write_check_error(int fd, void *buf, size_t count) {
    ssize_t n_bytes = write(fd, buf, count);
    if (n_bytes == -1)
        system_call_fail("write");
    return n_bytes;
}

// write to file descriptor
void write_to_fd(int fd, char *buf, ssize_t n_bytes) {
    char write_buf[8];
    int i;
    for (i = 0; i < n_bytes; i++) {
        switch (buf[i]) {
            case '\r':
            case '\n':
                if (fd == STDOUT_FILENO) {  // stdout
                    write_buf[0] = '\r';
                    write_buf[1] = '\n';
                    write_check_error(fd, write_buf, 2);  // <cr> or <lf> should echo as <cr><lf>
                }
                else {  // socket
                    write_buf[0] = '\n';
                    write_check_error(fd, write_buf, 1);  // <cr> or <lf> should go to server as <lf>
                }
                break;
            case 0x04:  // EOF (^D, or 0x04)
                if (fd == STDOUT_FILENO) {
                    write_buf[0] = '^';
                    write_buf[1] = 'D';
                    write_check_error(fd, write_buf, 2);  // echo it to stdout as "^D"
                }
                else {
                    write_buf[0] = buf[i];
                    write_check_error(fd, write_buf, 1);
                }
                break;
            case 0x03:  // the interrupt character (^C, or 0x03)
                if (fd == STDOUT_FILENO) {
                    write_buf[0] = '^';
                    write_buf[1] = 'C';
                    write_check_error(fd, write_buf, 2);  // echo it to stdout as "^C"
                }
                else {
                    write_buf[0] = buf[i];
                    write_check_error(fd, write_buf, 1);
                }
                break;
            default:
                write_buf[0] = buf[i];
                write_check_error(fd, write_buf, 1);
                break;
        }
    }
}

#define SENT 0
#define RECEIVED 1
// write transfered data to the log
void write_to_log(int log_fd, char *buf, ssize_t n_bytes, int sent_or_recv) {
    char log_buf[512];
    bzero((char *)log_buf, 512);
    if(sent_or_recv == SENT)
        sprintf(log_buf, "SENT %ld bytes: %s\n", n_bytes, buf);
    else
        sprintf(log_buf, "RECEIVED %ld bytes: %s\n", n_bytes, buf);
    write_check_error(log_fd, log_buf, strlen(log_buf));
    //printf("log here");
}

// get the length of the deflated char array
unsigned int my_strlen(char *buf, int size) {
    int i;
    for (i = size - 1; i >= 0; i--) {
        if(buf[i] != 0)
            break;
    }
    return (unsigned int)i + 1;
}

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif
#define CHUNK 16384
int zlib_compress(char *in, char *out) {
    int ret;
    z_stream strm;
    
    // allocate deflate state
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK)
        return ret;
    
    strm.avail_in = (unsigned int)strlen(in) + 1;
    strm.next_in = (Bytef *)in;
    strm.avail_out = CHUNK;
    strm.next_out = (Bytef *)out;
    ret = deflate(&strm, Z_FINISH);
    if(ret == Z_STREAM_ERROR)
        return ret;
    // clean up and return
    (void)deflateEnd(&strm);
    return Z_OK;
}

int zlib_decompress(char *in, char *out) {
    int ret;
    z_stream strm;
    
    // allocate inflate state
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;
    
    strm.avail_in = my_strlen(in, 256);
    strm.next_in = (Bytef *)in;
    strm.avail_out = CHUNK;
    strm.next_out = (Bytef *)out;
    ret = inflate(&strm, Z_NO_FLUSH);
    if(ret == Z_STREAM_ERROR)
        return ret;
    switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            return ret;
    }
    // clean up and return
    (void)inflateEnd(&strm);
    return Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

// write to file descriptor with zlib compression
void write_with_compression(int fd, char *buf, ssize_t n_bytes) {
    char write_buf[256];
    bzero((char *)write_buf, 256);
    int i;
    for (i = 0; i < n_bytes; i++) {
        switch (buf[i]) {
            case '\r':
                write_buf[i] = '\n';  // <cr> should go to server as <lf>
                break;
            default:
                write_buf[i] = buf[i];
                break;
        }
    }
    char buf_compressed[CHUNK];
    bzero((char *)buf_compressed, CHUNK);
    int ret = zlib_compress(write_buf, buf_compressed);
    if (ret != Z_OK) {
        fprintf(stderr, "Zlib compression occurs an error\n");
        exit(1);
    }
    write_check_error(fd, buf_compressed, my_strlen(buf_compressed, CHUNK) + 1);
    if (log_opt == 1)
        write_to_log(log_fd, buf_compressed, my_strlen(buf_compressed, CHUNK) + 1, SENT);  // write sent data to the log
}

// write to file descriptor with zlib decompression
void write_with_decompression(int fd, char *buf) {
    char buf_decompressed[CHUNK];
    bzero((char *)buf_decompressed, CHUNK);
    int ret = zlib_decompress(buf, buf_decompressed);
    if (ret != Z_OK) {
        fprintf(stderr, "Zlib decompression occurs an error: %d\n", ret);
        exit(1);
    }
    write_to_fd(fd, buf_decompressed, strlen(buf_decompressed) + 1);
}

int main(int argc, char * argv[]) {
    
    int port = -1;
    int compress = 0;  // option of compress
    static const struct option long_options[] = {
        { "port",     required_argument, NULL, 'p' },
        { "log",      required_argument, NULL, 'l' },
        { "compress", no_argument,       NULL, 'c' },
        { 0,          0,                 0,     0  }
    };
    int opt = -1;
    const char *optstring = "p:l:c";
    while ((opt = getopt_long(argc, argv, optstring, long_options, NULL)) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
            case 'l':
                log_opt = 1;
                log_fd = creat(optarg, 0666);
                if (log_fd == -1)
                    system_call_fail("creat");
                break;
            case 'c':
                compress = 1;
                break;
            default:  // encounter an unrecognized argument
                fprintf(stderr, "Error: Unrecongized argument\n");
                fprintf(stderr, "Usage: lab1b-client --port=port# [--log=filename] [--compress]\n");
                exit(1);
        }
    }
    
    save_normal_modes();
    atexit(restore_normal_modes);  // restore normal terminal modes whenever exiting
    set_non_canonical_modes();
    
    struct sockaddr_in server_addr;
    struct hostent *server;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)  // error occurs
        system_call_fail("socket");
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr, "Error: no such host, info: %s\n", strerror(errno));
        exit(1);
    }
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);  // set port
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    int status = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));  // connect to a server on sockfd
    if (status == -1)  // error occurs
        system_call_fail("connect");
    
    struct pollfd fds[2];
    fds[0].fd = STDIN_FILENO;  // fd of the keyboard (stdin)
    fds[0].events = POLLIN | POLLHUP | POLLERR;// wait for either input (POLLIN) or error (POLLHUP, POLLERR) events
    fds[1].fd = sockfd;  // fd of the socket
    fds[1].events = POLLIN | POLLHUP | POLLERR;// wait for either input (POLLIN) or error (POLLHUP, POLLERR) events
    while (1) {
        int status = poll(fds, 2, 0);
        if (status == -1) {  // error occurs
            system_call_fail("poll");
        }
        if (fds[0].revents & POLLIN) {  // fds[0] (stdin, keyboard) has data to read
            char buf[256];
            bzero((char *)buf, 256);
            ssize_t n_bytes = read_check_error(fds[0].fd, buf, 256);  // read input from the keyboard
            write_to_fd(STDOUT_FILENO, buf, n_bytes);  // echo it to stdout
            if (compress == 1) {
                write_with_compression(sockfd, buf, n_bytes);  // send input from keyboard to the socket with compression
            }
            else {
                write_to_fd(sockfd, buf, n_bytes);  // send input from keyboard to the socket without compression
                if (log_opt == 1)
                    write_to_log(log_fd, buf, n_bytes, SENT);  // write sent data to the log
            }
        }
        if(fds[0].revents & (POLLHUP | POLLERR)) {  // fds[0] (stdin) has a hang up or error
            fprintf(stderr, "Terminal occurs a hang up or error\n");
            break;
        }
        if(fds[1].revents & POLLIN) {  // fds[1] (socket) has data to read
            char buf[256];
            bzero((char *)buf, 256);
            ssize_t n_bytes = read_check_error(fds[1].fd, buf, 256);
            if(log_opt == 1)
                write_to_log(log_fd, buf, n_bytes, RECEIVED);  // write received data to the log
            if (compress == 1)
                write_with_decompression(STDOUT_FILENO, buf);  // send input from the socket to the display
            else
                write_to_fd(STDOUT_FILENO, buf, n_bytes);  // send input from the socket to the display
        }
        if(fds[1].revents & (POLLHUP | POLLERR)) {  // fds[1] (socket) has a hang up or error
            break;
        }
    }
    exit(0);
}
