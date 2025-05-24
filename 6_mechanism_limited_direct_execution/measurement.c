#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage: ./measurement <iterations>\n");
        return 1;
    }

    int iterations = atoi(argv[1]);

    struct timespec ts_bf;
    struct timespec ts_af;

    int sum = 0;

    for (int i = 0; i < iterations; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &ts_bf);

        // printf("before seconds: %li\n", tv_bf.tv_sec);
        // printf("before microseconds: %li\n", tv_bf.tv_usec);

        char buf[1];
        read(0, buf, 0);

        clock_gettime(CLOCK_MONOTONIC, &ts_af);
        // printf("after seconds %li\n", tv_af.tv_sec);
        // printf("after microseconds %li\n", tv_af.tv_usec);
        //
        // printf("diff seconds: %li\n", tv_af.tv_sec - tv_bf.tv_sec);
        // printf("diff microseconds: %li\n", tv_af.tv_usec - tv_bf.tv_usec);
        long long diff = (ts_af.tv_sec - ts_bf.tv_sec) * 1000000000LL + (ts_af.tv_nsec - ts_bf.tv_nsec);
        sum += diff;
    }

    printf("average time: %d\n", sum / iterations);

    return EXIT_SUCCESS;
}
