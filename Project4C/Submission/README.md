NAME: Zhengyuan Liu
EMAIL: zhengyuanliu@ucla.edu

Project 4C README

Included Files:
* lab4c_tcp.c, lab4c_tls.c: two C source files for two embedded applications (lab4c_tcp and lab4c_tls) that build and run (with no errors or warnings) on a Beaglebone.
* Makefile: the Makefile to build and test the application. The higher level targets include:
	* default ... build both versions of the program (compiling with the -Wall and -Wextra options).
	* clean ... delete all programs and output created by the Makefile and restore the directory to its freshly untarred state.
	* dist ... create the deliverable tarball.
* README: the README file containing descriptions of each of the included files and other comments.


Some Notes:
* Base on Piazza Post 491 (https://piazza.com/class/jdabtkwr46f7mn?cid=491), I removed all button related codes in this lab.


References:
* BeagleBone Tutorial: WiFi, SSH and SFTP
* getopt_long(3) - Linux man page, https://linux.die.net/man/3/getopt_long
* OpenSSL Manpages for 1.1.0, https://www.openssl.org/docs/man1.1.0/
* SSL_read, https://www.openssl.org/docs/man1.0.2/ssl/SSL_read.html
* SSL_write, https://www.openssl.org/docs/man1.0.2/ssl/SSL_write.html
* C/C++ SSL Socket in Linux - CSDN Blog, https://blog.csdn.net/sjin_1314/article/details/21043613
* Lab Session 1A Week 10 Slides, https://ccle.ucla.edu/pluginfile.php/2381849/mod_resource/content/0/Week%2010.pdf
