#include "lab4_util.h"

#define STDOUT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19
#define SYS_EXIT 1
#define STDIN 0
#define STDERR 2
#define SYS_GETDENTS 141

typedef struct dirent
{
	int inode;
	int offset;
	short len;
	char buf[1];
} dirent;

int main(int argc, char **argv)
{
	char buf[8192];
	dirent *dir = (dirent *)buf;
	char bn[1] = {'\n'};
	int i;
	int fp = system_call(SYS_OPEN, ".", 0, (0644 | 0777));
	int count = system_call(SYS_GETDENTS, fp, buf, 8192);
	for (i = 0; i < count;)
	{
		dir = (struct dirent *)(buf + i);
		if (dir->buf[0] == '.')
		{
			i += dir->len;
			continue;
		}
		system_call(SYS_WRITE, STDOUT, dir->buf, strlen(dir->buf));
		if (argc > 1)
		{
			char *d = "-D";
			char *toPrint = "\tlength: ";
			char *dirLen = itoa(dir->len);
			if (strcmp(argv[1], d) == 0)
			{
				system_call(SYS_WRITE, STDOUT, toPrint, strlen(toPrint));
				system_call(SYS_WRITE, STDOUT, dirLen, strlen(dirLen));
			}
			else
			{
				char *c = "illeagal argument";
				system_call(SYS_WRITE, STDOUT, c, strlen(c));
				system_call(SYS_EXIT, 0x55);
			}
		}
		system_call(SYS_WRITE, STDOUT, bn, 1);
		i += dir->len;
	}
	return (0);
}
