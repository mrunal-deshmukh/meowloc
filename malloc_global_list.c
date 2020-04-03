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

struct BlockHeader *request_space(struct BlockHeader *last, size_t requestedSize);

struct BlockHeader *findFreeBlock(size_t blockRequired, struct BlockHeader **last);

void *global_base = NULL;

void *malloc(size_t size) {
    struct BlockHeader *block;

    if (size <= 0) {
        return NULL;
    }

    if (!global_base) { // First call.
        block = request_space(NULL, size);
        if (!block) {
            return NULL;
        }
        global_base = block;
        return (block + 1);
    } else {
        struct BlockHeader *last = global_base;
        block = findFreeBlock(size,&last);
        if (!block) { // Failed to find free block.
            block = request_space(last, size);
            if (!block) {
                return NULL;
            }
        } else{
            //printf("found free block for size=%d",size);
        }
        return (block + 1);
    }
}

struct BlockHeader *findFreeBlock(size_t blockRequired, struct BlockHeader **last) {
    struct BlockHeader *current = global_base;
    while (current && !(current->is_free && current->size >= blockRequired)) {
        *last = current;
        current = current->next;
    }
    return current;
}

struct BlockHeader *request_space(struct BlockHeader *last, size_t requestedSize) {
    struct BlockHeader *block;
    block = sbrk(0);
    void *request = sbrk(requestedSize + META_SIZE);
    if (request == (void *) -1) {
        return NULL; // sbrk failed.
    }

    if (last) { // NULL on first request.
        last->next = block;
    }
    block->size = requestedSize;
    block->next = NULL;
    block->is_free = 0;
    return block;
}

void free(void *p) {

    if (p) {
        BlockHeader *header = (BlockHeader *) p - 1;
        header->is_free = 1;
        //clear all data from free process
        memset(p, '\0', header->size);
    }
}
