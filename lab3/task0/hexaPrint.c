#include <stdio.h>
#include <stdlib.h>

void PrintHex(char *buffer, int length)
{
    for (int i = 0; i < length - 1; i++)
    {
        fprintf(stdout, "%02X ", buffer[i] & 0xFF);
    }
}

int main(int argc, char **argv)
{
    FILE *fp = fopen(argv[1], "r");
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *string = malloc(len);
    string = fgets(string, len + 1, fp);
    PrintHex(string, len + 1);
    printf("\n");
    char cc = 56;
    printf("%c\n", cc);
    return 0;
}