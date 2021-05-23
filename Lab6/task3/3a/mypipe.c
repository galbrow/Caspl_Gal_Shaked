#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <linux/limits.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    char *message = "hello";
    int pipeArr[2], nbytes;
    char buf[strlen(message)];
    if (pipe(pipeArr) < 0)
        exit(1);
    pid_t pid = fork();
    if (pid > 0)
    {
        write(pipeArr[1], message, strlen(message));
        //wait(NULL);
    }
    else
    {
        while ((nbytes = read(pipeArr[0], buf, strlen(message))) > 0)
            printf("%s\n", buf);
        if (nbytes != 0)
            exit(2);
        kill(getpid(), SIGINT);
    }
    return 0;
}