# README
NAME: Zhengyuan Liu

EMAIL: zhengyuanliu@ucla.edu

## Included Files:
* lab0.c: the C source code that that copies its standard input to its standard output by read(2)-ing from file descriptor 0 (until encountering an end of file) and write(2)-ing to file descriptor 1 and implements other requirements of Project 0.
* Makefile: the Makefile to build the program and the tarball as the requirement of Project 0.
* backtrace.png: screen snapshot from a gdb session showing a segfault and associated stack-trace
* breakpoint.png: screen snapshot from a gdb session showing a breakpoint and variable inspection
* README: the README file


## Smoke-test Cases:
* Case 1: copy successful, and the program exits with 0. Create input file, run the command below, and test whether the program exits with 0.
	./lab0 --input in.txt --output out.txt

* Case 2: copy successful, and the program exits with 0. Create input file, run the command below, and test whether the program copies successfully (whether the content of output file is same as input file).
	./lab0 --input in.txt --output out.txt

* Case 3: unrecognized argument, and the program exit with 1. Create input file, run the incorrect command below, and test whether the program exits with 1.
	./lab0 --input in.txt --output out.txt --wrong

* Case 4: unable to open input file, and the program exit with 2. DO NOT create input file, run the command below, and test whether the program exits with 2.
	./lab0 --input in.txt --output out.txt

* Case 5: unable to create output file, and the program exit with 3. Create input file, run the incorrect command below, and test whether the program exits with 3.
	./lab0 --input in.txt --output

* Case 6: Cause the sefault and catch SIGSEGV, and the program exit with 4. Run the command below, and test whether the program exits with 4.
	./lab0 --segfault --catch


## Some Notes:
* Use @ before commands (e.g. @echo): do not print the command to CLI
* Use &> /dev/null: redirect all the output of this command to /dev/null, and do not print to CLI
* Use rm -f option: ignore files to delete that do not exist

## References:
* Project 0 description, http://web.cs.ucla.edu/classes/spring18/cs111/projects/P0.html
* Linux man pages, https://linux.die.net/man/
* getopt() and getopt_long() - CSDN Blog, https://blog.csdn.net/cashey1991/article/details/7942809
* [Linux] Detailed description for signal() - CSDN Blog, https://blog.csdn.net/yockie/article/details/51729774
* GNU Make, https://ftp.gnu.org/old-gnu/Manuals/make-3.79.1/html_chapter/make_toc.html
* Lab 1A Discussion Slide
