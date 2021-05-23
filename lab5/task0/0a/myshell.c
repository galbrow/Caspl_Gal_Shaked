#include <stdio.h>
#include <stdlib.h>
#include "linux/limits.h"
#include "LineParser.h"

char buf[PATH_MAX];
void execute(cmdLine *pCmdLine)
{
    execvp(buf, pCmdLine->arguments);
    perror("Error: ");
    exit(1);
}

int main(int argc, char **argv)
{
    cmdLine *cl;
    char input[2048];
    while (1)
    {
        getcwd(buf, PATH_MAX);
        char inSucces = fgets(input, 2048, stdin);
        if (strcpy("quit", buf) == 0)
            break;
        cl = parseCmdLines(input);
        execute(cl);
        free(cl);
    }
    return 0;
}