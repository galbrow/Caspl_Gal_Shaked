#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
	FILE *fp = stdin;
	FILE*out=stdout;
	char str[1000];
	int num=0;
	do
	{
	do
	{
		char c=fgetc(fp);
		char newc=c;
		if(c=='\n')
			break;
		if(c>64 && c<91){
			newc=c+32;
			num=num+1;
	}
	fputc(newc,out);
	if(argc>1)
		fprintf(stderr,"%d\t%d\n",c,newc);
	}while(1);
	printf("\n%s%d\n","the number of letters: ",num);
			if(feof(fp))
			break;
	}while(1);
	fclose(fp);
	return(0);
}
