# Project 2B README
NAME: Zhengyuan Liu

EMAIL: zhengyuanliu@ucla.edu

## Included Files:
* SortedList.h - a header file containing interfaces for linked list operations.
* SortedList.c - the source for a C source module that implements insert, delete, lookup, and length methods for a sorted doubly linked list.
* lab2_list.c - the source for a C program that implements the specified command line options (--threads, --iterations, --yield, --sync, --lists), drives one or more parallel threads that do operations on a shared linked list, and reports on the final list and performance.
* Makefile: A Makefile to build the deliverable programs, output, graphs, and tarball. The higher level targets are:
	* default ... the lab2_list executable (compiling with the -Wall and -Wextra options).
	* tests ... run all specified test cases to generate CSV results
	* profile ... run tests with profiling tools to generate an execution profiling report
	* graphs ... use gnuplot to generate the required graphs
	* dist ... create the deliverable tarball
	* clean ... delete all programs and output generated by the Makefile
* lab2b_list.csv - containing results for all of test runs.
* profile.out - execution profiling report showing where time was spent in the un-partitioned spin-lock implementation.
* graphs (.png files), created by gnuplot(1) on the above csv data showing:
	* lab2b_1.png ... throughput vs. number of threads for mutex and spin-lock synchronized list operations.
	* lab2b_2.png ... mean time per mutex wait and mean time per operation for mutex-synchronized list operations.
	* lab2b_3.png ... successful iterations vs. threads for each synchronization method.
	* lab2b_4.png ... throughput vs. number of threads for mutex synchronized partitioned lists.
	* lab2b_5.png ... throughput vs. number of threads for spin-lock-synchronized partitioned lists.
* lab2b_list.gp: the gnuplot script to plot lab2b_1.png to lab2b_5.png.
* lab2b_tests.sh: the script to do all the test cases and generate lab2b_list.csv.
* profile.sh: the script to do the CPU profiling.
* README: the README file including descriptions of each of the included files and any other information about the submission, and brief answers to all questions.


## Answers to Questions
### QUESTION 2.3.1 - Cycles in the basic list implementation:
* Where do you believe most of the cycles are spent in the 1 and 2-thread list tests?
* Why do you believe these to be the most expensive parts of the code?
* Where do you believe most of the time/cycles are being spent in the high-thread spin-lock tests?
* Where do you believe most of the time/cycles are being spent in the high-thread mutex tests?

In the 1 and 2-thread list tests, I believe most of the cycles  are spent in the operations of the list. Because since there are very few threads (only 1 and 2), there are very little cycles spending on acquiring or waiting for the lock and most of cycles are spenting on the list operations.

In the high-thread spin-lock tests, most of the time/cycles are being spent on the spinning waiting for the spin-lock.

In the high-thread mutex tests, most of the time/cycles are being spent on the list operations (because for waiting of the mutex lock, threads that cannot get the lock are blocked and yield CPU, which does not consuming any cycles).

### QUESTION 2.3.2 - Execution Profiling:
* Where (what lines of code) are consuming most of the cycles when the spin-lock version of the list exerciser is run with a large number of threads?
* Why does this operation become so expensive with large numbers of threads?

As shown in the profile.out, routine `list_operation_spin_lock` consumes most of the cycles (90%+), and inside the routine, statement `while (__sync_lock_test_and_set(&spin_lock, 1) == 1)` consumes most of the cycles. 

When there are a large number of threads, these thread contend for one exclusive resource, so the majority of these threads have to spend a lot of time on the spinning waiting, which costs a huge number of cycles.

### QUESTION 2.3.3 - Mutex Wait Time:

Look at the average time per operation (vs. # threads) and the average wait-for-mutex time (vs. #threads).
* Why does the average lock-wait time rise so dramatically with the number of contending threads?
* Why does the completion time per operation rise (less dramatically) with the number of contending threads?
* How is it possible for the wait time per operation to go up faster (or higher) than the completion time per operation?

Since there is only one thread can acquire the lock, the rest of threads have to wait for the lock. As the number of contending threads increase, average lock-wait time dramatically rises since there are an increasing number of threads waiting, and all these threads' waiting time are summed into the overall wait-time.

Because the completion time per operation is just recording the elapsed time for the whole process, although there are multiply threads waiting for one lock, there always exists one thread which acquires the lock and does the operation. 

Because wait time per operation is the sum of waiting time of all the threads (for example at one moment there is 10 threads waiting for the lock, and the overall wait-time will add all these 10 threads' time spending on waiting); the completion time per operation is just recording the elapsed time for the whole process, and if there are multiply threads waiting, the time counted are actually the waiting time of the next thread which is going to acquire the lock.

### QUESTION 2.3.4 - Performance of Partitioned Lists
* Explain the change in performance of the synchronized methods as a function of the number of lists.
* Should the throughput continue increasing as the number of lists is further increased? If not, explain why not.
* It seems reasonable to suggest the throughput of an N-way partitioned list should be equivalent to the throughput of a single list with fewer (1/N) threads. Does this appear to be true in the above curves? If not, explain why not.

As shown in the figures, as the number of lists increases, the performance (throughput) of the synchronized methods also increases no matter what the number of threads is. The reason is as the number of lists increases, the locks become more fine-grained, which reduces the probability/degree of contentions for locks. Also, the fine-grained locks are better for the parallel execution on multiply processors.

The throughput will not continue increasing as the number of lists is further increased. Because as the number of lists further increases, the operation of inserting and deleting element tends to be constant (since little contention on locks), but the get-length operations will need more locks, which will downgrade the throughput.

This appears to be true in the above curves that the throughput of an N-way partitioned list seems to be equivalent to the throughput of a single list with fewer (1/N) threads. This is reasonable because since the fine-grained locks and a good hash-function will make the operation on an N-way partitioned list just like a single list with fewer (1/N) threads.


## Some Notes:
* In order to make my Makefile more clear and modifiable, I used the script `profile.sh` to implement the `make profile` option.
* Because there are too many test cases, I used the script `lab2b_tests.sh` to do all the test cases for lab2b.
* I used the RSHash function to do the hash of the keys (see the references).


## References:
* Linux man pages, https://linux.die.net/man/
* Lab Session 1A Week 6 Slides
* gperftools wiki - GitHub, https://github.com/gperftools/gperftools/wiki
* 10 classic string hash functions, https://blog.csdn.net/jcwkyl/article/details/4088436
* Operating Systems: Three Easy Pieces, 27 Thread API, http://pages.cs.wisc.edu/~remzi/OSTEP/threads-api.pdf
* Operating Systems: Three Easy Pieces, 28 Locks, http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf
* Operating Systems: Three Easy Pieces, 29 Lock-based Concurrent Data Structures, http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks-usage.pdf
