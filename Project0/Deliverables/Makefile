# NAME: Zhengyuan Liu
# EMAIL: zhengyuanliu@ucla.edu

default:
	@gcc -Wall -Wextra -g lab0.c -o lab0

check: default smoke-test

clean:
	@rm -f lab0.tar.gz lab0 in.txt out.txt

dist:
	@tar -cvzf lab0.tar.gz lab0.c Makefile backtrace.png breakpoint.png README

smoke-test: smoke-test-case-1 smoke-test-case-2 smoke-test-case-3 smoke-test-case-4 smoke-test-case-5 smoke-test-case-6
	@echo "All test cases passed!"

smoke-test-case-1:
	@echo "This is test input." > in.txt; \
	./lab0 --input=in.txt --output=out.txt; \
	if [ $$? -eq 0 ]; \
	then \
		echo "Case 1 passed: program ended with the correct exit code 0."; \
	else \
		echo "Case 1 failed: program ended with the wrong exit code!"; \
		exit 1; \
	fi
	@rm -f in.txt out.txt

smoke-test-case-2:
	@echo "This is test input." > in.txt; \
	./lab0 --input=in.txt --output=out.txt; \
	cmp in.txt out.txt; \
	if [ $$? -eq 0 ]; \
	then \
		echo "Case 2 passed: copy successful."; \
	else \
		echo "Case 2 failed: content of output file is different from input file!"; \
		exit 1; \
	fi
	@rm -f in.txt out.txt

smoke-test-case-3:
	@echo "This is test input." > in.txt; \
	./lab0 --input=in.txt --output=out.txt --wrong &> /dev/null ; \
	if [ $$? -eq 1 ]; \
	then \
		echo "Case 3 passed: program ended with the correct exit code 1."; \
	else \
		echo "Case 3 failed: program ended with the wrong exit code!"; \
		exit 1; \
	fi
	@rm -f in.txt out.txt

smoke-test-case-4:
	@./lab0 --input=in.txt --output=out.txt &> /dev/null ; \
	if [ $$? -eq 2 ]; \
	then \
		echo "Case 4 passed: program ended with the correct exit code 2."; \
	else \
		echo "Case 4 failed: program ended with the wrong exit code!"; \
		exit 1; \
	fi
	@rm -f in.txt out.txt

smoke-test-case-5:
	@echo "This is test input." > in.txt ; \
	./lab0 --input=in.txt --output= &> /dev/null ; \
	if [ $$? -eq 3 ]; \
	then \
		echo "Case 5 passed: program ended with the correct exit code 3."; \
	else \
		echo "Case 5 failed: program ended with the wrong exit code!"; \
		exit 1; \
	fi
	@rm -f in.txt out.txt

smoke-test-case-6:
	@./lab0 --segfault --catch &> /dev/null ; \
	if [ $$? -eq 4 ]; \
	then \
		echo "Case 6 passed: program ended with the correct exit code 4."; \
	else \
		echo "Case 6 failed: program ended with the wrong exit code!"; \
		exit 1; \
	fi
	@rm -f in.txt out.txt
