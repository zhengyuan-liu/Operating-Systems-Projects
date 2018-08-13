//
// NAME: Zhengyuan Liu
// EMAIL: zhengyuanliu@ucla.edu
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

// force a segmentation fault by setting a char * pointer to NULL and then storing through the null pointer
void force_segfault() {
    char *nullp = NULL;
    *nullp = '0';
}

void signal_handler(int signum) {
    printf("handler\n");
    if (signum == SIGSEGV) {
        fprintf(stderr, "Error: Received SIGSEGV and caught segmentation fault\n");
        exit(4);
    }
}

int main(int argc, char *argv[]) {
    static const struct option long_options[] = {
        { "input",    required_argument, NULL, 'i' },
        { "output",   required_argument, NULL, 'o' },
        { "segfault", no_argument,       NULL, 's' },
        { "catch",    no_argument,       NULL, 'c' },
        { 0,          0,                 0,    0 }
    };

    int opt = -1;
    const char *optstring = "i:o:sc";
    char *ifilename = NULL;
    char *ofilename = NULL;
    int segment = 0;
    int catch = 0;

    while ((opt = getopt_long(argc, argv, optstring, long_options, NULL)) != -1) {
        switch (opt) {
            case 'i':
                ifilename = optarg;
                break;
            case 'o':
                ofilename = optarg;
                break;
            case 's':
                segment = 1;
                break;
            case 'c':
                catch = 1;
                break;
            default:  // encounter an unrecognized argument
                // print out an error message including a correct usage line
                fprintf(stderr, "Error: Unrecongized argument\n");
                fprintf(stderr, "Usage: lab0 --input=filename --output=filename --segfault --catch\n");
                exit(1);
        }
    }
    
    int fd0 = -1;  // file descriptor for input file
    int fd1 = -1;  // file descriptor for output file
    if (ifilename != NULL) {  // use the specified file as standard input
        fd0 = open(ifilename, O_RDONLY);
        if (fd0 == -1) {  // unable to open the specified input file
            fprintf(stderr, "Error: Unable to open the specified input file\n");  // report the failure on stderr using fprintf
            fprintf(stderr, "Error info: argument: --input, filename: %s, reason: %s\n", ifilename, strerror(errno));
            exit(2);  // exit with a return code of 2
        }
        else {  // redirect fd0 to file descriptor 0
            close(0);
            dup(fd0);
            close(fd0);
        }
    }  // else use standard input
    if (ofilename != NULL) {  // create the specified file and use it as standard output
        fd1 = creat(ofilename, 0666);
        if (fd1 == -1) {  // unable to create the specified output file
            fprintf(stderr, "Error: Unable to create the specified output file\n");
            fprintf(stderr, "Error info: argument: --output, filename: %s, reason: %s\n", ofilename, strerror(errno));
            exit(3);
        }
        else {  // redirect fd1 to file descriptor 1
            close(1);
            dup(fd1);
            close(fd1);
        }
    }  // else use standard output
    if (catch == 1)
        signal(SIGSEGV, signal_handler);  // register the signal handler
    if (segment == 1)
        force_segfault();  // cause the segfault
    
    // copy stdin to stdout
    char buf[64];
    ssize_t size = 0;
    while ((size = read(0, buf, 64)) > 0) {
        write(1, buf, size);
    }
    exit(0);
}
