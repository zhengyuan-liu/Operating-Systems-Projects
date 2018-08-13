# NAME: Zhengyuan Liu
# EMAIL: zhengyuanliu@ucla.edu

default: build

build:
	@gcc -Wall -Wextra -g lab2_add.c -o lab2_add -lpthread -lrt
	@gcc -Wall -Wextra -g SortedList.c lab2_list.c -o lab2_list -lpthread -lrt

tests: build
	@rm -f lab2_add.csv lab2_list.csv
	@bash ./lab2_add_tests.sh
	@-bash ./lab2_list_tests.sh

graphs:
	@gnuplot lab2_add.gp
	@gnuplot lab2_list.gp

dist: tests graphs
	@tar -cvzf lab2a.tar.gz \
	lab2_add.c SortedList.h SortedList.c lab2_list.c \
	Makefile README \
	lab2_add.gp lab2_list.gp lab2_add_tests.sh lab2_list_tests.sh \
	lab2_add.csv lab2_list.csv \
	lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png lab2_add-5.png \
	lab2_list-1.png lab2_list-2.png lab2_list-3.png lab2_list-4.png \

clean:
	@rm -f lab2a.tar.gz lab2_add lab2_list