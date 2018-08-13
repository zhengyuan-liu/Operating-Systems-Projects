# NAME: Zhengyuan Liu
# EMAIL: zhengyuanliu@ucla.edu

default:
	@gcc -Wall -Wextra -g SortedList.c lab2_list.c -o lab2_list -lpthread -lrt

tests: default
	@rm -f lab2b_list.csv
	@-bash ./lab2b_tests.sh

profile:
	@bash ./profile.sh

graphs:
	@gnuplot lab2b_list.gp

dist: tests graphs
	@tar -cvzf lab2b.tar.gz \
	SortedList.h SortedList.c lab2_list.c \
	Makefile README \
	lab2b_list.csv profile.out \
	lab2b_list.gp lab2b_tests.sh profile.sh \
	lab2b_1.png lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png

clean:
	@rm -f lab2b.tar.gz lab2_list