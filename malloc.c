#include <unistd.h>
#include <string.h> //For Memset


#define PGSZ 4096

typedef struct BlockHeader {
    size_t size;
    short int is_free;
    void *next;
} BlockHeader;

void *malloc(size_t size)
{
    //write(1, "Custom malloc\n", 14);
    size_t required_size;
    void *block;
    void *ret;
    BlockHeader *header;
    
    //check for Invalid memory request
    if (size <= 0)
    {
        return NULL;
    }
    
    //check for whole memory request
    if (size == PGSZ)
    {
        block = sbrk(0);
        ret = sbrk(size);
        if (block == (void*) - 1 || ret == (void *) -1) {
            return NULL;
        }
        return (void *) header;
    }
    
    required_size = sizeof(header) + size;
    
    // store curr break.
    block = sbrk(0);
    ret = sbrk(required_size);
    if (block == (void*) - 1 || ret == (void *) -1) {
        return NULL;
    }
    
    header = block;
    header->size = size;
    header->is_free = 0;
    return (void *) (header + 1);
}

void free(void *p) {
    
    if(p)
    {
        BlockHeader *header;
        header = (BlockHeader*)p - 1;
        header->is_free = 1;
        //clear all data from free process
        memset(p, '\0', header->size);
    }
    
    return;
}
