#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
	FILE *fp = fopen("input.txt","r");
	FILE *wr=fopen("output.txt","w");
	if(fp==NULL)
		return 0;
	char str[100];
	strcpy(str,argv[1]);
	int num=0;
	do
	{
		char c=fgetc(fp);
		if(feof(fp))
			break;
		if(str[0]=='+'){
			if(str[2]>='0' && str[2]<='9'){
				fputc(c+str[2]-48,wr);
			}
			else
				fputc(c+str[2]-55,wr);
		}else{
				if(str[2]>=0 && str[2]<=9)
				fputc(c-str[2],wr);
			else
				fputc(c-str[2]-55,wr);
	}
	}while(1);
	fclose(fp);
	fclose(wr);
	return(0);
}
