# lab0
write a program that copies its standard input to its standard output by read(2)-ing from file descriptor 0 (until encountering an end of file) and write(2)-ing to file descriptor 1. 
If no errors (other than EOF) are encountered, your program should exit(2) with a return code of 0.

Your program executable should be called lab0, and accept the following (optional) command line arguments (in any combination or order):
* --input=filename ... use the specified file as standard input (making it the new fd0). 
If you are unable to open the specified input file, report the failure (on stderr, file descriptor 2) using fprintf(3), and exit(2) with a return code of 2.
* --output=filename ... create the specified file and use it as standard output (making it the new fd1). 
If you are unable to create the specified output file, report the failure (on stderr, file descriptor 2) using fprintf(3), and exit(2) with a return code of 3.
* --segfault ... force a segmentation fault (e.g., by calling a subroutine that sets a char * pointer to NULL and then stores through the null pointer). If this argument is specified, do it immediately, and do not copy from stdin to stdout.
* --catch ... use signal(2) to register a SIGSEGV handler that catches the segmentation fault, logs an error message (on stderr, file descriptor 2) and exit(2) with a return code of 4.

When you print out an error message (e.g., because an open failed), your message should include enough information to enable a user to understand not merely the nature of the problem but its cause ... for example:
* which argument caused the problem ... e.g., --input
* which file could not be opened ... e.g., myfile.txt
* the reason it could not be opened ... e.g., no such file

Do your argument parsing with getopt_long(3). This is, for historical reasons, a somewhat convoluted API, but ...
* it is very similar APIs are used in many other languages and systems.
* I want you to gain experience with the very common trial-and-error process of learning how to use a non-trivial API.

If you encounter an unrecognized argument you should print out an error message including a correct usage line, and exit(2) with a return code of 1.

To ensure that operations are performed in the right order when multiple arguments are specified, it is suggested that you

