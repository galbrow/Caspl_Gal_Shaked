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

#define DT_UNKNOWN 0
#define DT_FIFO 1
#define DT_CHR 2
#define DT_DIR 4
#define DT_BLK 6
#define DT_REG 8
#define DT_LNK 10
#define DT_SOCK 12
#define DT_WHT 14

extern int start_code;
extern int end_code;

typedef struct dirent
{
	int inode;
	int offset;
	short len;
	char buf[1];
} dirent;

int main(int argc, char **argv)
{
	char bn[1] = {'\n'};
	system_call(SYS_WRITE, STDOUT, "code_start: ", 12);
	system_call(SYS_WRITE, STDOUT, itoa((int)&start_code), 12);
	system_call(SYS_WRITE, STDOUT, "\ncode_end: ", 11);
	system_call(SYS_WRITE, STDOUT, itoa((int)&end_code), 12);
	system_call(SYS_WRITE, STDOUT, "\n-----------------------files----------------------\n", 53);
	int p = 0;
	int d = 0;
	int a = 0;
	char *pref;
	char buf[8192];
	dirent *dir = (dirent *)buf;
	char *dd = "-D";
	char *pp = "-p";
	char *aa = "-a";
	int i;
	int fp = system_call(SYS_OPEN, ".", 0, (0644 | 0777));
	int count = system_call(SYS_GETDENTS, fp, buf, 8192);
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], dd) == 0)
			d = 1;
		else if ((strncmp(argv[i], pp, 2) == 0) && strlen(argv[i]) == 3)
		{
			p = 1;
			pref = argv[i] + 2;
		}
		else if ((strncmp(argv[i], aa, 2) == 0) && strlen(argv[i]) == 3)
		{
			a = 1;
			pref = argv[i] + 2;
		}
		else
		{
			char *c = "illeagal argument";
			system_call(SYS_WRITE, STDOUT, c, strlen(c));
			system_call(SYS_WRITE, STDOUT, bn, 1);
			system_call(SYS_EXIT, 0x55);
		}
	}
	for (i = 0; i < count;)
	{
		dir = (struct dirent *)(buf + i);
		if (dir->buf[0] == '.')
		{
			i += dir->len;
			continue;
		}
		if (((a == 1) && (strncmp(dir->buf, pref, strlen(pref)) == 0)) || ((p == 0) && (a == 0)) || (((p == 1)) && (strncmp(dir->buf, pref, strlen(pref)) == 0)))
		{
			system_call(SYS_WRITE, STDOUT, dir->buf, strlen(dir->buf));
			if (d == 1)
			{
				char *toPrint = "\tlength: ";
				char *dirLen = itoa(dir->len);
				system_call(SYS_WRITE, STDOUT, toPrint, strlen(toPrint));
				system_call(SYS_WRITE, STDOUT, dirLen, strlen(dirLen));
			}
			char *info = "\t type: ";
			char d_type = *(buf + i + dir->len - 1);
			char *dinfo = (d_type == DT_REG) ? "regular" : (d_type == DT_DIR) ? "directory"
													   : (d_type == DT_FIFO)  ? "FIFO"
													   : (d_type == DT_SOCK)  ? "socket"
													   : (d_type == DT_LNK)	  ? "symlink"
													   : (d_type == DT_BLK)	  ? "block dev"
													   : (d_type == DT_CHR)	  ? "char dev"
																			  : "unknown";
			if (a == 1)
			{
				infector(dir->buf);
			}
			if (p == 1)
			{
				system_call(SYS_WRITE, STDOUT, info, strlen(info));
				system_call(SYS_WRITE, STDOUT, dinfo, strlen(dinfo));
			}
			system_call(SYS_WRITE, STDOUT, bn, 1);
		}
		i += dir->len;
	}
	return (0);
}
