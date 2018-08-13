# Lab 1A

## termios(3), tcgetattr(3), tcsetattr(3)

	#include <termios.h>
	#include <unistd.h>
	int tcgetattr(int fd, struct termios *termios_p);
	int tcsetattr(int fd, int optional_actions, const struct termios *termios_p);

	struct termios {
		tcflag_t c_iflag;      /* input modes */
		tcflag_t c_oflag;      /* output modes */
		tcflag_t c_cflag;      /* control modes */
		tcflag_t c_lflag;      /* local modes */
		cc_t     c_cc[NCCS];   /* special characters */
	}

TCSANOW: the change occurs immediately.

## read(2)
### Name
read - read from a file descriptor
### Synopsis

	#include <unistd.h>
	ssize_t read(int fd, void *buf, size_t count);

### Description
read() attempts to read up to #count# bytes from file descriptor #fd# into the buffer starting at #buf#.

### Return Value
On success, the number of bytes read is returned (zero indicates end of file), and the file position is advanced by this number. On error, -1 is returned, and errno is set appropriately.

## write(2)
### Name
write - write to a file descriptor
### Synopsis

	#include <unistd.h>
	ssize_t write(int fd, const void *buf, size_t count);

### Description
write() writes up to count bytes from the buffer pointed buf to the file referred to by the file descriptor fd.

## fork(2)
Name
fork - create a child process
Synopsis

	#include <unistd.h>
	pid_t fork(void);

Description
fork() creates a new process by duplicating the calling process. 

## exec族函数
### exec函数说明
fork函数是用于创建一个子进程，该子进程几乎是父进程的副本，而有时我们希望子进程去执行另外的程序，exec函数族就提供了一个在进程中启动另一个程序执行的方法。它可以根据指定的文件名或目录名找到可执行文件，并用它来取代原调用进程的数据段、代码段和堆栈段，在执行完之后，原调用进程的内容除了进程号外，其他全部被新程序的内容替换了。另外，这里的可执行文件既可以是二进制文件，也可以是Linux下任何可执行脚本文件。
在Linux中使用exec函数族主要有以下两种情况
a. 当进程认为自己不能再为系统和用户做出任何贡献时，就可以调用任何exec函数族让自己重生。
b. 如果一个进程想执行另一个程序，那么它就可以调用fork函数新建一个进程，然后调用任何一个exec函数使子进程重生。
### Synopsis

	#include <unistd.h>

	extern char **environ;

	int execl(const char *path, const char *arg, ...);
	int execlp(const char *file, const char *arg, ...);
	int execle(const char *path, const char *arg, ..., char * const envp[]);
	int execv(const char *path, char *const argv[]);
	int execvp(const char *file, char *const argv[]);
	int execvpe(const char *file, char *const argv[],
	char *const envp[]);

### execl() vs execlp()
execl()函数：执行文件函数
函数说明：execl()用来执行参数path 字符串所代表的文件路径, 接下来的参数代表执行该文件时传递过去的argv(0), argv[1], ..., 最后一个参数必须用空指针(NULL)作结束.
返回值：如果执行成功则函数不会返回, 执行失败则直接返回-1, 失败原因存于errno 中.
范例

	#include <unistd.h>
	main()
	{
	  execl("/bin/ls", "ls", "-al", "/etc/passwd", (char *)0);
	}

execlp()函数：从PATH环境变量中查找文件并执行
函数说明：execlp()会从PATH 环境变量所指的目录中查找符合参数file 的文件名, 找到后便执行该文件, 然后将第二个以后的参数当做该文件的argv[0], argv[1], ..., 最后一个参数必须用空指针(NULL)作结束.
返回值：如果执行成功则函数不会返回, 执行失败则直接返回-1, 失败原因存于errno 中.
错误代码：参考 execve().
范例

	/* 执行ls -al /etc/passwd execlp()会依PATH 变量中的/bin 找到/bin/ls */
	#include <unistd.h>
	main()
	{
	  execlp("ls", "ls", "-al", "/etc/passwd", (char *)0);
	}

## poll(2)
### Name
poll, ppoll - wait for some event on a file descriptor

### Synopsis
	#include <poll.h>

	int poll(struct pollfd *fds, nfds_t nfds, int timeout);

	#define _GNU_SOURCE         /* See feature_test_macros(7) */
	#include <poll.h>

	int ppoll(struct pollfd *fds, nfds_t nfds,
	        const struct timespec *timeout_ts, const sigset_t *sigmask);
### Description
poll() performs a similar task to select(2): it waits for one of a set of file descriptors to become ready to perform I/O.

The set of file descriptors to be monitored is specified in the fds argument, which is an array of structures of the following form:

	struct pollfd {
	    int   fd;         /* file descriptor */
	    short events;     /* requested events */
	    short revents;    /* returned events */
	};

## 连接Linux服务器
Example: assuming your SEAS account loginname is zhengyua

    ssh -X lnxsrv.seas.ucla.edu -l zhengyua
    chmod a+x P1A_check.sh
    ./P1A_check.sh 604945064