first process all arguments and store the results in variables
then check which options were specified and carry actions out in the correct order:
do any file redirection
register the signal handler
cause the segfault
if no segfault was caused, copy stdin to stdout
It is relatively easy to generate primitive error messages with perror(3), but if you study the documentation you will see how to get access to the underlying error descriptions, which you could then use with fprintf(stderr,... to generate better formatted error messages to the correct file descriptor.

Note that to use the advanced debugging features of gdb(1) you will need to compile your program with the -g option, which adds debugging symbol table information to your program.

# Backrgound

## Linux系统调用 
所谓系统调用是指操作系统提供给用户程序的一组“特殊”接口，用户程序可以通过这组“特殊”接口来获得操作系统 内核提供的特殊服务。 
在linux中用户程序不能直接访问内核提供的服务。为了更好的保护内核空间，将程序的运行空间分为内核空间和用户空间，他们运行在不同的级别上，在逻辑上是相互隔离的。 
在linux中用户编程接口(API)遵循了在UNIX中最流行的应用编程界面标准—POSIX标准。这些系统调用编程接口主要通过C库(libc)实现的。

## 文件I/O
可用的文件I/O函数——打开文件、读文件、写文件 等等。大多数linux文件I / O只需用到5个函数:open 、read、write、lseek 以及close。
不带缓存指的是每个read和write都调用内核中的一个系统调用。这些不带缓存的I/O函数不是ANSI C的组成部分，而是POSIX组成部分。

## 文件描述符
对于内核而言，所有打开文件都由文件描述符引用。文件描述符是一个非负整数。当打开一个现存文件或创建一个新文件时，内核向进程返回一个文件描述符。当读、写一个文件时，用open或creat返回
的文件描述符标识该文件，将其作为参数传送给r e a d或w r i t e。

## creat函数
可用creat函数创建一个新文件。

    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    int creat(const char * pathname, mode_t mode) ;
  
返回:若成功为只写打开的文件描述符，若出错为‐ 1。
注意，此函数等效于:

    open (pathname, O_WRONLY | O_CREAT | O_TRUNC, mode) ;

creat的一个不足之处是它以只写方式打开所创建的文件

## int main(int argc, char *argv[])
前者可接受 命令行参数，argc表示参数个数， argv[]是参数数组,其中第0个参数是程序本身的名称（包含路径）。
argv[]是一个字符数组.
argv[0]:指向程序的全路径名
argv[1]:指向在DOS命令行中执行程序名后的第一个字符串。
argv[2]:指向第二个字符串。
...

## getopt, getopt_long
从最简单的getopt讲起，getopt函数的前两个参数，就是main函数的argc和argv，这两者直接传入即可，要考虑的就只剩下第三个参数。

optstring的格式举例说明比较方便，例如：

    char *optstring = "abcd:";

上面这个optstring在传入之后，getopt函数将依次检查命令行是否指定了 -a， -b， -c及-d（这需要多次调用getopt函数，直到其返回-1），当检查到上面某一个参数被指定时，函数会返回被指定的参数名称（即该字母）
最后一个参数d后面带有冒号，: 表示参数d是可以指定值的

## stderr vs stdout
在此之前先区分一下：printf，sprintf，fprintf。

1，printf就是标准输出，在屏幕上打印出一段字符串来。
2，sprintf就是把格式化的数据写入到某个字符串中。返回值字符串的长度。
3，fprintf是用于文件操作。

   原型：
    int fprintf(FILE *stream,char *format,[argument])；       

   功能：fprintf()函数根据指定的format(格式)发送信息(参数)到由stream(流)指定的文件.因此fprintf()可以使得信息输出到指  定的文件。

   例子:

    char name[20] = "lucy"; 

    FILE *out;
    out = fopen( "output.txt", "w" );
    if( out != NULL )
    fprintf( out, "Hello %s\n", name );

返回值：若成功则返回输出字符数，若输出出错则返回负值。

好了，以上到此为止。

然后深挖stdout，stderr。

stdout, stdin, stderr的中文名字分别是标准输出，标准输入和标准错误。

1，我们知道，标准输出和标准错误默认都是将信息输出到终端上，那么他们有什么区别呢？让我们来看个题目：

问题：下面程序的输出是什么？（intel笔试2011）

    int main(){
        fprintf(stdout,"Hello ");
        fprintf(stderr,"World!");
        return0;
    }

解答：这段代码的输出是什么呢？你可以快速的将代码敲入你电脑上（当然，拷贝更快），然后发现输出是

World!Hello

这是为什么呢？在默认情况下，stdout是行缓冲的，他的输出会放在一个buffer里面，只有到换行的时候，才会输出到屏幕。而stderr是无缓冲的，会直接输出，举例来说就是printf(stdout, "xxxx") 和 printf(stdout, "xxxx\n")，前者会憋住，直到遇到新行才会一起输出。而printf(stderr, "xxxxx")，不管有么有\n，都输出。

2，

    fprintf(stderr, "Can't open it!\n"); 
    fprintf(stdout, "Can't open it!\n"); 
    printf("Can't open it!\n"); 

这3句效果不是一样啊，有什么区别吗？

有区别。 
stdout -- 标准输出设备 (printf("..")) 同 stdout。 
stderr -- 标准错误输出设备 
两者默认向屏幕输出。 
但如果用转向标准输出到磁盘文件，则可看出两者区别。stdout输出到磁盘文件，stderr在屏幕。 

例如： 
my.exe 
Can't open it! 
Can't open it! 
Can't open it! 

转向标准输出到磁盘文件tmp.txt 
my.exe > tmp.txt 
Can't open it! 

用TYPE 看 tmp.txt的内容： 
TYPE tmp.txt 
Can't open it! 
Can't open it!

总结：注意1，点，2点即可！

## gcc
实例
常用编译命令选项

假设源程序文件名为test.c

### 无选项编译链接

    gcc test.c

将test.c预处理、汇编、编译并链接形成可执行文件。这里未指定输出文件，默认输出为a.out(就是这个文件名)。

### 选项 -o

    gcc test.c -o test

将test.c预处理、汇编、编译并链接形成可执行文件test。-o选项用来指定输出文件的文件名。


## 连接Linux服务器
Example: assuming your SEAS account loginname is zhengyua

    ssh -X lnxsrv.seas.ucla.edu -l zhengyua

## gdb
Start gdb with the executable you wish to debug

    gdb lab0

run starts the program and you can pass arguments as you normally would from the command line

    run --input=in.txt --output=out.txt --segfault

bt shows the backtrace, or call stack of the current execution

Setting breakpoints:

    break 19

print allows us to evaluate expressions from the gdb

## Makefile中的shell命令
@ before commands (e.g. @echo): do not print the command to CLI
&> /dev/null: redirect all the output of this command to /dev/null, and do not print to CLI
rm -f option: ignore files to delete that do not exist

