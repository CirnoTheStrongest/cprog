#include <stdio.h>
#include "file.h"
#include <string.h>

#define SUCCESS_EXIT 0
#define INIT_FILE_ERROR 200

int init_file(file_t file)
{
    char str[MAX_FILENAME_LEN];
    puts("Введите имя файла (если файл не существует, то он будет создан при возможности):");
    if (!fgets(str, sizeof(str), stdin))
        return INIT_FILE_ERROR;

    if (str[strlen(str) - 1] == '\n')
        str[strlen(str) - 1] = '\0';

    FILE *f;
    if ((f = fopen(str, "a")) != NULL)
    {
        fclose(f);
        strcpy(file, str);
        return SUCCESS_EXIT;
    }
    return INIT_FILE_ERROR;
}