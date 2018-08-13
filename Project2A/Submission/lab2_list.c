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
#include <signal.h>
#include "SortedList.h"

int n_threads = 1;  // number of parallel threads
int n_iterations = 1;  // number of iterations
int n_lists = 1;  // number of lists (always 1 in this project)
int opt_yield = 0;
int opt_sync = 0;
SortedList_t *list;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int spin_lock = 0;

// signal handler for segmentation faults
void signal_handler(int signum) {
    if (signum == SIGSEGV) {
        fprintf(stderr, "Error: a segmentation fault is caught.\n");
        exit(2);
    }
}

// print an error message on stderr when system call fails and exit with a return code of 1
void system_call_fail(char *name) {
    fprintf(stderr, "Error: system call %s fails, info: %s\n", name, strerror(errno));
    exit(1);
}

// initializes an empty list
void init_list() {
    list = malloc(sizeof(SortedList_t));  // initializes an empty list
    list->key = NULL;
    list->prev = list;
    list->next = list;
}

void list_operation(SortedListElement_t **elements) {
    int i;
    for (i = 0; i < n_iterations; i++) {
        SortedListElement_t *element = elements[i];
        SortedList_insert(list, element);
    }
    for (i = 0; i < n_iterations; i++) {
        int length = SortedList_length(list);
        if (length == -1) {  // get length fails
            fprintf(stderr, "Error: get length operation fails, the list is corrupted.\n");
            exit(2);
        }
    }
    for (i = 0; i < n_iterations; i++) {
        SortedListElement_t *oldelem = elements[i];
        SortedListElement_t *elem = SortedList_lookup(list, oldelem->key);
        if (elem == NULL) {  // lookup fails
            fprintf(stderr, "Error: lookup operation fails, the list is corrupted.\n");
            exit(2);
        }
        int rc = SortedList_delete(elem);
        if (rc == -1) {  // delete fails
            fprintf(stderr, "Error: delete fails, the list is corrupted.\n");
            exit(2);
        }
    }
}

void list_operation_mutex(SortedListElement_t **elements) {
    int i;
    for (i = 0; i < n_iterations; i++) {
        SortedListElement_t *element = elements[i];
        pthread_mutex_lock(&lock);
        SortedList_insert(list, element);
        pthread_mutex_unlock(&lock);
    }
    for (i = 0; i < n_iterations; i++) {
        pthread_mutex_lock(&lock);
        int length = SortedList_length(list);
        pthread_mutex_unlock(&lock);
        if (length == -1) {  // get length fails
            fprintf(stderr, "Error: get length operation fails, the list is corrupted.\n");
            exit(2);
        }
    }
    for (i = 0; i < n_iterations; i++) {
        SortedListElement_t *oldelem = elements[i];
        pthread_mutex_lock(&lock);
        SortedListElement_t *elem = SortedList_lookup(list, oldelem->key);
        if (elem == NULL) {  // lookup fails
            fprintf(stderr, "Error: lookup operation fails, the list is corrupted.\n");
            exit(2);
        }
        int rc = SortedList_delete(elem);
        pthread_mutex_unlock(&lock);
        if (rc == -1) {  // delete fails
            fprintf(stderr, "Error: delete fails, the list is corrupted.\n");
            exit(2);
        }
    }
}

void list_operation_spin_lock(SortedListElement_t **elements) {
    int i;
    for (i = 0; i < n_iterations; i++) {
        SortedListElement_t *element = elements[i];
        while (__sync_lock_test_and_set(&spin_lock, 1) == 1) ; // spin-wait
        SortedList_insert(list, element);
        __sync_lock_release(&spin_lock);
    }
    for (i = 0; i < n_iterations; i++) {
        while (__sync_lock_test_and_set(&spin_lock, 1) == 1) ; // spin-wait
        int length = SortedList_length(list);
        __sync_lock_release(&spin_lock);
        if (length == -1) {  // get length fails
            fprintf(stderr, "Error: get length operation fails, the list is corrupted.\n");
            exit(2);
        }
    }
    for (i = 0; i < n_iterations; i++) {
        SortedListElement_t *oldelem = elements[i];
        while (__sync_lock_test_and_set(&spin_lock, 1) == 1) ; // spin-wait
        SortedListElement_t *elem = SortedList_lookup(list, oldelem->key);
        if (elem == NULL) {  // lookup fails
            fprintf(stderr, "Error: lookup operation fails, the list is corrupted.\n");
            exit(2);
        }
        int rc = SortedList_delete(elem);
        __sync_lock_release(&spin_lock);
        if (rc == -1) {  // delete fails
            fprintf(stderr, "Error: delete fails, the list is corrupted.\n");
            exit(2);
        }
    }
}

