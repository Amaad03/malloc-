
#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

void test_basic_allocation() {
    void *ptr1 = mymalloc(100, __FILE__, __LINE__);
    if (ptr1 == NULL) {
        fprintf(stderr, "Test failed: Failed to allocate memory.\n");
        return;
    }
    
    myfree(ptr1, __FILE__, __LINE__);
    printf("Test passed: Memory allocated and freed successfully.\n");
}

void test_memory_leak() {
    void *ptr1 = mymalloc(50, __FILE__, __LINE__);
    void *ptr2 = mymalloc(200, __FILE__, __LINE__);
    
    // Intentionally not freeing ptr1
    myfree(ptr2, __FILE__, __LINE__);
    
    // When the program exits, the leak detector should be triggered
}

void test_invalid_free() {
    void *ptr1 = mymalloc(50, __FILE__, __LINE__);
    myfree(ptr1, __FILE__, __LINE__);
    
    // Attempt to free the same pointer again
    myfree(ptr1, __FILE__, __LINE__); // This should print an error message
}

void test_exceeding_capacity() {
    void *ptr1 = mymalloc(5000, __FILE__, __LINE__); // Exceeds 4096
    if (ptr1 == NULL) {
        printf("Test passed: Correctly failed to allocate more than available memory.\n");
    } else {
        printf("Test failed: Allocated memory beyond capacity.\n");
        myfree(ptr1, __FILE__, __LINE__);
    }
}
void test_coalescing() {
    void *ptr1 = mymalloc(100, __FILE__, __LINE__);
    void *ptr2 = mymalloc(200, __FILE__, __LINE__);
    
    myfree(ptr1, __FILE__, __LINE__);
    myfree(ptr2, __FILE__, __LINE__);
    
    // Check memory leak detection at program exit
}

int main() {
    printf("Running tests...\n");

    test_basic_allocation();
    test_memory_leak();
    test_invalid_free();
    test_exceeding_capacity();
    test_coalescing();

    return 0;
}

