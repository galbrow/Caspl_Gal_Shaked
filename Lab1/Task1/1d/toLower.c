#include <stdio.h>

int main(int argc,char **argv)
{
	if(argc==1){
	FILE *fp = fopen("input.txt","r");
	FILE*wr=fopen("output.txt","w");
	if(fp==NULL)
		return 0;
	do
	{
		char c=fgetc(fp);
		if(feof(fp))
			break;
		if(c>64 && c<91)
			c=c+32;
		fputc(c,wr);
	}while(1);
	fclose(fp);
	fclose(wr);
}
	return(0);
}
