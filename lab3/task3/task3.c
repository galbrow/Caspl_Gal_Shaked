#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct virus
{
    unsigned short SigSize;
    char virusName[16];
    unsigned char *sig;
} virus;

typedef struct link link;

struct link
{
    link *nextVirus;
    virus *vir;
};

struct fun_desc
{
    char *name;
    link *(*fun)(link *, char *);
};

virus *readVirus(FILE *fp)
{
    virus *virus1 = calloc(1, sizeof(struct virus));
    char sigSizeStr[2];
    fread(sigSizeStr, 1, 2, fp);
    virus1->SigSize = (sigSizeStr[0]) | sigSizeStr[1] << 8;
    fread(virus1->virusName, 1, 16, fp);
    virus1->sig = malloc(virus1->SigSize);
    fread(virus1->sig, 1, virus1->SigSize, fp);
    return virus1;
}

virus *readVirusBig(FILE *fp)
{
    virus *virus1 = calloc(1, sizeof(struct virus));
    char sigSizeStr[2];
    fread(sigSizeStr, 1, 2, fp);
    virus1->SigSize = (sigSizeStr[0] << 8) | sigSizeStr[1];
    fread(virus1->virusName, 1, 16, fp);
    virus1->sig = malloc(virus1->SigSize);
    fread(virus1->sig, 1, virus1->SigSize, fp);
    return virus1;
}

void printVirus(virus *virus, FILE *output)
{
    fprintf(output, "virus name: %s\nvirus size: %d\nvirus signature:\n", virus->virusName, virus->SigSize);
    for (int i = 0; i < virus->SigSize; i++)
    {
        fprintf(output, "%02X ", virus->sig[i] & 0xFF);
    }
    fprintf(output, "\n");
}

void list_print(link *virus_list, FILE *file)
{
    while (1)
    {
        printVirus(virus_list->vir, file);
        if (virus_list->nextVirus == NULL)
            break;
        virus_list = virus_list->nextVirus;
    }
}

link *PrintSignatures(link *virus_list, char *file)
{
    if (virus_list->vir != NULL)
    {
        list_print(virus_list, stdout);
    }
    return virus_list;
}
link *list_append(link *virus_list, virus *data)
{
    if (!virus_list->vir)
    {
        virus_list->vir = data;
        return virus_list;
    }
    if (data->virusName[0] == '\0')
    {
        free(data->sig);
        free(data);
        return virus_list;
    }

    link *firstLink = virus_list;
    link *lnk = calloc(1, sizeof(struct link));
    while ((virus_list->nextVirus) != NULL)
    {
        virus_list = virus_list->nextVirus;
    }
    lnk->vir = data;
    virus_list->nextVirus = lnk;
    return firstLink;
}
void list_free(link *virus_list)
{
    link *lnk = virus_list;
    while (virus_list != NULL)
    {
        if (virus_list->vir != NULL)
        {
            free(virus_list->vir->sig);
            free(virus_list->vir);
        }
        lnk = virus_list->nextVirus;
        free(virus_list);
        virus_list = lnk;
    }
}

link *LoadSignatures(link *lnk, char *file)
{
    list_free(lnk);
    lnk = calloc(1, sizeof(struct link));
    char *input = malloc(256);
    scanf("%s", input);
    FILE *fp = fopen(input, "r");
    fgetc(stdin);
    fgetc(fp);
    fgetc(fp);
    fgetc(fp);
    char c = fgetc(fp);
    virus *virus;
    while (1)
    {
        if (feof(fp))
            break;
        if (c == 'B')
            virus = readVirusBig(fp);
        else
            virus = readVirus(fp);
        lnk = list_append(lnk, virus);
    }
    fclose(fp);
    free(input);
    return lnk;
}

void detect_virus(char *buffer, unsigned int size, link *virus_list, FILE *output)
{
    while (virus_list->vir != NULL)
    {
        for (int i = 0; i < size; i++)
        {
            if (memcmp(buffer + i, virus_list->vir->sig, virus_list->vir->SigSize) == 0)
            {
                fprintf(output, "The starting byte location in the suspected file: %d\nThe virus name: %s\nThe size of the virus signature: %d\n", i, virus_list->vir->virusName, virus_list->vir->SigSize);
            }
        }
        if (virus_list->nextVirus == NULL)
            break;
        virus_list = virus_list->nextVirus;
    }
}

link *DetectViruses(link *virus_list, char *file)
{
    FILE *fp = fopen(file, "r");
    char *buffer = malloc(10000);
    unsigned long len = fread(buffer, 1, 10000, fp);
    detect_virus(buffer, len, virus_list, stdout);
    free(buffer);
    fclose(fp);
    return virus_list;
}

void kill_virus(char *fileName, int signitureOffset, int signitureSize)
{
    FILE *fp = fopen(fileName, "r+");
    fseek(fp, signitureOffset, SEEK_SET);
    char array[signitureSize];
    for (int i = 0; i < signitureSize; i++)
        array[i] = 144;
    fwrite(array, signitureSize, 1, fp);
    fclose(fp);
}

link *FixFile(link *lnk, char *file)
{
    FILE *fp = fopen(file, "r");
    char *buffer = malloc(10000);
    unsigned long len = fread(buffer, 1, 10000, fp);
    fclose(fp);
    link *first = lnk;
    while (lnk->vir != NULL)
    {
        for (int i = 0; i < len; i++)
        {
            if (memcmp(buffer + i, lnk->vir->sig, lnk->vir->SigSize) == 0)
            {
                kill_virus(file, i, lnk->vir->SigSize);
            }
        }
        if (lnk->nextVirus == NULL)
            break;
        lnk = lnk->nextVirus;
    }
    free(buffer);
    return first;
}

int main(int argc, char **argv)
{
    struct fun_desc menu[] = {{NULL, NULL}, {"Load signatures", LoadSignatures}, {"Print signatures", PrintSignatures}, {"Detect viruses", DetectViruses}, {"Fix file", FixFile}};
    int size = sizeof(menu) / sizeof(*menu);
    int bounds = 1;
    for (int i = 0; i < size; i++)
    {
        if (menu[i].name)
            bounds++;
    }
    int input = -1;
    link *lnk = calloc(1, sizeof(struct link));
    lnk->vir = NULL;
    while (1)
    {
        char arr[256];
        printf("please choose a function:\n");

        for (int i = 0; i < size; i++)
        {
            if (menu[i].name)
                printf("%d)  %s\n", i, menu[i].name);
        }
        printf("Option : ");
        fgets(arr, 256, stdin);
        int parse = sscanf(arr, "%d", &input);
        if (parse == 0 || (input < 1 || input > bounds - 1))
        {
            printf("\nNot within bounds\n");
            list_free(lnk);
            exit(0);
        }
        printf("\nWithin bounds\n");
        lnk = menu[input].fun(lnk, argv[1]);
    }
    return 0;
}