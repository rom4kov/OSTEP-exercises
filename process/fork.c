#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    printf("hello (pid: %d)\n", (int) getpid());
    mode_t mode = 0777;
    int x = 100;
    int rc = fork();
    int waitrc = waitpid(rc, NULL, 0);

    if (rc < 0)
    {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (rc == 0)
    {
        // child (new process)
        close(STDOUT_FILENO);
        // for the redirection to work the file to write to has 
        // to be opened AFTER stdout was closed.
        int fd = open("crazy.txt", O_CREAT | O_RDWR, mode);
        // dup2(fd, STDOUT_FILENO);
        printf("child (pid: %d)\n", (int) getpid());
        printf("x = %i\n", x);
        printf("waitrc = %i\n", waitrc);
    }
    else {
        // parent goes down this path (main)
        printf("parent of %d (pid: %d)\n", rc, (int) getpid());
        x += 20;
        printf("x = %i\n", x);
        printf("waitrc = %i\n", waitrc);
    }

    return EXIT_SUCCESS;
}
