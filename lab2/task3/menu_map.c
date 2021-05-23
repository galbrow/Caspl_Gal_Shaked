#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char censor(char c)
{
  if (c == '!')
    return '.';
  else
    return c;
}

char encrypt(char c)
{
  if (c < 32 || c > 126)
    return c;
  else
    return c + 3;
}

char decrypt(char c)
{
  if (c < 32 || c > 126)
    return c;
  else
    return c - 3;
}

char cprt(char c)
{
  if (c < 32 || c > 126)
    printf(".\n");
  else
    printf("%c\n", c);
  return c;
}
char my_get(char c)
{
  c = fgetc(stdin);
  return c;
}

char *map(char *array, int array_length, char (*f)(char))
{
  char *mapped_array = (char *)(malloc(array_length * sizeof(char)));
  for (int i = 0; i < array_length; i++)
  {
    mapped_array[i] = f(array[i]);
  }
  return mapped_array;
}

struct fun_desc
{
  char *name;
  char (*fun)(char);
};

int main(int argc, char **argv)
{
  char *carray = malloc(5);
  struct fun_desc menu[] = {{"censor", censor}, {"encrypt", encrypt}, {"decrypt", decrypt}, {"cprt", cprt}, {"my_get", my_get}, {NULL, NULL}};
  int size = sizeof(menu) / sizeof(*menu);
  int bounds = 0;
  for (int i = 0; i < size; i++)
    if (menu[i].name)
      bounds++;
  int input = -1;
  while (1)
  {
    char arr[256];
    printf("Please choose a function:\n");
    for (int i = 0; i < size; i++)
      if (menu[i].name)
        printf("%d)  %s\n", i, menu[i].name);
    printf("Option : ");
    fgets(arr, 256, stdin);
    int parse = sscanf(arr, "%d", &input);
    if (parse == 0 || (input < 0 || input > bounds))
    {
      printf("\nNot within bounds\n");
      exit(0);
    }
    printf("\nWithin bounds\n");
    char *toDelete = carray;
    carray = map(carray, 5, menu[input].fun);
    free(toDelete);
    printf("DONE.\n\n");
  }
  free(carray);
}
