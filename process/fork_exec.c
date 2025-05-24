#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define EXIT_SUCCESS 0

int main(int argc, char *argv[])
{
    char *args[3];
    args[0] = "ls";
    args[1] = "-l";
    args[2] = NULL;

    int rc = fork();

    if (rc == 0)
    {
        int re = execvp(args[0], args);
        printf("%i\n", re);
    }

    sleep(1);
    kill(rc, SIGTERM);

    return EXIT_SUCCESS;
}
