#include<stdio.h>
#include "malloc.c"



int main(int argc, char **argv) {
    int *p = (int *)malloc(10 * sizeof(int));
    p[0] = 7729;
    printf("\n value of p[0] = %d\n", p[0]);
    return 0;
}
