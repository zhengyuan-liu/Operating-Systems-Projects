# Project 0 Warm-Up
## INTRODUCTION:
It is not uncommon for students come into this course without basic C software development skills (which should have been developed in the CS111 prerequisites), and invest a great deal of time and frustrating effort in the first two projects before concluding they will not pass the course and must drop. We have created this simple warm-up to determine whether or not students are prepared to work on C programming projects. Most students should find this project to be relatively easy (a few hours of work, mostly understanding the APIs). If you do not find this project to be relatively straight-forward, you may want to reconsider whether or not you are ready to take this course.

## PROJECT DESCRIPTION:
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