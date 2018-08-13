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

struct termios normal_termios;  // normal terminal modes
struct termios nc_termios;      // non-canonical terminal modes
int terminal_to_shell[2];  // pipe from terminal (parent process) to shell (child process)
int shell_to_terminal[2];  // pipe from shell (child process) to terminal (parent process)
pid_t shell_pid;  // pid of the shell process

void signal_handler(int signum) {
    if (signum == SIGPIPE) {
        exit(0);
    }
}

// save normal terminal modes
void save_normal_modes() {
    int flag = tcgetattr(STDIN_FILENO, &normal_termios);
    if (flag == -1) {
        fprintf(stderr, "Error: system calls fail, info: %s\n", strerror(errno));
        exit(1);  // exit with a return code of 1
    }
}

// set terminal modes into non-canonical input mode
void set_non_canonical_modes() {
    nc_termios = normal_termios;
    nc_termios.c_iflag = ISTRIP;  /* only lower 7 bits    */
    nc_termios.c_oflag = 0;       /* no processing    */
    nc_termios.c_lflag = 0;       /* no processing    */
    int flag = tcsetattr(STDIN_FILENO, TCSANOW, &nc_termios);
    if (flag == -1) {
        fprintf(stderr, "Error: system call fails, info: %s\n", strerror(errno));
        exit(1);
    }
}

// restore normal terminal modes
void restore_normal_modes() {
    int flag = tcsetattr(STDIN_FILENO, TCSANOW, &normal_termios);
    if (flag == -1) {
        fprintf(stderr, "Error: system call fails, info: %s\n", strerror(errno));
        exit(1);
    }
}

// read system call and check for errors after the system call
ssize_t read_check_error(int fd, void *buf, size_t count) {
    ssize_t n_bytes = read(fd, buf, count);
    if (n_bytes == -1) {
        fprintf(stderr, "Error: system call fails, info: %s\n", strerror(errno));
        exit(1);
    }
    return n_bytes;
}

// write system call and check for errors after the system call
ssize_t write_check_error(int fd, void *buf, size_t count) {
    ssize_t n_bytes = write(fd, buf, count);
    if (n_bytes == -1) {
        fprintf(stderr, "Error: system call fails, info: %s\n", strerror(errno));
        exit(1);
    }
    return n_bytes;
}

// write received and processed characters back out to the display (stdout)
int process_write(char *buf, ssize_t n_bytes) {
    char write_buf[8];
    int eof = 0;
    int i;
    for (i = 0; i < n_bytes; i++) {
        switch (buf[i]) {
            case '\r':
            case '\n':
                write_buf[0] = '\r';
                write_buf[1] = '\n';
                write_check_error(STDOUT_FILENO, write_buf, 2);
                break;
            case 0x04:
                eof = 1;
                break;
            default:
                write_buf[0] = buf[i];
                write_check_error(STDOUT_FILENO, write_buf, 1);
                break;
        }
        if (eof == 1)
            break;
    }
    return eof;  // return if encountering EOF
}

