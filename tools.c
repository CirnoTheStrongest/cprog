#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tools.h"

#define SUCCESS_EXIT 0
#define FAILURE_EXIT 1

int str_to_int(char *str, int *num)
{
    if (!strlen(str))
        return FAILURE_EXIT;

    if (str[strlen(str) - 1] == '\n')
        str[strlen(str) - 1] = '\0';
    
    if (!strlen(str))
        return FAILURE_EXIT;

    for (size_t i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]))
            return FAILURE_EXIT;
    }

    *num = atoi(str);
    return SUCCESS_EXIT;
}