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
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <zlib.h>

int server_to_shell[2];  // pipe from terminal (parent process) to shell (child process)
int shell_to_server[2];  // pipe from shell (child process) to terminal (parent process)
pid_t shell_pid;  // pid of the shell process

void signal_handler(int signum) {
    if (signum == SIGPIPE)
        exit(0);
}

// print an error message on stderr when system call fails and exit with a return code of 1
void system_call_fail(char *name) {
    fprintf(stderr, "Error: system call %s fails, info: %s\n", name, strerror(errno));
    exit(1);
}

// collect the shell's exit status (using waitpid(2)) and report it to stderr
void report_exit_status() {
    int status;
    pid_t w = waitpid(shell_pid, &status, 0);
    if (w == -1) {
        fprintf(stderr, "Error: waitpid system call fails, info: %s\n", strerror(errno));
        exit(1);
    }
    fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\n", WTERMSIG(status), WEXITSTATUS(status));
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

// write to fd
void write_to_fd(int fd, char *buf, ssize_t n_bytes) {
    char write_buf[8];
    int eof = 0;
    int i;
    for (i = 0; i < n_bytes; i++) {
        switch (buf[i]) {
            case '\r':
            case '\n':
                if (fd == server_to_shell[1]) {  // go to shell
                    write_buf[0] = '\n';
                    write_check_error(fd, write_buf, 1);  // <cr> or <lf> go to shell as <lf>
                }
                else {  // socket
                    write_buf[0] = buf[i];
                    write_check_error(fd, write_buf, 1);
                }
                break;
            case 0x04:  // EOF (^D, or 0x04)
                if (fd == server_to_shell[1]) {
                    close(server_to_shell[1]); // close the pipe to the shell
                }
                eof = 1;
                break;
            case 0x03:  // the interrupt character (^C, or 0x03)
                kill(shell_pid, SIGINT);  // send a SIGINT to the shell process
                break;
            default:
                write_buf[0] = buf[i];
                write_check_error(fd, write_buf, 1);
                break;
        }
        if (eof == 1)
            break;
    }
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
    int eof = 0;
    int i;
    for (i = 0; i < n_bytes; i++) {
        switch (buf[i]) {
            case 0x04:  // EOF (^D, or 0x04)
                eof = 1;
                break;
            case 0x03:  // the interrupt character (^C, or 0x03)
                kill(shell_pid, SIGINT);  // send a SIGINT to the shell process
                break;
            default:
                write_buf[i] = buf[i];
                break;
        }
        if (eof == 1)
            break;
    }
    char buf_compressed[CHUNK];
    bzero((char *)buf_compressed, CHUNK);
    int ret = zlib_compress(write_buf, buf_compressed);
    if (ret != Z_OK) {
        fprintf(stderr, "Zlib compression occurs an error\n");
        exit(1);
    }
    write_check_error(fd, buf_compressed, my_strlen(buf_compressed, CHUNK) + 1);
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
    int compress = 0;
    static const struct option long_options[] = {
        { "port",     required_argument, NULL, 'p' },
        { "compress", no_argument,       NULL, 'c' },
        { 0,          0,                 0,     0  }
    };
    int opt = -1;
    const char *optstring = "p:c";
    while ((opt = getopt_long(argc, argv, optstring, long_options, NULL)) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
            case 'c':
                compress = 1;
                break;
            default:  // encounter an unrecognized argument
                fprintf(stderr, "Error: Unrecongized argument\n");
                fprintf(stderr, "Usage: lab1b-server --port=port# --compress\n");
                exit(1);
        }
    }
    
    struct sockaddr_in serv_addr, cli_addr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)  // error occurs
        system_call_fail("socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    int status = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (status == -1)  // error occurs
        system_call_fail("bind");
    listen(sockfd, 5);
    socklen_t addrlen = sizeof(cli_addr);
    int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &addrlen);
    if (newsockfd == -1)  // error occurs
        system_call_fail("accept");
    
    signal(SIGPIPE, signal_handler);  // register a SIGPIPE handler
    status = pipe(server_to_shell);
    if (status == -1)
        system_call_fail("pipe");
    status = pipe(shell_to_server);
    if (status == -1)
        system_call_fail("pipe");
    
    pid_t pid = fork();
    shell_pid = pid;
    if (pid == -1) { // failure
        system_call_fail("fork");
    }
    else if (pid == 0) {  // child process (shell)
        close(server_to_shell[1]);
        close(shell_to_server[0]);
        dup2(server_to_shell[0], STDIN_FILENO);
        dup2(shell_to_server[1], STDOUT_FILENO);
        dup2(shell_to_server[1], STDERR_FILENO);
        
        status = execl("/bin/bash", "bash", NULL);
        if (status == -1)  // error occurs
            system_call_fail("execl");
    }
    else {  // parent process (server)
        atexit(report_exit_status);  // collect the shell's exit status whenever exiting
        close(server_to_shell[0]);
        close(shell_to_server[1]);
        
        struct pollfd fds[2];
        fds[0].fd = newsockfd;  // fd of socket
        fds[0].events = POLLIN | POLLHUP | POLLERR;// wait for either input (POLLIN) or error (POLLHUP, POLLERR) events
        fds[1].fd = shell_to_server[0];  // fd of the pipe that returns output from the shell
        fds[1].events = POLLIN | POLLHUP | POLLERR;// wait for either input (POLLIN) or error (POLLHUP, POLLERR) events
        while (1) {
            status = poll(fds, 2, 0);
            if (status == -1)  // error occurs
                system_call_fail("poll");
            if(fds[0].revents & POLLIN) {  // fds[0] (socket) has data to read
                char buf[256];
                bzero((char *)buf, 256);
                ssize_t n_bytes = read_check_error(fds[0].fd, buf, 256);  // read input from fds[0] (socket)
                if(compress == 1)
                    write_with_decompression(server_to_shell[1], buf);
                else
                    write_to_fd(server_to_shell[1], buf, n_bytes);  // forward input to the shell through the pipe to shell
            }
            if(fds[0].revents & (POLLHUP | POLLERR)) {  // fds[0] (socket) has a hang up or error
                close(server_to_shell[1]);
                break;
            }
            if(fds[1].revents & POLLIN) {  // fds[1] (pipe from shell) has data to read
                char buf[256];
                bzero((char *)buf, 256);
                ssize_t n_bytes = read_check_error(fds[1].fd, buf, 256);  // read input from fds[1] (pipe from shell)
                if(compress == 1)
                    write_with_compression(newsockfd, buf, n_bytes);  // forward input to the network socket
                else
                    write_to_fd(newsockfd, buf, n_bytes);  // forward input to the network socket
            }
            if(fds[1].revents & (POLLHUP | POLLERR)) {  // fds[1] has a hang up or error
                break;
            }
        }
    }
    exit(0);
}
