#define _GNU_SOURCE
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sched.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage: ./measure_cswitch <iterations>\n");
        return 1;
    }

    int pipe_fd1[2];
    int pipe_fd2[2];

    struct timespec ts_bf;
    struct timespec ts_af;

    int iterations = atoi(argv[1]);

    if (pipe(pipe_fd1) == -1)
    {
        perror("pipe1");
        exit(1);
    }

    if (pipe(pipe_fd2) == -1)
    {
        perror("pipe2");
        exit(1);
    }

    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    int rc = fork();

    if (rc == 0)
    {
        char a;

        if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
        {
            perror("sched_setaffinit");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < iterations; i++)
        {

            write(pipe_fd2[1], &a, 1);
            read(pipe_fd1[0], &a, 1);
        }
    }
    else
    {
        char b;

        if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
        {
            perror("sched_setaffinit");
            exit(EXIT_FAILURE);
        }

        clock_gettime(CLOCK_MONOTONIC, &ts_bf);
        for (int i = 0; i < iterations; i++)
        {
            write(pipe_fd1[1], &b, 1);
            read(pipe_fd2[0], &b, 1);
        }
        wait(NULL);
        clock_gettime(CLOCK_MONOTONIC, &ts_af);

        long long diff = (ts_af.tv_sec - ts_bf.tv_sec) * 1000000000LL +
                         (ts_af.tv_nsec - ts_bf.tv_nsec);
        printf("average time: %lli\n", diff / (iterations * 2));
    }

    return EXIT_SUCCESS;
}
