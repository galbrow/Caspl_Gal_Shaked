#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	FILE *fp = stdin;
	FILE *out = stdout;
	int debug = 0, none = 0, numOfJump = 0, input = 0;
	char sign = '+';
	char in[100];
	char outFile[100];
	for (int i = 1; i < argc; i++)
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
			strcpy(in, argv[i] + 2);
			fp = fopen(in, "r");
			input = 1;
		}
		else if (argv[i][0] == '-' && argv[i][1] == 'o')
		{
			strcpy(outFile, argv[i] + 2);
			out = fopen(outFile, "w");
		}
		else
		{
			printf("invalid parameter - %s\n", argv[i]);
			return 1;
		}
	}
	char str[1000];
	int num = 0;
	do
	{
		do
		{
			char c = fgetc(fp);
			char newc = c;
			if (c == '\n' && input == 0)
				break;
			if (feof(fp))
				break;
			if (none == 0)
			{
				if (c > 64 && c < 91)
				{
					newc = c + 32;
					num = num + 1;
				}
			}
			else
			{
				num = num + 1;
				newc = c + numOfJump;
			}
			fputc(newc, out);
			if (debug == 1)
				fprintf(stderr, "%d\t%d\n", c, newc);
		} while (1);
		if (debug == 1)
		{
			printf("%c", '\n');
			printf("\n%s%d\n", "the number of letters: ", num);
			num = 0;
		}
		fputc('\n', out);
		if (feof(fp))
			break;
	} while (1);
	if (out != stdout)
		fclose(out);
	fclose(fp);
	return (0);
}
