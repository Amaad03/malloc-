#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mymalloc.h"


struct node {
int allocated;
int size;
};


#define MEMSIZE 4096
static union {
char bytes[MEMSIZE];
double not_used;
} heap;

static int initialized;

static void leak_detector(void) {
    struct node *current = (struct node *)heap.bytes;
    int total_leaked_size = 0;
    int total_leaked_objects = 0;
    
    while ((char*) current < heap.bytes + MEMSIZE) {
        if(current->allocated){
            total_leaked_size += current->size;
            total_leaked_objects++;
        }
        current = (struct node *)((char *)current + current->size);
    }
    if (total_leaked_objects > 0){
        fprintf(stderr, "mymalloc: %d bytes leaked in %d objects.\n", total_leaked_size, total_leaked_objects);
    }

}
 
static void initialize_heap(void) {
    struct node *first_header = (struct node *)heap.bytes;
    first_header-> allocated = 0;
    first_header-> size = MEMSIZE;
    initialized = 1;
    atexit(leak_detector); 

}

void *mymalloc(size_t size, char *file, int line){
   
    
    if(!initialized) {
        initialize_heap();
    }

    size = (size + 7) & ~7; 
    
    struct node *current = (struct node *)heap.bytes;
    struct node *res = NULL;

    while ((char*) current < heap.bytes + MEMSIZE) {
        if(!current->allocated && current-> size >= size + sizeof(struct node)) {
            if (!res  || current -> size < res -> size) {
                res = current;
            }
        }
        current = (struct node *)((char *)current + current->size);

    }
    if (res) {
        // Split the block if it's larger than needed
        if (res->size >= size + sizeof(struct node) + 8) {
            struct node *new_header = (struct node *)((char *)res + sizeof(struct node) + size);
            new_header->allocated = 0; // New block is free
            new_header->size = res->size - (sizeof(struct node) + size);
            res->size = size + sizeof(struct node); // Update the size of the allocated block
        }
        res->allocated = 1; // Mark as allocated
        return (char *)res + sizeof(struct node); // Return pointer to payload
    }

    // If no block is found, print an error
    fprintf(stderr, "malloc: Unable to allocate %zu bytes (%s:%d)\n", size, file, line);
    return NULL; // Indicate failure

}

void myfree(void *ptr, char *file, int line) {
    if (!ptr) return; // Check if ptr is NULL

    // Get the header of the chunk
    struct node *header = (struct node *)((char *)ptr - sizeof(struct node));
    
    // Validate pointer: Check if it's within bounds and allocated
    if ((char *)header < heap.bytes || (char *)header >= heap.bytes + MEMSIZE || !header->allocated) {
        fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line);
        exit(2); // Terminate with an error code
    }

    // Mark as free
    header->allocated = 0;

    // Coalesce adjacent free chunks
    struct node *next_header = (struct node *)((char *)header + header->size);
    if ((char *)next_header < heap.bytes + MEMSIZE && !next_header->allocated) {
        // Combine with the next chunk
        header->size += next_header->size;
    }
    struct node *prev_header = (struct node *)heap.bytes;
    while ((char *)prev_header + prev_header->size < (char *)header) {
        prev_header = (struct node *)((char *)prev_header + prev_header->size);
    }
    if (prev_header != header && !prev_header->allocated) {
        prev_header->size += header->size;
    }

}
