#include <stdio.h>

int main()
{
	FILE *fp = stdin;
	do
	{
		char c=fgetc(fp);
		if(feof(fp))
			break;
		if(c>64 && c<91)
			c=c+32;
		printf("%c",c);
	}while(1);
	fclose(fp);
	return(0);
}