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

#define PAGE_SIZE 4096

#define TRUE 1
#define FALSE 0

void *FREE_LIST = NULL;

void *CURR_PAGE_BASE = NULL;

void *CURR_PAGE_END = NULL;


void free(void *p);

int requestNewPage();

struct BlockHeader *findFreeBlock(size_t size);

struct BlockHeader *getBlockFromInternalPage(size_t size);

struct BlockHeader *getBlockFromSystem(size_t size);

int shouldRequestNewPage(size_t size);

size_t calculateAvailableSpace();


void *malloc(size_t size) {
    struct BlockHeader *block;

    if (size <=0) {
        return NULL;
    }

    if (size >= PAGE_SIZE) {
        block = getBlockFromSystem(size);
        if (block) {
            return (void *) (block + 1);
        }
        return NULL;
    }

    // step 1: try to get from free list.
    block = findFreeBlock(size);
    if(block != NULL) {
        return (void *) (block + 1);
    }

    // not found in free list.
    // Try to get from internal page.
    block = getBlockFromInternalPage(size);

    if (block != NULL) {
        return (void *) (block + 1);
    }

    // request exact size from OS using sbrk here.
    block = getBlockFromSystem(size);

    if (!block) {
        return NULL;
    }
    return (void *) (block + 1);
}


size_t calculateAvailableSpace() {
    size_t availableSpace =  (size_t)(CURR_PAGE_END - CURR_PAGE_BASE);
    return availableSpace;
}


int shouldRequestNewPage(size_t size) {
    // if fist call : return TRUE
    if (CURR_PAGE_BASE == NULL) {
        return TRUE;
    }

    size_t actualRequiredSize = size + META_SIZE;
    size_t availableSpace = calculateAvailableSpace();
    if (availableSpace < actualRequiredSize) {
        return TRUE;
    }
    return FALSE;
}


struct BlockHeader *getBlockFromInternalPage(size_t size) {
    if (shouldRequestNewPage(size) == TRUE) {
        int status = requestNewPage();
        if (status == FALSE) {
            return NULL;
        }
    }

    // carve out block from existing page and update related global BASE.
    struct BlockHeader *block = (BlockHeader*) CURR_PAGE_BASE;
    size_t requiredSize = size + META_SIZE;
    CURR_PAGE_BASE += requiredSize;

    block->size = size;
    block->is_free = 0;
    block->next = NULL;
    return block;
}


struct BlockHeader *getBlockFromSystem(size_t size) {
    struct BlockHeader *block = (BlockHeader*) sbrk(0);
    void *request = sbrk(size + META_SIZE);
    if (request == (void *) -1) {
        return NULL;
    }

    block->size = size;
    block->is_free = 0;
    block->next = NULL;
    return block;
}


// return first free block that has sufficient space.
struct BlockHeader *findFreeBlock(size_t size) {
    struct BlockHeader *current = FREE_LIST;
    while (current && !(current->size >= size)) {
        current = current->next;
    }
    return current;
}


int requestNewPage(){
    void *oldBreakPoint = sbrk(0);

    //Request the whole page
    void *request = sbrk(PAGE_SIZE);
    if (request == (void *) - 1 || request == NULL) {
        return FALSE;
    }

    CURR_PAGE_BASE = oldBreakPoint;
    CURR_PAGE_END = oldBreakPoint + PAGE_SIZE;
    return TRUE;
}


void free(void *p) {
    if (p) {
        BlockHeader *blockToFree = (BlockHeader *) p - 1;
        blockToFree->is_free = 1;
        //clear all data from free process
        memset(p, '\0', blockToFree->size);
        // add to the beginning of the list.
        blockToFree->next = FREE_LIST;
        FREE_LIST = blockToFree;
    }
}
