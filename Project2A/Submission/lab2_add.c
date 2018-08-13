//
// NAME: Zhengyuan Liu
// EMAIL: zhengyuanliu@ucla.edu
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

int n_threads = 1;  // number of parallel threads
int n_iterations = 1;  // number of iterations
int opt_yield = 0;
int opt_sync = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int spin_lock = 0;

// print an error message on stderr when system call fails and exit with a return code of 1
void system_call_fail(char *name) {
    fprintf(stderr, "Error: system call %s fails, info: %s\n", name, strerror(errno));
    exit(1);
}

void add(long long *pointer, long long value) {
    long long sum = *pointer + value;
    if (opt_yield)
        sched_yield();
    *pointer = sum;
}

void add_mutex(long long *pointer, long long value) {
    pthread_mutex_lock(&lock);
    long long sum = *pointer + value;
    if (opt_yield)
        sched_yield();
    *pointer = sum;
    pthread_mutex_unlock(&lock);
}

void add_spin_lock(long long *pointer, long long value) {
    while (__sync_lock_test_and_set(&spin_lock, 1) == 1)
        ; // spin-wait
    long long sum = *pointer + value;
    if (opt_yield)
        sched_yield();
    *pointer = sum;
    __sync_lock_release(&spin_lock);
}

void add_compare_and_swap(long long *pointer, long long value) {
    long long oldval, newval;
    do {
        oldval = *pointer;
        newval = oldval + value;
        if (opt_yield)
            sched_yield();
    }
    while (__sync_val_compare_and_swap(pointer, oldval, newval) != oldval);  // if the current value of *ptr is oldval, then write newval into *ptr
}

void add_opt_sync(long long *pointer, long long value) {
    switch(opt_sync) {
        case 0:
            add(pointer, value);
            break;
        case 1:
            add_mutex(pointer, value);
            break;
        case 2:
            add_spin_lock(pointer, value);
            break;
        case 3:
            add_compare_and_swap(pointer, value);
            break;
        default:  // unknown error
            exit(2);
    }
}

void * thread_routine(void * counter) {
    int i = 0;
    for (i = 0; i < n_iterations; i++) {  // add 1 to the counter the specified number of times
        add_opt_sync((long long *)counter, 1);
    }
    for (i = 0; i < n_iterations; i++) {  // add -1 to the counter the specified number of times
        add_opt_sync((long long *)counter, -1);
    }
    return NULL;
}

int main(int argc, char * argv[]) {
    int return_code;
    int i = 0;
    
    static const struct option long_options[] = {
        { "threads",    required_argument, NULL, 't' },
        { "iterations", required_argument, NULL, 'i' },
        { "yield",      no_argument,       NULL, 'y' },
        { "sync",       required_argument, NULL, 's' },
        { 0,            0,                 0,     0  }
    };
    int opt = -1;
    const char *optstring = "t:i:ys:";
    while ((opt = getopt_long(argc, argv, optstring, long_options, NULL)) != -1) {
        switch (opt) {
            case 't':
                n_threads = atoi(optarg);
                break;
            case 'i':
                n_iterations = atoi(optarg);
                break;
            case 'y':
                opt_yield = 1;
                break;
            case 's':
                if(strcmp("m", optarg) == 0)
                    opt_sync = 1;
                else if(strcmp("s", optarg) == 0)
                    opt_sync = 2;
                else if(strcmp("c", optarg) == 0)
                    opt_sync = 3;
                else {  // Invalid value for --sync option
                    fprintf(stderr, "Error: Invalid value for --sync option\n");
                    fprintf(stderr, "Usage: lab2_add [--threads=#] [--iterations=#] [--yield] [--sync=m/s/c]\n");
                    exit(1);
                }
                break;
            default:  // encounter an unrecognized argument
                fprintf(stderr, "Error: Unrecongized argument\n");
                fprintf(stderr, "Usage: lab2_add [--threads=#] [--iterations=#] [--yield] [--sync=m/s/c]\n");
                exit(1);
        }
    }
    pthread_t threads[n_threads];
    long long counter = 0;
    struct timespec start_time, end_time;
    return_code = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
    if(return_code != 0) {  // system call fails
        system_call_fail("clock_gettime");
    }
    for (i = 0; i < n_threads; i++) {
        return_code = pthread_create(&threads[i], NULL, thread_routine, (void *)&counter);
        if(return_code != 0) {  // system call fails
            system_call_fail("pthread_create");
        }
    }
    for (i = 0; i < n_threads; i++) {
        return_code = pthread_join(threads[i], NULL);
        if(return_code != 0) {  // system call fails
            system_call_fail("pthread_create");
        }
    }
    return_code = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
    if(return_code != 0) {  // system call fails
        system_call_fail("clock_gettime");
    }
    long long runtime = (end_time.tv_sec - start_time.tv_sec) * 1000000000L + (end_time.tv_nsec - start_time.tv_nsec);  // the total run time (in nanoseconds)
    long long n_operations = n_threads * n_iterations * 2;  // the total number of operations performed
    char option1[2][10] = { "", "-yield" };
    char option2[4][10] = { "none", "m", "s", "c" };
    printf("add%s-%s,%d,%d,%lld,%lld,%lld,%lld\n", option1[opt_yield], option2[opt_sync], n_threads, n_iterations, n_operations, runtime, runtime / n_operations, counter);
    return 0;
}
