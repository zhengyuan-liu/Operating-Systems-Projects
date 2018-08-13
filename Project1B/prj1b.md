# Lab 1B

## socket

### Synopsis
	#include <sys/socket.h>

	sockfd = socket(int socket_family, int socket_type, int protocol);

## gethostbyname(3)
### Name
read - read from a file descriptor
### Synopsis

	#include <netdb.h>
	extern int h_errno;

	struct hostent *gethostbyname(const char *name);

	#include <sys/socket.h>       /* for AF_INET */
	struct hostent *gethostbyaddr(const void *addr,
	                              socklen_t len, int type);

	struct hostent {
    char  *h_name;            /* official name of host */
    char **h_aliases;         /* alias list */
    int    h_addrtype;        /* host address type */
    int    h_length;          /* length of address */
    char **h_addr_list;       /* list of addresses */
	}
	#define h_addr h_addr_list[0] /* for backward compatibility */


### Description
read() attempts to read up to #count# bytes from file descriptor #fd# into the buffer starting at #buf#.

### Return Value
On success, the number of bytes read is returned (zero indicates end of file), and the file position is advanced by this number. On error, -1 is returned, and errno is set appropriately.

## sockaddr和sockaddr_in详解
### 一、sockaddr
sockaddr在头文件#include <sys/socket.h>中定义，sockaddr的缺陷是：sa_data把目标地址和端口信息混在一起了，如下：

	struct sockaddr {  
	     sa_family_t sin_family;//地址族
	　　  char sa_data[14]; //14字节，包含套接字中的目标地址和端口信息               
	}; 

### 二、sockaddr_in
sockaddr_in在头文件#include<netinet/in.h>或#include <arpa/inet.h>中定义，该结构体解决了sockaddr的缺陷，把port和addr 分开储存在两个变量中

### 三、总结
二者长度一样，都是16个字节，即占用的内存大小是一致的，因此可以互相转化。二者是并列结构，指向sockaddr_in结构的指针也可以指向sockaddr。

sockaddr常用于bind、connect、recvfrom、sendto等函数的参数，指明地址信息，是一种通用的套接字地址。 
sockaddr_in 是internet环境下套接字的地址形式。所以在网络编程中我们会对sockaddr_in结构体进行操作，使用sockaddr_in来建立所需的信息，最后使用类型转化就可以了。一般先把sockaddr_in变量赋值后，强制类型转换后传入用sockaddr做参数的函数：sockaddr_in用于socket定义和赋值；sockaddr用于函数参数。

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
