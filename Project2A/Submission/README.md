# README
NAME: Zhengyuan Liu

EMAIL: zhengyuanliu@ucla.edu

## Included Files:
* lab2_add.c ... a C program that implements and tests a shared variable add function and produces the specified output statistics.
* SortedList.h ... a header file describing the interfaces for linked list operations.
* SortedList.c ... a C module that implements insert, delete, lookup, and length methods for a sorted doubly linked list.
* lab2_list.c ... a C program that implements the (below) specified command line options and produces the (below) specified output statistics.
* Makefile: the Makefile to build the deliverable programs (lab2_add and lab2_list), output, graphs, and tarball. The higher level targets are:
	* build ... (default target) compile all programs (with the -Wall and -Wextra options).
	* tests ... run all (over 200) specified test cases to generate results in CSV files. 
	* graphs ... use gnuplot(1) and the supplied data reduction scripts to generate the required graphs
	* dist ... create the deliverable tarball
	* clean ... delete all programs and output created by the Makefile
* lab2_add.csv ... containing all results for all of the Part-1 tests.
* lab2_list.csv ... containing all results for all of the Part-2 tests.
* graphs (.png files), created by running gnuplot(1) on the above .csv files with the supplied data reduction scripts.
	* lab2_add-1.png ... threads and iterations required to generate a failure (with and without yields)
	* lab2_add-2.png ... average time per operation with and without yields.
	* lab2_add-3.png ... average time per (single threaded) operation vs. the number of iterations.
	* lab2_add-4.png ... threads and iterations that can run successfully with yields under each of the synchronization options.
	* lab2_add-5.png ... average time per (protected) operation vs. the number of threads.
	* lab2_list-1.png ... average time per (single threaded) unprotected operation vs. number of iterations (illustrating the correction of the per-operation cost for the list length).
	* lab2_list-2.png ... threads and iterations required to generate a failure (with and without yields).
	* lab2_list-3.png ... iterations that can run (protected) without failure.
	* lab2_list-4.png ... (length-adjusted) cost per operation vs the number of threads for the various synchronization options.
* lab2_add.gp: the gnuplot script to plot lab2_add-1.png to lab2_add-5.png
* lab2_list.gp: the gnuplot script to plot lab2_list-1.png to lab2_list-4.png
* lab2_add_tests.sh: the script to do all the test cases for Part 1 and generate lab2_add.csv
* lab2_list_tests.sh: the script to do all the test cases for Part 2 and generate lab2_list.csv
* README: the README file including descriptions of each of the included files and any other information about the submission, and brief answers to all questions.


## Answers to the Questions
QUESTION 2.1.1 - causing conflicts:
* Why does it take many iterations before errors are seen?
* Why does a significantly smaller number of iterations so seldom fail?

Because as the number of iterations grows, there is more time for each thread spending in the loop of add 1 and add -1 to the counter, so that it is more possible for the CPU to reschdule another thread during the access of counter, which may raise an error on the counter. 
If a significantly smaller number of iterations is used, less time is spent in the loop of add 1 and add -1 for each thread, it is much less possible for the CPU to reschdule another thread during the access of counter, so the counter seldom fails.

QUESTION 2.1.2 - cost of yielding:
* Why are the --yield runs so much slower?
* Where is the additional time going?
* Is it possible to get valid per-operation timings if we are using the --yield option?
* If so, explain how. If not, explain why not.

Because with the yield option, in each add operation the thread will yield the CPU itself, which will involves a much larger number of context switches for the CPU to schedule other threads. The additional time are spent on the additional context switches.
It is not possible to get valid per-operation timings with yield option because a significant number of time is spent on unnecessary context switches. Using runtime / # of operations will get the time of operation and context switch.

QUESTION 2.1.3 - measurement errors:
* Why does the average cost per operation drop with increasing iterations?
* If the cost per iteration is a function of the number of iterations, how do we know how many iterations to run (or what the "correct" cost is)?

