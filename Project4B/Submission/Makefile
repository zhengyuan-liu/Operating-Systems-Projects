# NAME: Zhengyuan Liu
# EMAIL: zhengyuanliu@ucla.edu
#
# Makefile

default:
	@gcc -Wall -Wextra -lmraa -lm -o lab4b lab4b.c

check: default
	@bash ./smoke_test.sh

clean:
	@rm -f lab4b.tar.gz lab4b

dist:
	@tar -cvzf lab4b.tar.gz lab4b.c Makefile README smoke_test.sh
