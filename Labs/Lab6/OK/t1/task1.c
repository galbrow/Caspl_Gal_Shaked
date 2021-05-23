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
    char *lsArgs[3] = {"ls", "-l", NULL};
    char *tailArgs[4] = {"tail", "-n", "2", NULL};
    int pipeArr[2], nbytes;
    char buf[2048];
    if (pipe(pipeArr) < 0)
        exit(1);
    pid_t child1 = fork();
    if (child1 > 0)
    {
        close(1);
        int file = dup(pipeArr[1]);
        // FILE *fp = fopen("out.txt", "w+");
        // fprintf(fp, "%d", pipeArr[1]);
        close(pipeArr[1]);
        // printf("============");
        execvp(lsArgs[0], lsArgs);
        perror("Error1: ");
    }
    else if (child1 == 0)
    {
        close(pipeArr[1]);
    }
    pid_t child2 = fork();
    if (child2 > 0)
    {
        close(0);
        int readDup = dup(pipeArr[0]);
        close(pipeArr[0]);
        // printf("\n\n============\n\n");
        execvp(tailArgs[0], tailArgs);
        perror("Error2: ");
    }
    else if (child2 == 0)
    {
        close(pipeArr[0]);
    }
    return 0;
}