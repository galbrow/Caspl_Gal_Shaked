#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "elf.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

char * types(int type) {
    return type ==0x0?
    "SHT_NULL" :   //Section header table entry unused
    type ==0x1?
    "SHT_PROGBITS": //	Program data
    type ==0x2?
    "SHT_SYMTAB" :   //Symbol table
    type ==0x3?
    "SHT_STRTAB" :   //String table
    type ==0x4?
    "SHT_RELA" :   //Relocation entries with addends
    type ==0x5?
    "SHT_HASH" :   //Symbol hash table
    type ==0x6?
    "SHT_DYNAMIC": //	Dynamic linking information
    type ==0x7?
    "SHT_NOTE"  :  //Notes
    type ==0x8?
    "SHT_NOBITS" :   //Program space with no data (bss)
    type ==0x9?
    "SHT_REL"  :  //Relocation entries, no addends
    type ==0x0A?
    "SHT_SHLIB":    //Reserved
    type ==0x0B?
    "SHT_DYNSYM":    //Dynamic linker symbol table
    type ==0x0E?
    "SHT_INIT_ARRAY":// 	Array of constructors
    type ==0x0F?
    "SHT_FINI_ARRAY"    ://Array of destructors
    type ==0x10?
    "SHT_PREINIT_ARRAY": //	Array of pre-constructors
    type ==0x11?
    "SHT_GROUP" :   //Section group
    type ==0x12?
    "SHT_SYMTAB_SHNDX":    //Extended section indices
    type ==0x13?
    "SHT_NUM" :   //Number of defined types.
    type ==0x60000000?
    "SHT_LOOS":""; //	Start OS-specific.
}

char *map_start; /* will point to the start of the memory mapped file */
Elf32_Ehdr *header;
int fd;
char file_name[2048];
struct stat fd_stat; /* this is needed to  the size of the file */

char debug_mode = '0';
struct fun_desc
{
    char *name;
    void (*fun)();
} fun_desc;

void toggleDebugMode()
{
    // printf("start toggle, %c", elfHeader->debug_mode);
    if (debug_mode == '0')
    {
        debug_mode = '1';
        printf("Debug flag now on");
    }
    else
    {
        debug_mode = '0';
        printf("Debug flag now off");
    }
}