Because our measurement of run time includes the time spending on thread create and join system calls, which is a constant since we only create one thread in this experiment. So the more operations, the less this constant time is shared to each operation, therefore the average cost per operation drop with increasing iterations.
To know the correct cost, I think one method is we can simply use a large number of iterations and increase the number of iterations gradually, and record cost of every try. When the current cost is approximately equal to the last cost (maybe less than 1 ns), we get the "correct" cost.

QUESTION 2.1.4 - costs of serialization:
* Why do all of the options perform similarly for low numbers of threads?
* Why do the three protected operations slow down as the number of threads rises?
Because when the number of thread is low, there are fewer threads contending for the same resource (lock). A thread does not need to spend a lot of time waiting for the lock.
As the number of threads rises, there are more threads contending for the same resource (lock). Since every time only one thread can acquire the lock, a thread will spend much more time waiting for the lock.

QUESTION 2.2.1 - scalability of Mutex
* Compare the variation in time per mutex-protected operation vs the number of threads in Part-1 (adds) and Part-2 (sorted lists).
* Comment on the general shapes of the curves, and explain why they have this shape.
* Comment on the relative rates of increase and differences in the shapes of the curves, and offer an explanation for these differences.

The general shapes of both curves for Part-1 (adds) and Part-2 (sorted lists) are incremental. Because as number of threads increasing, there are more threads contending for the same exclusive resource, and each thread will wait for a longer time.
The relative rates of increase of Part-1 (adds) becomes smaller as the number of threads increases; however the relative rates of increase of Part-2 (sorted lists) does not become smaller as apparent as the adds. The reason is there is more complex to operate a shared sorted linked list than a shared counter, as the shared sorted linked list has much longer critical sections. Therefore the scalability of shared sorted linked list is much worse than a shared counter.

QUESTION 2.2.2 - scalability of spin locks
* Compare the variation in time per protected operation vs the number of threads for list operations protected by Mutex vs Spin locks. Comment on the general shapes of the curves, and explain why they have this shape.
* Comment on the relative rates of increase and differences in the shapes of the curves, and offer an explanation for these differences.

The general shapes of the curves of Mutex vs Spin locks all both incremental as the number of threads increases, but they have different relative rates of increase. As the number of threads increases, the relative rate of increase of mutex becomes smaller, but the relative rate of increase of spin locks remain approximately constant. The reason is spin locks is more expensive than mutex, since spin lock is just doing the self spinning and waste CPU time, so the scalability of spin lock is much worse than the mutex.


## Some Notes:
* Based on the Piazza note 281 (https://piazza.com/class/jdabtkwr46f7mn?cid=281), I implemented the list as circular, and I found a cicular list fixed a problem when deleting the last element in the list. Since there is only one parameter `element` in `SortedList_delete` and `list` is not passed, if the last element's next is pointed to NULL, there will be some overhead to adjust the head element's prev to the new tail.
* Because there are too many test cases, I used 2 scripts, lab2_add_tests.sh and lab2_list_tests.sh to do all the test cases for Part 1 and Part 2, respectively.


## References:
* Linux man pages, https://linux.die.net/man/
* clock_gettime(3) - Linux man page, https://linux.die.net/man/3/clock_gettime
* pthread_create(3) - Linux man page, https://linux.die.net/man/3/pthread_create
* strcmp(3) - Linux man page, https://linux.die.net/man/3/strcmp
* rand(3) - Linux man page, https://linux.die.net/man/3/rand
* Atomic Builtins - Using the GNU Compiler Collection (GCC), https://gcc.gnu.org/onlinedocs/gcc-4.4.3/gcc/Atomic-Builtins.html
* Operating Systems: Three Easy Pieces, 28 Locks, http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf
* Operating Systems: Three Easy Pieces, 29 Lock-based Concurrent Data Structures, http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks-usage.pdf
* Introduction to CAS (compare-and-swap) - CSDN Blog, https://blog.csdn.net/flashxing/article/details/41696801
