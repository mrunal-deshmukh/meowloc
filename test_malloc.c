#include "malloc.c"
//#include <malloc.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM_ITERATIONS 10000
#define MEMORY_SIZE 5000

float timedifference_msec(struct timeval time1, struct timeval time2)
{
    return (time2.tv_sec - time1.tv_sec) * 1000.0f + (time2.tv_usec - time1.tv_usec) / 1000.0f;
}

int main() {
    int index = 0;
    void *p[NUM_ITERATIONS];
    struct timeval t0;
    struct timeval t1;
    float calculated_time_diff;

    gettimeofday(&t0, 0);

    for(index = 0; index < NUM_ITERATIONS; index++) {
        p[index] = malloc(MEMORY_SIZE);
    }

    for(index = 0; index < NUM_ITERATIONS; index++) {

        free(p[index]);
    }

    gettimeofday(&t1, 0);
    calculated_time_diff = timedifference_msec(t0, t1);;
    //printf("\n Time: %f \n", calculated_time_diff);
    write(1, "\n Time(ms): ", 12);
    char buf[1024];
    memset(buf, '\0', sizeof(buf));
    snprintf(buf, sizeof(float), "%f", calculated_time_diff);
    write(1, buf, sizeof(buf));
    write(1, "\n", 1);
    return 0;
}
