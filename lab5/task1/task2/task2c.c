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

typedef struct process
{
    cmdLine *cmd;         /* the parsed command line*/
    pid_t pid;            /* the process id that is running the command*/
    int status;           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next; /* next process in chain */
} process;

char buf[PATH_MAX];
int debug = 0;
pid_t pid;
process *procList;

void updateProcessStatus(process *process_list, int pid, int status)
{
    process *p = process_list;
    while (p != NULL)
    {
        if (p->pid == pid)
        {
            p->status = status;
            return;
        }
    }
    fprintf(stderr, "Proccess with the given pid does not exist");
}

void updateProcessList(process **process_list)
{
    int status = 0;
    process *p = process_list[0];
    while (p != NULL)
    {
        //printf("\n%s\n", stat);
        pid_t pidReturn = waitpid(p->pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
        if (WIFSIGNALED(status))
            updateProcessStatus(*process_list, p->pid, TERMINATED);
        else if (WIFCONTINUED(status))
            updateProcessStatus(*process_list, p->pid, RUNNING);
        else if (WIFSTOPPED(status))
        {
            printf("--------------------------------");
            updateProcessStatus(*process_list, p->pid, SUSPENDED);
        }

        p = p->next;
    }
}

void printProcessList(process **process_list)
{
    updateProcessList(process_list);
    process *prev = NULL;
    fprintf(stdout, "PID\tCOMMAND\t\tSTATUS\n");
    process *p = process_list[0];
    while (p != NULL)
    {
        char *stat = p->status == TERMINATED ? "Terminated" : p->status == SUSPENDED ? "Suspended"
                                                                                     : "Running";
        fprintf(stdout, "%d\t%s\t%d\n", p->pid, p->cmd->arguments[0], p->status);
        if (p->status == TERMINATED)
        {
            if (prev == NULL)
            {
                printf("-----------------------------");
                process_list[0] = p->next;
                freeCmdLines(p->cmd);
                free(p);
                p = process_list[0];
            }
            else
            {
                prev->next = p->next;
                freeCmdLines(p->cmd);
                free(p);
                p = prev->next;
            }
        }
        else
            p = p->next;
    }
}

void cdCase(char *args)
{
    if (chdir(args) == -1)
    {
        perror("Err1");
        _exit(1);
    }
}

void addProcess(process **process_list, cmdLine *cmd, pid_t pid)
{
    process *proc = calloc(sizeof(process), 1);
    proc->cmd = cmd;
    proc->pid = pid;
    proc->status = RUNNING;
    process *p = process_list[0];
    if (p == NULL)
    {
        process_list[0] = proc;
        return;
    }
    while (p->next != NULL)
        p = p->next;
    p->next = proc;
}

void debugPrint(cmdLine *pCmdLine)
{
    if (debug == 1) //case of -d
        fprintf(stderr, "the execute command is: %s\n", pCmdLine->arguments[0]);
}

void execute(cmdLine *pCmdLine)
{
    if (debug == 1)
        debugPrint(pCmdLine);
    if (strcmp("cd", pCmdLine->arguments[0]) == 0)
    { // case of cd
        cdCase(pCmdLine->arguments[1]);
    }
    else if (strcmp(pCmdLine->arguments[0], "suspend") == 0)
    {
        int pidNum = atoi(pCmdLine->arguments[1]);
        int sleepTime = atoi(pCmdLine->arguments[2]);
        int pidd = fork();
        if (pidd == 0)
        {
            kill(pidNum, SIGTSTP);
            sleep(sleepTime);
            kill(pidNum, SIGCONT);
            _exit(EXIT_SUCCESS);
        }
    }
    else if (strcmp(pCmdLine->arguments[0], "procs") == 0)
    {
        printProcessList(&procList);
    }
    else if (strcmp(pCmdLine->arguments[0], "kill") == 0)
    {
        kill(atoi(pCmdLine->arguments[1]), SIGINT);
    }
    else
    {
        if (pCmdLine->blocking == 1)
            waitpid(pid, NULL, 0);
        pid = fork();
        addProcess(&procList, pCmdLine, pid);
        if (pid == 0)
        {
            execvp(pCmdLine->arguments[0], pCmdLine->arguments);
            perror("Err2");
            _exit(1);
        }
    }
}

void freeProcessList(process *process_list)
{
    if (process_list != NULL)
    {
        while (process_list != NULL)
        {
            process *curr = process_list;
            process_list = process_list->next;
            freeCmdLines(curr->cmd);
            free(curr);
        }
    }
}

int main(int argc, char **argv)
{
    int debug = 0;
    char arr[2048];
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-d", argv[i]) == 0)
            debug = 1;
    }
    while (1)
    {
        getcwd(buf, PATH_MAX); //buf= current directory
        //fprintf(stdout, "%s> ", buf);
        fgets(arr, 2048, stdin); // get line from user
        if (feof(stdin))         // check endof file
            _exit(0);
        if (strcmp("quit", arr) == 0) //end cond
            break;
        cmdLine *cl = parseCmdLines(arr); //parse---->
        execute(cl);                      //running proccess
    }
    freeProcessList(procList);
    return 0;
}