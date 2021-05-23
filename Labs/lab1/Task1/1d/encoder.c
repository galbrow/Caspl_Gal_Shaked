#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
	FILE*wr=fopen("output.txt","w");
	if(argc==1){
	FILE *fp = fopen("input.txt","r");
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
	}else{
	for(int i=1;i<argc;i++){
		if(strcmp(argv[i],"-D")){

	FILE *fp = fopen("input.txt","r");
	if(fp==NULL)
		return 0;
	char str[1000];
	int num=0;
	do
	{
		
		char c=fgetc(fp);
		char newc=c;
		if(feof(fp))
			break;
		if(c>64 && c<91){
			newc=c+32;
			num=num+1;
		}
		printf("%d\t%d\n",c,newc);
	}while(1);
	printf("\n%s%d\n","the number of letters: ",num);
	fclose(fp);

}
		else{

	FILE *fp = fopen("input.txt","r");
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

}
	}
}
	fclose(wr);
	return(0);
}
