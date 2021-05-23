#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

void signalHandler(int num)
{
	char *signame = strsignal(num);
	printf("the signal %s recieved", signame);
	signal(num, );
	if (num == 0)
	{
	}
	else if (num == 1)
	{
	}
	else if (num == 2)
	{
	}
}

int main(int argc, char **argv)
{

	printf("Starting the program\n");

	while (1)
	{
		sleep(2);
	}

	return 0;
}