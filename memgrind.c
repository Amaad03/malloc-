#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h> 
#include "mymalloc.h"

#define NUM_RUNS 50
#define NUM_OBJECTS 120

long get_elapsed_time(struct timeval start, struct timeval end) {
    return (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
}

// Task 1: malloc() and immediately free() a 1-byte object, 120 times
void task1() {
    for (int i = 0; i < NUM_OBJECTS; i++) {
        char *ptr = mymalloc(1, __FILE__, __LINE__);
        if (ptr) {
            myfree(ptr, __FILE__, __LINE__);
        }
    }
}

// Task 2: Use mymalloc() to get 120 1-byte objects, storing pointers in an array
void task2() {
    char *pointers[NUM_OBJECTS];
    for (int i = 0; i < NUM_OBJECTS; i++) {
        pointers[i] = mymalloc(1, __FILE__, __LINE__);
    }
    for (int i = 0; i < NUM_OBJECTS; i++) {
        myfree(pointers[i], __FILE__, __LINE__);
    }
}

// Task 3: Randomly allocate and deallocate 1-byte objects
void task3() {
    char *pointers[NUM_OBJECTS];
    int count = 0;

    while (count < NUM_OBJECTS) {
        if (rand() % 2 == 0 && count < NUM_OBJECTS) { 
            pointers[count] = mymalloc(1, __FILE__, __LINE__);
            if (pointers[count] != NULL) {
                count++;
            }
        } else if (count > 0) { 
            int index = rand() % count;
            myfree(pointers[index], __FILE__, __LINE__);
            pointers[index] = pointers[count - 1]; 
            count--;
        }
    }

    for (int i = 0; i < count; i++) {
        myfree(pointers[i], __FILE__, __LINE__);
    }
}

// Task 4: Allocate and free with a slight delay to simulate work
void task4() {
    char *pointers[NUM_OBJECTS];
    for (int i = 0; i < NUM_OBJECTS; i++) {
        pointers[i] = mymalloc(1, __FILE__, __LINE__);
        usleep(10); 
    }
    for (int i = 0; i < NUM_OBJECTS; i++) {
        myfree(pointers[i], __FILE__, __LINE__);

    }
}

// Task 5: Allocate 1-byte objects and check for successful allocations
void task5() {
    char *pointers[NUM_OBJECTS];
    for (int i = 0; i < NUM_OBJECTS; i++) {
        pointers[i] = mymalloc(1, __FILE__, __LINE__);
        if (pointers[i] == NULL) {
            printf("Failed to allocate memory at index %d\n", i);
        }
    }
    for (int i = 0; i < NUM_OBJECTS; i++) {
        myfree(pointers[i], __FILE__, __LINE__);
    }
}

int main() {
    struct timeval start, end;
    long total_time = 0; 

    srand((unsigned int)time(NULL));

    for (int i = 0; i < NUM_RUNS; i++) {
        gettimeofday(&start, NULL);
        
        task1();
        task2();
        task3();
        task4();
        task5();

        gettimeofday(&end, NULL);
        total_time += get_elapsed_time(start, end);
    }

    printf("Average time for %d runs: %ld microseconds\n", NUM_RUNS, total_time / NUM_RUNS);
    
    return 0;
}
