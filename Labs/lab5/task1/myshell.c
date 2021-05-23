#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <sys/wait.h>

char buf[PATH_MAX];
int debug = 0;
int pid;

void execute(cmdLine *pCmdLine)
{
    if (debug == 1) //case of -d
        fprintf(stderr, "the execute command is: %s\n", pCmdLine->arguments[0]);
    //------------------------------------------end of -d case ------------------------------------------
    if (strcmp("cd", pCmdLine->arguments[0]) == 0) // case of cd
    {
        if (chdir(pCmdLine->arguments[1]) == -1)
        {
            fprintf(stdout, "-------------------------");
            perror("Err1");
            _exit(1);
        }
    }
    else if (!(pid = fork()))
    {
        if (debug == 1)
            fprintf(stdout, "the execute command is: %s\nthe pid is: %d\n", pCmdLine->arguments[0], pid);
        execv(pCmdLine->arguments[0], pCmdLine->arguments);
        perror("Err2");
        exit(1);
    }
    else
    {
        if (debug == 1)
            fprintf(stdout, "the execute command is: %s\nthe pid is: %d\n", pCmdLine->arguments[0], pid);
        if (pCmdLine->blocking == '1')
            waitpid(pid, NULL, 0);
    }
}

int main(int argc, char **argv)
{
    int debug = 0;
    char arr[2048];
    cmdLine *cl;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-d", argv[i]) == 0)
            debug = 1;
    }
    while (1)
    {
        getcwd(buf, PATH_MAX); //buf= current directory
        fprintf(stdout, "%s> ", buf);
        fgets(arr, 2048, stdin); // get line from user
        if (feof(stdin))         // check endof file
            _exit(0);
        if (strcmp("quit", arr) == 0) //end cond
            break;
        cl = parseCmdLines(arr); //parse---->
        execute(cl);             //running proccess
        freeCmdLines(cl);
    }
    return 0;
}