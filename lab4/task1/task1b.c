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

int main(int argc, char **argv)
{
	char errorr[256];
	char *decVal;
	int errIndex = 0;
	int i;
	int fp = STDIN;
	int out = STDOUT;
	char *backn = "\n";
	int debug = 0, none = 0, numOfJump = 0, input = 0;
	char sign = '+';
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-D") == 0)
			debug = 1;
		else if ((argv[i][0] == '+' || argv[i][0] == '-') && argv[i][1] == 'e')
		{
			none = 1;
			numOfJump = argv[i][2];
			sign = argv[i][0];
			if (argv[i][2] >= '0' && argv[i][2] <= '9')
				numOfJump = (argv[i][2] - 48);
			else
				numOfJump = (argv[i][2] - 55);
			if (sign == '-')
				numOfJump = numOfJump * (-1);
		}
		else if (argv[i][0] == '-' && argv[i][1] == 'i')
		{
			fp = system_call(SYS_OPEN, argv[i] + 2, 0, (0777 | 0644));
			if (fp < 0)
			{
				char *err = "invalid input name\n";
				system_call(SYS_WRITE, STDERR, err, strlen(err));
				system_call(SYS_CLOSE, out);
				system_call(SYS_CLOSE, fp);
				system_call(SYS_EXIT);
			}
			input = 1;
		}
		else if (argv[i][0] == '-' && argv[i][1] == 'o')
		{
			out = system_call(SYS_OPEN, argv[i] + 2, (64 | 1), (0777 | 0644));
			if (out < 0)
			{
				char *err = "invalid output name\n";
				system_call(SYS_WRITE, STDERR, err, strlen(err));
				system_call(SYS_CLOSE, out);
				system_call(SYS_CLOSE, fp);
				system_call(SYS_EXIT);
			}
		}
		else
		{
			char *err = "invalid parameter\n";
			system_call(SYS_WRITE, STDERR, err, strlen(err));
			system_call(SYS_CLOSE, out);
			system_call(SYS_CLOSE, fp);
			system_call(SYS_EXIT);
		}
	}
	int num = 0;
	do
	{
		do
		{
			char c[1];
			int read = system_call(SYS_READ, fp, c, 1);
			char newc[1];
			newc[0] = c[0];
			if (read == 0)
			{
				system_call(SYS_CLOSE, out);
				system_call(SYS_CLOSE, fp);
				system_call(SYS_EXIT);
			}
			if (c[0] == '\n' && input == 0)
			{
				system_call(SYS_WRITE, out, backn, 1);
				break;
			}
			if (none == 0)
			{
				if (c[0] > 64 && c[0] < 91)
				{
					newc[0] = c[0] + 32;
					num = num + 1;
				}
			}
			else
			{
				num = num + 1;
				newc[0] = c[0] + numOfJump;
			}
			system_call(SYS_WRITE, out, newc, 1);
			if (debug == 1)
			{
				decVal = itoa((int)c[0]);
				for (i = 0; i < strlen(decVal); i++, errIndex++)
					errorr[errIndex] = decVal[i];
				errorr[errIndex] = '\t';
				errIndex++;
				decVal = itoa((int)newc[0]);
				for (i = 0; i < strlen(decVal); i++, errIndex++)
					errorr[errIndex] = decVal[i];
				errorr[errIndex] = '\n';
				errIndex++;
			}
		} while (1);
		if (debug == 1)
		{
			int i;
			char *msg = "\nthe number of letters: ";
			char *number = itoa(num);
			for (i = 0; i < strlen(msg); i++)
				errorr[errIndex + i] = msg[i];
			errorr[errIndex] = '\n';
			errIndex = errIndex + strlen(msg) + 1;
			for (i = 0; i < strlen(number); i++)
				errorr[errIndex + i] = number[i];
			errorr[errIndex + 1] = '\n';
			system_call(SYS_WRITE, STDOUT, errorr, errIndex + 1 + strlen(number));
			num = 0;
			errIndex = 0;
			for (i = 0; i < 256; i++)
				errorr[i] = '\0';
		}
	} while (1);
	if (out != STDOUT)
		system_call(SYS_CLOSE, out);
	system_call(SYS_CLOSE, fp);
	return (0);
}
