#include<stdio.h>
#include "malloc.c"

int main(int argc, char **argv) {
    int *p = (int *)malloc(10 * sizeof(int));
    p[0] = 7729;
    printf("\n value of p[0] = %d\n", p[0]);
    free(p);
    printf("\n value of p[0] after free = %d\n", p[0]);

    //request again for same size block, it will return by linkedList.
    p = (int *)malloc(10 * sizeof(int));
    p[0] = 7729;
    printf("\n value of p[0] = %d\n", p[0]);
    free(p);
    printf("\n value of p[0] after free = %d\n", p[0]);
    return 0;
}