void * thread_routine(void * elements) {
    switch(opt_sync) {
        case 0:
            list_operation((SortedListElement_t **)elements);
            break;
        case 1:
            list_operation_mutex((SortedListElement_t **)elements);
            break;
        case 2:
            list_operation_spin_lock((SortedListElement_t **)elements);
            break;
        default:  // unknown error
            exit(2);
    }
    return NULL;
}

int main(int argc, char * argv[]) {
    int return_code;
    int i = 0, j = 0;
    
    static const struct option long_options[] = {
        { "threads",    required_argument, NULL, 't' },
        { "iterations", required_argument, NULL, 'i' },
        { "yield",      required_argument, NULL, 'y' },
        { "sync",       required_argument, NULL, 's' },
        { 0,            0,                 0,     0  }
    };
    int opt = -1;
    const char *optstring = "t:i:y:s:";
    while ((opt = getopt_long(argc, argv, optstring, long_options, NULL)) != -1) {
        switch (opt) {
            case 't':
                n_threads = atoi(optarg);
                break;
            case 'i':
                n_iterations = atoi(optarg);
                break;
            case 'y':
                for (i = 0; i < (int)strlen(optarg); i++) {
                    if (optarg[i] == 'i')
                        opt_yield |= INSERT_YIELD;
                    else if (optarg[i] == 'd')
                        opt_yield |= DELETE_YIELD;
                    else if (optarg[i] == 'l')
                        opt_yield |= LOOKUP_YIELD;
                    else {  // Invalid value for --yield option
                        fprintf(stderr, "Error: Invalid value for --yield option\n");
                        fprintf(stderr, "Usage: lab2_add --threads=# --iterations=# --yield=[idl] --sync=m/s\n");
                        exit(1);
                    }
                }
                break;
            case 's':
                if(strcmp("m", optarg) == 0)
                    opt_sync = 1;
                else if(strcmp("s", optarg) == 0)
                    opt_sync = 2;
                else {  // Invalid value for --sync option
                    fprintf(stderr, "Error: Invalid value for --sync option\n");
                    fprintf(stderr, "Usage: lab2_add --threads=# --iterations=# --yield=[idl] --sync=m/s\n");
                    exit(1);
                }
                break;
            default:  // encounter an unrecognized argument
                fprintf(stderr, "Error: Unrecongized argument\n");
                fprintf(stderr, "Usage: lab2_add --threads=# --iterations=# --yield=[idl] --sync=m/s\n");
                exit(1);
        }
    }
    
    signal(SIGSEGV, signal_handler);  // register the signal handler
    
    init_list();  // initializes an empty list
    int n_elements = n_threads * n_iterations;  // number (threads x iterations) of list elements
    SortedListElement_t **elements = malloc(sizeof(SortedList_t) * n_elements);  // initializes an empty list
    
    //initializes (with random keys) the required number (threads x iterations) of list elements
    srand((unsigned int)time(NULL));
    int MAX_LEN = 10;  // max length of the key
    for (i = 0; i < n_elements; i++) {
        int length = rand() % MAX_LEN + 1;  // random length
        elements[i] = malloc(sizeof(SortedList_t));
        char *key = malloc(sizeof(char) * (MAX_LEN + 1));
        bzero((char *)key, MAX_LEN + 1);
        for (j = 0; j < length; j++) {
            char c = 'a' + rand() % 26;  // random character from [a-z]
            key[j] = c;
        }
        elements[i]->key = key;
    }
    
    pthread_t threads[n_threads];
    struct timespec start_time, end_time;
    return_code = clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
    if(return_code != 0) {  // system call fails
        system_call_fail("clock_gettime");
    }
    for (i = 0; i < n_threads; i++) {
        return_code = pthread_create(&threads[i], NULL, thread_routine, elements + i * n_iterations);
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
    // checks the length of the list to confirm that it is zero.
    if(SortedList_length(list) != 0) {
        fprintf(stderr, "Error: the length of the list is not zero at the end, the list is corrupted\n");
        exit(2);
    }
    
    long long runtime = (end_time.tv_sec - start_time.tv_sec) * 1000000000L + (end_time.tv_nsec - start_time.tv_nsec);  // the total run time (in nanoseconds)
    long long n_operations = n_threads * n_iterations * 3;  // the total number of operations performed
    char yieldopts[8][10] = { "none", "i", "d", "id", "l", "il", "dl", "idl" };
    char syncopts[3][10] = { "none", "m", "s" };
    printf("list-%s-%s,%d,%d,%d,%lld,%lld,%lld\n", yieldopts[opt_yield], syncopts[opt_sync], n_threads, n_iterations, n_lists, n_operations, runtime, runtime / n_operations);
    return 0;
}
