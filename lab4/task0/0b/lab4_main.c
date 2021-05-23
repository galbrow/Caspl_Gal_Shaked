#include "lab4_util.h"

#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_READ 3
#define SYS_LSEEK 19
#define STDOUT 1
int main(int argc, char *argv[], char *envp[])
{
  int i = 0;
  char name[5];
  int greetingFile = system_call(SYS_OPEN, argv[1], 2, (0700 | 0644));
  int greetingLen = system_call(SYS_LSEEK, greetingFile, 0, 2);
  for (i = 0; i < greetingLen; i++)
  {
    system_call(SYS_LSEEK, greetingFile, i, 0);
    system_call(SYS_READ, greetingFile, name, 5);
    if (strcmp("Shira", name) == 0)
    {
      char file[greetingLen - i - strlen(argv[2])];
      system_call(SYS_LSEEK, greetingFile, i + 5, 0);
      system_call(SYS_READ, greetingFile, file, greetingLen - i - 5);
      system_call(SYS_LSEEK, greetingFile, i, 0);
      system_call(SYS_WRITE, greetingFile, argv[2], strlen(argv[2]));
      system_call(SYS_LSEEK, greetingFile, i + strlen(argv[2]), 0);
      system_call(SYS_WRITE, greetingFile, file, greetingLen - i - 5);
    }
  }
  return 0;
}
