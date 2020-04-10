#include <stdio.h>
#include "malloc.c"
//#include <stdlib.h>
#define NUM_ITERATIONS 100000
int main() {

  int index = 0;
  void *p[NUM_ITERATIONS];

  for(index = 0; index < NUM_ITERATIONS; index++) {
    p[index] = malloc(5000); //allocate 5000 bytes.
  }

  for(index = 0; index < NUM_ITERATIONS; index++) {
    free(p[index]);
  }

  return 0;
}
