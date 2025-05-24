#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
// #include <sys/wait.h>

#define EXIT_SUCCESS 0

int main(int argc, char *argv[])
{
    int wc = 0;
    mode_t mode = 0777;
    int fd = open("file2.txt", O_CREAT | O_RDWR, mode);
    char *child_buf = "hello world\n";
    char *parent_buf = "goodbye\n";

    int rc = fork();
    if (rc < 0)
    {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0) {
        printf("child pid: %i, getpid: %i\n", rc, getpid());
        printf("fd: %i\n",fd);
        wc = write(fd, child_buf, strlen(child_buf));
        printf("child write success: %i\n", wc);
    }
    else {
        sleep(1);
        printf("parent pid: %i, getpid: %i\n", rc, getpid());
        printf("fd: %i\n",fd);
        wc = write(fd, parent_buf, strlen(parent_buf));
        printf("parent write success: %i\n", wc);
    }

    close(fd);
    return EXIT_SUCCESS;
}
