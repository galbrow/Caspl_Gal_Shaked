#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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
} state;

struct fun_desc
{
    char *name;
    void (*fun)(state *);
} fun_desc;

char *unit_to_format(int unit_size)
{
    static char *formats[] = {"%#hhX\n", "%#hX\n", "No such unit", "%#X\n"};
    return formats[unit_size - 1];
}

char *unit_to_Decimal(int unit_size)
{
    static char *formats[] = {"%#hhd\n", "%#hd\n", "No such unit", "%#d\n"};
    return formats[unit_size - 1];
}

/* Prints the buffer to screen by converting it to text with printf */
void print_units(FILE *output, char *buffer, int count, int unit_size)
{
    char *beginBuf = buffer;
    char *end = buffer + unit_size * count;
    printf("Hex\n================\n");
    while (buffer < end)
    {
        //print ints
        int var = *((int *)(buffer));
        fprintf(output, unit_to_format(unit_size), var);
        buffer += unit_size;
    }
    end = beginBuf + unit_size * count;
    printf("Dec\n================\n");
    while (beginBuf < end)
    {
        //print ints
        int var = *((int *)(beginBuf));
        fprintf(output, unit_to_Decimal(unit_size), var);
        beginBuf += unit_size;
    }
}

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

void loadIntoMemory(state *state)
{
    if (state->file_name[0] == '\0')
    {
        fprintf(stderr, "there is no file name");
        return;
    }
    FILE *fp = fopen(state->file_name, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "failed to open file : %s", state->file_name);
        return;
    }
    char arr[NAME_LEN];
    printf("Please enter <location> <length>\n");
    fgets(arr, NAME_LEN, stdin);
    int arrLen = strlen(arr);
    char *len;
    char *location;
    for (int i = 0; i < arrLen; i++)
    {
        if (arr[i] == ' ')
        {
            arr[i] == '\0';
            location = arr;
            len = arr + i + 1;
            i = arrLen;
        }
    }
    int length = atoi(len);
    int locationHex;
    int parse = sscanf(location, "%X", &locationHex);
    if (parse == 0)
    {
        fprintf(stderr, "invalid input");
        fclose(fp);
        return;
    }
    int success = fseek(fp, locationHex, SEEK_SET);
    if (success == -1)
    {
        fprintf(stderr, "failed to get current location");
        fclose(fp);
        return;
    }
    fread(state->mem_buf, state->unit_size, length, fp);
    fclose(fp);
    if (state->debug_mode == '1')
        printf("file name:%s\nlocation: %x\nlength:%d\n", state->file_name, locationHex, length);
    printf("Loaded %d units into memory\n", length);
}

void memoryDisplay(state *state)
{
    char arr[NAME_LEN];
    printf("Enter address and length\n");
    fgets(arr, NAME_LEN, stdin);
    int arrLen = strlen(arr);
    char *units;
    char *addr;
    for (int i = 0; i < arrLen; i++)
    {
        if (arr[i] == ' ')
        {
            arr[i] == '\0';
            addr = arr;
            units = arr + i + 1;
            i = arrLen;
        }
    }
    int length = atoi(units);
    int locationHex;
    int parse = sscanf(addr, "%X", &locationHex);
    if (parse == 0)
    {
        fprintf(stderr, "invalid input");
        return;
    }
    if (locationHex == 0)
        print_units(stdout, state->mem_buf, length, state->unit_size);
    else
        print_units(stdout, (char *)locationHex, length, state->unit_size);
}

void saveIntoFile(state *state)
{
    char arr[NAME_LEN];
    printf("Please enter <source-address> <target-location> <length>\n");
    fgets(arr, NAME_LEN, stdin);
    int target, sourceAddr, len;
    int parse = sscanf(arr, "%x %x %d\n", &sourceAddr, &target, &len);
    if (parse == 0)
    {
        fprintf(stderr, "invalid input");
        return;
    }
    FILE *fp = fopen(state->file_name, "r+b");
    if (fp == NULL)
    {
        fprintf(stderr, "failed to open file : %s", state->file_name);
        return;
    }
    fseek(fp, 0L, SEEK_END);
    int res = ftell(fp);
    if (res < target)
    {
        printf("fileName:%s\nres: %d \ntarget: %x\n", state->file_name, res, target);
        fprintf(stderr, "file is shorter than target");
    }
    else
    {
        printf("%p", &state->mem_buf);
        fseek(fp, target, SEEK_SET);
        if (sourceAddr == 0)
            fwrite(state->mem_buf, state->unit_size, len, fp);
        else
            fwrite(&sourceAddr, state->unit_size, len, fp);
        if (state->debug_mode == '1')
            printf("file name:%s\nlocation: %x\nlength:%d\n", state->file_name, sourceAddr, len);
    }
    fclose(fp);
}

void memoryModify(state *state)
{
    char arr[NAME_LEN];
    printf("Please enter <location> <val>");
    fgets(arr, NAME_LEN, stdin);
    int location, val;
    int parse = sscanf(arr, "%x %x\n", &location, &val);
    if (parse == 0)
    {
        fprintf(stderr, "invalid input");
        return;
    }
    FILE *fp = fopen(state->file_name, "r+");
    if (fp == NULL)
    {
        fprintf(stderr, "failed to open file : %s", state->file_name);
        return;
    }
    fseek(fp, 0L, SEEK_END);
    int res = ftell(fp);
    if (res < location)
        fprintf(stderr, "file is shorter than target");
    else
    {
        fseek(fp, location, SEEK_SET);
        fwrite(&val, state->unit_size, 1, fp);
        if (state->debug_mode == '1')
            printf("location: %x\nval:%x\n");
    }
    fclose(fp);
}

void printDebug(state *state)
{
    if (state->debug_mode == '1')
        printf("Unit size: %d\nFile name: %s\nMem count: %d\n\n", state->unit_size, state->file_name, state->mem_count);
}

void quit(state *state)
{
    if (state->debug_mode == '1')
        printf("quitting");
    exit(0);
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
