#include <stdio.h>
#include "mymalloc.h"

void case1() {
    int x;
    printf("Test 1: Freeing a pointer that was not obtained from malloc\n");
    myfree(&x, __FILE__, __LINE__);
}

void case2() {
    printf("\nTest 2: Freeing a pointer that is not at the start of a chunk\n");
    int *p = (int *)mymalloc(sizeof(int) * 2, __FILE__, __LINE__);
    if (p) {
        myfree(p + 1, __FILE__, __LINE__);  // This should print an error message
        myfree(p, __FILE__, __LINE__);  // Free the original chunk correctly
    }
}

void case3() {
    printf("\nTest 3: Freeing the same pointer twice\n");
    int *q = (int *)mymalloc(sizeof(int) * 100, __FILE__, __LINE__);
    if (q) {
        myfree(q, __FILE__, __LINE__);  // Free the pointer for the first time
        myfree(q, __FILE__, __LINE__);  // This should print an error message
    }
}

int main() {
    // Test 1: Freeing a pointer that was not obtained from malloc
    //case1();
    
    // Test 2: Freeing a pointer that is not at the start of a chunk
    //case2();
    
    // Test 3: Freeing the same pointer twice
    case3();

    return 0;
}
