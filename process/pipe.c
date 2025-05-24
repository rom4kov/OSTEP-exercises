#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int pipedes[2];
    int rc2;
    if (pipe(pipedes))
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    int child1 = fork();
    if (child1 < 0)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (child1 == 0)
    {
        close(pipedes[0]);

        char input[50];

        fflush(stdout);
        fgets(input, sizeof(input), stdin);
        write(pipedes[1], input, sizeof(input));
        close(pipedes[1]);
        exit(0);
    }

    int child2 = fork();
    if (child2 < 0)
    {
        perror("fork2");
        exit(EXIT_FAILURE);
    }

    if (child2 == 0)
    {
        close(pipedes[1]);
        dup2(pipedes[0], STDIN_FILENO);
        close(pipedes[0]);

        char buf[100];
        fgets(buf, sizeof(buf), stdin);
        printf("child 2 received: %s", buf);
        exit(0);
    }

    close(pipedes[0]);
    close(pipedes[1]);

    waitpid(child1, NULL, 0);
    waitpid(child2, NULL, 0);

    return EXIT_SUCCESS;
}
