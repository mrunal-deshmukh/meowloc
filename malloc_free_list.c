#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

typedef struct BlockHeader {
    size_t size;
    short int is_free;
    struct BlockHeader *next;
} BlockHeader;

#define META_SIZE sizeof(struct BlockHeader)

void free(void *p);

struct BlockHeader *requestSpace(size_t size);

struct BlockHeader *findFreeBlock(size_t size);

void *freeList = NULL;

void *malloc(size_t size) {
    struct BlockHeader *block;

    if (size <= 0) {
        return NULL;
    }

    block = findFreeBlock(size);
    if(block != NULL) {
      return (void *)(block + 1);
    }

    // We are here means no block was found in free list.
    block = requestSpace(size);

    // means failed to allocate memory.
    if (block == NULL) {
      return NULL;
    }

    // return some valid space.
    return (void *) (block + 1);
}


// return first free block that has sufficient space.
struct BlockHeader *findFreeBlock(size_t size) {
    struct BlockHeader *current = freeList;
    while (current && !(current->size >= size)) {
        current = current->next;
    }
    return current;
}


struct BlockHeader *requestSpace(size_t size) {
    struct BlockHeader *block;
    block = sbrk(0);
    void *request = sbrk(size + META_SIZE);
    if (request == (void *) -1) {
        return NULL; // sbrk failed.
    }

    block->size = size;
    block->is_free = 0;
    block->next = NULL;
    return block;
}


void free(void *p) {
    if (p) {
        BlockHeader *blockToFree = (BlockHeader *) p - 1;
        blockToFree->is_free = 1;
        //clear all data from free process
        memset(p, '\0', blockToFree->size);
        // add to the beginning of the list.
        blockToFree->next = freeList;
        freeList = blockToFree;
    }
}
