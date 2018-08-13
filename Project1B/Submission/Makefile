# NAME: Zhengyuan Liu
# EMAIL: zhengyuanliu@ucla.edu

default:
	@gcc -Wall -Wextra -g lab1b-client.c -o lab1b-client -lz
	@gcc -Wall -Wextra -g lab1b-server.c -o lab1b-server -lz

client:
	@gcc -Wall -Wextra -g lab1b-client.c -o lab1b-client

server:
	@gcc -Wall -Wextra -g lab1b-server.c -o lab1b-server

clean:
	@rm -f lab1b.tar.gz lab1b-client lab1b-server

dist:
	@tar -cvzf lab1b.tar.gz lab1b-client.c lab1b-server.c Makefile README
