#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../headers/token.h"
#include "../headers/codebook.h"

void usage();
char *file_to_string(FILE *);

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        usage();
        return 0;
    }

    FILE *file = fopen(argv[1], "r");

    if (!file)
    {
        printf("Error: file '%s' could not be opened.\n", argv[1]);
        return 0;
    }

    char *string = file_to_string(file);
    
    fclose(file);

    free(string);
}

void usage()
{
    printf("usage: cb3 <file>\n");
}

char *file_to_string(FILE *file)
{
    fseek(file, 0L, SEEK_END);
    size_t fsize = ftell(file);
    rewind(file);
    char *string = malloc(sizeof(char)*fsize+1);
    fread(string, sizeof(char), fsize, file);
    string[fsize] = '\0';
    return string;
}
