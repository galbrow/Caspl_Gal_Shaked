#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
	FILE *fp = stdin;
	char str[100];
	strcpy(str,argv[1]);
	int num=0;
	do
	{	
		FILE*out=stdout;
	do
	{
		char c=fgetc(fp);
		if(c=='\n')
			break;
		if(str[0]=='+'){
			if(str[2]>='0' && str[2]<='9'){
				fputc(c+str[2]-48,out);
			}
			else
				fputc(c+str[2]-55,out);
		}else{
				if(str[2]>=0 && str[2]<=9)
				fputc(c-str[2],out);
			else
				fputc(c-str[2]-55,out);
	}
	}while(1);
			if(feof(fp))
			break;
	}while(1);
	fclose(fp);
	return(0);
}
