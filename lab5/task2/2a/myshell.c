#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <linux/limits.h>
#include "LineParser.h"
#include <sys/wait.h>

#define TERMINATED -1
#define RUNNING 1
#define SUSPENDED 0

char buf[PATH_MAX];
int debug = 0;
int pid;

typedef struct process
{
    cmdLine *cmd;         /* the parsed command line*/
    pid_t pid;            /* the process id that is running the command*/
    int status;           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next; /* next process in chain */
} process;

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

void addProcess(process **process_list, cmdLine *cmd, pid_t pid)
{
    process *proc = calloc(sizeof(process), 1);
    proc->cmd = cmd;
    proc->pid = pid;
    process *p = process_list[0];
    while (p->next != NULL)
        p = p->next;
    p->next = proc;
}

void printProcessList(process **process_list)
{
    process *p = process_list[0];
    fprintf("PID\tCOMMAND\tSTATUS\n");
    while (p != NULL)
        fprintf(stdout, "%s\t%d\t%d\n", p->pid, p->cmd->arguments[0], p->status);
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
    /*
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
freemethod of proccess*/
}