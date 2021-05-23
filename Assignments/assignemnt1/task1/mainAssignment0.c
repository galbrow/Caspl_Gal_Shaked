#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
extern void assFunc(int x);

int printfAssembly(int c)
{
  printf("%d\n", c);
}

char c_checkValidity(int x)
{
  if (x % 2 == 0)
    return '1';
  return '0';
}

int main(int argc, char **argv)
{
  unsigned int maxNum = 1 << 20;
  char arr[maxNum];
  int input = -1;
  fgets(arr, maxNum, stdin);
  int parse = sscanf(arr, "%d", &input);
  if (parse == 0)
  {
    printf("not legal input");
    exit(0);
  }
  assFunc(input);
  return 0;
}