void examineElfFile()
{
    printf("please enter ELF file name\n");
    fgets(file_name, 2048, stdin);
    printf("file name: %s", file_name);
    file_name[strlen(file_name) - 1] = '\0';
    fd = open(file_name, O_RDWR);
    printf("file descriptor: %d\n", fd);
    if (fd < 0)
    {
        perror("error in open");
        exit(-1);
    }

    if (fstat(fd, &fd_stat) != 0)
    {
        perror("stat failed");
        exit(-1);
    }

    if ((map_start = mmap(0, fd_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        perror("mmap failed");
        exit(-4);
    }

    /* now, the file is mapped starting at map_start.
    * all we need to do is tell *header to point at the same address:
    */

    header = (Elf32_Ehdr *)map_start;
    /* now we can do whatever we want with header!!!!
    * for example, the number of section header can be obtained like this:
    */
    printf("magic number 1: %x\nmagic number 2: %x\nmagic number 3: %x\n", header->e_ident[0], header->e_ident[1], header->e_ident[2]);
    printf("entry point: %X\n", header->e_entry);
    printf("the number of section headers entry: %d\n", header->e_shnum);
    printf("the number of progeam sections %d", header->e_phnum);

    /* now, we unmap the file */
    //munmap(map_start, fd_stat.st_size);
}

void printSectionNames()
{
    Elf32_Half numOfSections=header->e_shnum;
    printf("There are %d section headers, starting at offset %x:\n\nSection headers:\n", numOfSections, header->e_shoff);
    printf("%4s\t%20s\t%10s\t%10s\t%10s\t%10s\n","[NR]","Name","Addr","Off","Size","Type");
    Elf32_Shdr *shdr=(Elf32_Shdr*)(map_start+header->e_shoff);
    char *shdrNames=(shdr+header->e_shstrndx)->sh_offset+map_start;
    for(int i=0;i<numOfSections;i++){
        printf("[%2d]\t%20s\t%10x\t%10x\t%10x\t%10s\n",i,shdrNames+shdr[i].sh_name,shdr[i].sh_addr,shdr[i].sh_offset,shdr[i].sh_size,types(shdr[i].sh_type));//,&shdr->sh_addr,&shdr->sh_offset,&shdr->sh_size,shdr->sh_type);
    }
}

void printSymbols()
{
    char* symbolNames;
    Elf32_Half numOfSections=header->e_shnum;
    Elf32_Shdr *shdr=(Elf32_Shdr*)(map_start+header->e_shoff);
    Elf32_Shdr *firstShdr=(Elf32_Shdr*)(map_start+header->e_shoff);
    Elf32_Sym* symTable;
    char *shdrNames=(shdr+header->e_shstrndx)->sh_offset+map_start;
    for(int i=0;i<numOfSections;i++){
        if(shdr[i].sh_type==0x2) {
            symTable = (void *) (map_start + shdr[i].sh_offset);
            symbolNames = shdr[shdr[i].sh_link].sh_offset+map_start;
            shdr=shdr+i;
        }
    }
    //data = elf_getdata(scn, NULL);
    int count = shdr->sh_size / shdr->sh_entsize;
    printf("%4s\t%10s\t%10s\t%10s\t%10s\n","[NR]","value","section_index","section_name","symbol_name");
    /* print the symbol names */
    for (int i = 0; i < count; ++i) {
        printf("[%2d]\t%10x\t%10d\t%10s\t%10s\n", i, symTable[i].st_value, symTable[i].st_shndx,
               symTable[i].st_shndx > numOfSections ? "" : shdrNames + firstShdr[symTable[i].st_shndx].sh_name,
               symbolNames + symTable[i].st_name == NULL ? "" : symbolNames + symTable[i].st_name);
    }
}

void relocationTables()
{
    Elf32_Sym * symTable;
    char* symbolNames;
    Elf32_Half numOfSections=header->e_shnum;
    Elf32_Shdr *shdr=(Elf32_Shdr*)(map_start+header->e_shoff);
    //Elf32_Shdr *firstShdr=(Elf32_Shdr*)(map_start+header->e_shoff);
    //Elf32_Sym* symTable;
    //char *shdrNames=(shdr+header->e_shstrndx)->sh_offset+map_start;
    Elf32_Rel *relTable;

    for(int i=0;i<numOfSections;i++){
        if(shdr[i].sh_type==SHT_DYNSYM) {
            symTable = (void *) (map_start + shdr[i].sh_offset);
            symbolNames = shdr[shdr[i].sh_link].sh_offset+map_start;
        }
    }

    for(int i=0;i<numOfSections;i++){
        if(shdr[i].sh_type==0x09) {
            relTable = (void *) (map_start + shdr[i].sh_offset);
            //shdr=shdr+i;
            int count = shdr[i].sh_size / shdr[i].sh_entsize;
            printf("%8s\t%8s\t%5s\t%8s\t%10s\n","Info","Offset","Type","Sym.value","Sym.Name");
            for(int j=0;j<count;j++){
                printf("%08x\t%08x\t%5d\t%08x\t%10s\n",relTable[j].r_info,relTable[j].r_offset, ELF32_R_TYPE(relTable[j].r_info),symTable[ELF32_R_SYM(relTable[j].r_info)].st_value,symbolNames+symTable[ELF32_R_SYM(relTable[j].r_info)].st_name);
            }
            printf("\n");
        }
    }


}

void quit()
{
    if (debug_mode == '1')
        printf("quitting");
    exit(0);
}

int main(int argc, char **argv)
{
    struct fun_desc menu[] = {{"Toggle Debug Mode", toggleDebugMode}, {"Examine ELF File", examineElfFile}, {"Print Section names", printSectionNames}, {"Print Symbols", printSymbols}, {"Relocation Tables", relocationTables}, {"Quit", quit}, {NULL, NULL}};
    int size = sizeof(menu) / sizeof(*menu);
    int bounds = 0;
    for (int i = 0; i < size; i++)
        if (menu[i].name)
            bounds++;
    int intOption = -1;
    while (1)
    {
        //printDebug(debug_mode);
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
        menu[intOption].fun();
        printf("\n\nDONE.\n\n");
    }
    return 0;
}
