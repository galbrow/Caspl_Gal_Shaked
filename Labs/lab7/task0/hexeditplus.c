#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NAME_LEN 128
#define BUF_SZ 10000
//void fun(state* s);

typedef struct
{
    char debug_mode;
    char file_name[NAME_LEN];
    int unit_size;
    unsigned char mem_buf[BUF_SZ];
    size_t mem_count;
    /*
   .
   .
   Any additional fields you deem necessary
  */
} state;

struct fun_desc
{
    char *name;
    void (*fun)(state *);
} fun_desc;

void toggleDebugMode(state *state)
{
    // printf("start toggle, %c", state->debug_mode);
    if (state->debug_mode == '0')
    {
        state->debug_mode = '1';
        printf("Debug flag now on");
    }
    else
    {
        state->debug_mode = '0';
        printf("Debug flag now off");
    }
}

void setFileName(state *state)
{
    printf("please enter file name: ");
    fgets(state->file_name, 2048, stdin); // get name from user
    state->file_name[strlen(state->file_name) - 1] = '\0';
    if (state->debug_mode == '1')
        printf("Debug: file name set to %s ", state->file_name);
}

void setUnitSize(state *state)
{
    int intOption;
    char option[10];
    fgets(option, 10, stdin);
    int parse = sscanf(option, "%d", &intOption);
    if (parse != 0 && (intOption == 1 || intOption == 2 || intOption == 4))
    {
        state->unit_size = intOption;
        if (state->debug_mode == '1')
            printf("Debug: set size to %s", option);
    }
    else
        fprintf(stderr, "not a valid size: %s", option);
}

void quit(state *state)
{
    if (state->debug_mode == '1')
        printf("quitting");
    exit(0);
}

void loadIntoMemory(state *state)
{
    printf("not implemented yet");
}

void memoryDisplay(state *state)
{
    printf("not implemented yet");
}

void saveIntoFile(state *state)
{
    printf("not implemented yet");
}

void memoryModify(state *state)
{
    printf("not implemented yet");
}

void printDebug(state *state)
{
    if (state->debug_mode == '1')
        printf("Unit size: %d\nFile name: %s\nMem count: %d\n\n", state->unit_size, state->file_name, state->mem_count);
}

int main(int argc, char **argv)
{
    state *state = calloc(1, sizeof(state));
    state->debug_mode = '0';
    struct fun_desc menu[] = {{"Toggle Debug Mode", toggleDebugMode}, {"Set File Name", setFileName}, {"Set Unit Size", setUnitSize}, {"Load Into Memory", loadIntoMemory}, {"Memory Display", memoryDisplay}, {"Save Into File", saveIntoFile}, {"Memory Modify", memoryModify}, {"Quit", quit}, {NULL, NULL}};
    int size = sizeof(menu) / sizeof(*menu);
    int bounds = 0;
    for (int i = 0; i < size; i++)
        if (menu[i].name)
            bounds++;
    int intOption = -1;
    while (1)
    {
        printDebug(state);
        char option[256];
        printf("Please choose a function:\n");
        for (int i = 0; i < size; i++)
            if (menu[i].name)
                printf("%d-%s\n", i, menu[i].name);
        printf("Option : ");
        fgets(option, 256, stdin);
        int parse = sscanf(option, "%d", &intOption);
        if (parse == 0 || (intOption < 0 || intOption > bounds))
        {
            printf("\nNot within bounds\n");
            exit(0);
        }
        printf("\nWithin bounds:\n\n");
        // printf("%d", intOption);
        menu[intOption].fun(state);
        printf("\n\nDONE.\n\n");
    }
    free(state);
    return 0;
}
