# NAME: Xiao Yang,Zhengyuan Liu
# EMAIL: avadayang@icloud.com,zhengyuanliu@ucla.edu


.SILENT:

default:
	gcc -g -Wall -Wextra  -o  lab3a lab3a.c -lm
	
build: default

clean:
	rm -f lab3a *.tar.gz

dist: 
	tar -czf lab3a-104946787.tar.gz lab3a.c ext2_fs.h Makefile README
