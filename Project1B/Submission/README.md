# README
NAME: Zhengyuan Liu

EMAIL: zhengyuanliu@ucla.edu

## Included Files:
* lab1b-client.c: the C source code of the client.
* lab1b-server.c: the C source code of the server.
* Makefile: the Makefile to build the program and the tarball.
* README: the README file describing each of the included files and any other information

## Some Notes
* It is tricky that for the zlib-compressed char array, it is not '\0' terminated (because actually it is byte array rather that char array), so that we cannot use strlen() to get the length of zlib-compressed char array and it would lose length. Therefore I write a function my_strlen() to get the really length for the zlib-compressed char array.

## References:
* Lab 1A Week 3 Slide
* Sockets Tutorial, http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
* socket(2) - Linux man page, https://linux.die.net/man/2/socket
* connect(2) - Linux man page, https://linux.die.net/man/2/connect
* gethostbyname(3) - Linux man page, https://linux.die.net/man/3/gethostbyname
* bind(2) - Linux man page, https://linux.die.net/man/2/bind
* zlib Usage Example, https://www.zlib.net/zlib_how.html
* deflate and inflate (zlib.h) in C, https://stackoverflow.com/questions/7540259/deflate-and-inflate-zlib-h-in-c
