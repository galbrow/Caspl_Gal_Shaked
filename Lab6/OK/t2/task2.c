#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
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
process *procList;
pid_t pidd = 0;

void pipeOption(cmdLine *cl)
{
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
        execvp(cl->arguments[0], cl->arguments);
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
        execvp(cl->next->arguments[0], cl->next->arguments);
        perror("Error2: ");
    }
    else if (child2 == 0)
    {
        close(pipeArr[0]);
    }
    _exit(0);
}

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
        p = p->next;
    }
    fprintf(stderr, "Proccess with the given pid does not exist");
}

void updateProcessList(process **process_list)
{
    if (process_list == NULL)
        return;
    process *p = *process_list;
    while (p != NULL)
    {
        int status = 0;
        pid_t pidReturn = waitpid(p->pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
        if (pidReturn == -1 || WIFSIGNALED(status))
            updateProcessStatus(p, p->pid, TERMINATED);
        else if (WIFCONTINUED(status))
            updateProcessStatus(p, p->pid, RUNNING);
        else if (WIFSTOPPED(status))
            updateProcessStatus(*process_list, p->pid, SUSPENDED);
        p = p->next;
    }
}

void printProcessList(process **process_list)
{
    updateProcessList(process_list);
    process *prev = NULL;
    printf("PID\tCOMMAND\t\tSTATUS\n");
    process *p = *process_list;
    while (p != NULL)
    {
        char *stat = p->status == TERMINATED ? "Terminated" : p->status == SUSPENDED ? "Suspended"
                                                                                     : "Running";
        printf("%d\t%s\t%s\n", p->pid, p->cmd->arguments[0], stat);
        if (p->status == TERMINATED)
        {
            if (prev == NULL)
            {
                *process_list = p->next;
                freeCmdLines(p->cmd);
                free(p);
                p = *process_list;
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
        {
            prev = p;
            p = p->next;
        }
    }
}

void cdCase(char *args)
{
    if (chdir(args) == -1)
    {
        perror("Error: ");
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
        fprintf(stderr, "the pid is: %d\nthe execute command is: %s\n", getpid(), pCmdLine->arguments[0]);
}

void execute(cmdLine *pCmdLine)
{
    // printf("\n%s\n", pCmdLine->arguments[0]);
    // printf("\n%s\n", pCmdLine->arguments[1]);
    // printf("\n%s\n", pCmdLine->next->arguments[0]);
    // printf("\n%s\n", pCmdLine->next->arguments[1]);
    if (strcmp("cd", pCmdLine->arguments[0]) == 0)
    { // case of cd
        freeCmdLines(pCmdLine);
        cdCase(pCmdLine->arguments[1]);
    }
    else if (strcmp(pCmdLine->arguments[0], "suspend") == 0)
    {
        int pidNum = atoi(pCmdLine->arguments[1]);
        int sleepTime = atoi(pCmdLine->arguments[2]);
        freeCmdLines(pCmdLine);
        pidd = fork();
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
        freeCmdLines(pCmdLine);
        printProcessList(&procList);
    }
    else if (strcmp(pCmdLine->arguments[0], "kill") == 0)
    {
        kill(atoi(pCmdLine->arguments[1]), SIGINT);
        freeCmdLines(pCmdLine);
    }
    else
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            if (pCmdLine->inputRedirect != NULL)
            {
                close(0);
                fopen(pCmdLine->inputRedirect, "r");
            }
            if (pCmdLine->outputRedirect != NULL)
            {
                close(1);
                fopen(pCmdLine->outputRedirect, "w+");
            }
            debugPrint(pCmdLine);
            if (pCmdLine->next != NULL)
            {
                pipeOption(pCmdLine);
            }
            execvp(pCmdLine->arguments[0], pCmdLine->arguments);
            perror("Error: ");
            _exit(1);
        }
        addProcess(&procList, pCmdLine, pid);
        if (pCmdLine->blocking == 1)
            waitpid(pid, NULL, 0);
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
    char arr[2048];
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-d", argv[i]) == 0)
            debug = 1;
    }
    while (1)
    {
        memset(buf, '\0', PATH_MAX);
        getcwd(buf, PATH_MAX); //buf= current directory
        //fprintf(stdout, "%s> ", buf);
        fgets(arr, 2048, stdin);          // get line from user
        cmdLine *cl = parseCmdLines(arr); //parse---->
        if (strcmp("quit", cl->arguments[0]) == 0)
        { //end cond
            freeCmdLines(cl);
            break;
        }
        execute(cl); //running proccess
    }
    freeProcessList(procList);
    return 0;
}