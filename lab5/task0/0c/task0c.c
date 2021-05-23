#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
// #include <unistd.h>
// #include "linux/limits.h"
// #include "LineParser.h"

int main(int argc, char **argv)
{
    cmdLine *cl = malloc(1);
    int debug = 0;
    char buf[2048];
    int pid;
    char path[PATH_MAX];
    char *pathBaf = getcwd(path, PATH_MAX);
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-d", argv[i]) == 0)
        {
            debug = 1;
        }
        if (strcmp("cd", argv[i]) == 0)
        {
            if (strcmp(argv[1], "..") == 0)
            {
                for (int i = strlen(path) - 1; i >= 0; i--)
                    if (path[i] != '/')
                    {
                        path[i] = '\0';
                        break;
                    }
            }
            else if (strcmp(argv[1], ".") != 0)
            {
                strcat(path, "/");
                strcat(path, argv[i + 1]);
                int dirErr = chdir(path);
                if (dirErr == -1)
                {
                    perror("No such file or directory");
                    _exit(1);
                }
            }
        }
    }
    while (1)
    {

        if (feof(stdin))
            _exit(0);
        parseCmdLines(buf);
        if (!(pid = fork()))
        {
            if (debug == 1)
                fprintf(stderr, "the pid is :%d\nthe execute command is: %s", pid, buf);
            execvp(path, argv);
            _exit(1); /* error! */
        }
        //wait_for_child(pid);
    }
}