// write with --shell option
void write_with_shell_opt(int fd, char *buf, ssize_t n_bytes) {
    char write_buf[8];
    int eof = 0;
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
                else {  // shell
                    write_buf[0] = '\n';
                    write_check_error(fd, write_buf, 1);  // <cr> or <lf> should go to shell as <lf>
                }
                break;
            case 0x04:  // EOF (^D, or 0x04)
                if (fd == STDOUT_FILENO) {
                    write_buf[0] = '^';
                    write_buf[1] = 'D';
                    write_check_error(fd, write_buf, 2);  // echo it to stdout as ^D
                    close(terminal_to_shell[1]); // close the pipe to the shell
                }
                eof = 1;
                break;
            case 0x03:  // the interrupt character (^C, or 0x03)
                if (fd == STDOUT_FILENO) {
                    write_buf[0] = '^';
                    write_buf[1] = 'C';
                    write_check_error(fd, write_buf, 2);  // echo it to stdout as ^C
                    kill(shell_pid, SIGINT);  // send a SIGINT to the shell process
                }
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

int main(int argc, char * argv[]) {
    
    save_normal_modes();
    atexit(restore_normal_modes);  // restore normal terminal modes whenever exiting
    set_non_canonical_modes();
    
    static const struct option long_options[] = {
        { "shell", no_argument, NULL, 's' },
        { 0,       0,           0,     0  }
    };
    int opt = -1;
    const char *optstring = "s";
    int shell = 0;
    while ((opt = getopt_long(argc, argv, optstring, long_options, NULL)) != -1) {
        switch (opt) {
            case 's':
                shell = 1;
                break;
            default:  // encounter an unrecognized argument
                fprintf(stderr, "Error: Unrecongized argument\n");
                exit(1);
        }
    }
    
    if (shell) {  // with --shell option
        signal(SIGPIPE, signal_handler);  // register a SIGPIPE handler
        if (pipe(terminal_to_shell) == -1) {
            fprintf(stderr, "Error: system call fails, info: %s\n", strerror(errno));
            exit(1);
        }
        if (pipe(shell_to_terminal) == -1) {
            fprintf(stderr, "Error: system call fails, info: %s\n", strerror(errno));
            exit(1);
        }
        
        pid_t pid = fork();
        shell_pid = pid;
        if (pid == -1) {  // failure
            fprintf(stderr, "Error: system call fails, info: %s\n", strerror(errno));
            exit(1);
        }
        else if (pid == 0) {  // child process
            close(terminal_to_shell[1]);
            close(shell_to_terminal[0]);
            dup2(terminal_to_shell[0], STDIN_FILENO);
            dup2(shell_to_terminal[1], STDOUT_FILENO);
            dup2(shell_to_terminal[1], STDERR_FILENO);
            
            int err = execl("/bin/bash", "bash", NULL);
            if (err == -1) {  // error occurs
                fprintf(stderr, "Error: system call fails, info: %s\n", strerror(errno));
                exit(1);
            }
        }
        else {  // parent process
            atexit(report_exit_status);  // collect the shell's exit status whenever exiting
            close(terminal_to_shell[0]);
            close(shell_to_terminal[1]);
            
            struct pollfd fds[2];
            fds[0].fd = STDIN_FILENO;  // fd of the keyboard (stdin)
            fds[0].events = POLLIN | POLLHUP | POLLERR;// wait for either input (POLLIN) or error (POLLHUP, POLLERR) events
            fds[1].fd = shell_to_terminal[0];  // fd of the pipe that returns output from the shell
            fds[1].events = POLLIN | POLLHUP | POLLERR;// wait for either input (POLLIN) or error (POLLHUP, POLLERR) events
            char buf[256];
            while (1) {
                int flag = poll(fds, 2, 0);
                if (flag == -1) {  // error occurs
                    fprintf(stderr, "Error: system call fails, info: %s\n", strerror(errno));
                    exit(1);
                }
                if(fds[0].revents & POLLIN) {  // fds[0] (stdin) has data to read
                    ssize_t n_bytes = read_check_error(STDIN_FILENO, buf, 256);  // read input from the keyboard
                    write_with_shell_opt(STDOUT_FILENO, buf, n_bytes);  // echo it to stdout
                    write_with_shell_opt(terminal_to_shell[1], buf, n_bytes);  // forward it to the shell
                }
                if(fds[0].revents & (POLLHUP | POLLERR)) {  // fds[0] has a hang up or error
                    fprintf(stderr, "Terminal occurs a hang up or error\n");
                    break;
                }
                if(fds[1].revents & POLLIN) {  // fds[1] (pipe from shell) has data to read
                    ssize_t n_bytes = read_check_error(fds[1].fd, buf, 256);
                    write_with_shell_opt(STDOUT_FILENO, buf, n_bytes);
                }
                if(fds[1].revents & (POLLHUP | POLLERR)) {  // fds[1] has a hang up or error
                    break;
                }
            }
        }
    }
    else {
        char buf[256];
        ssize_t n_bytes = read_check_error(STDIN_FILENO, buf, 256);
        while (1) {
            int eof = process_write(buf, n_bytes);
            if (eof == 1)
                break;
            n_bytes = read_check_error(STDIN_FILENO, buf, 256);
        }
    }
    exit(0);
}
