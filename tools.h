#ifndef TOOLS_H
#define TOOLS_H
#include <stdio.h>

#define MAX_STR_LEN 50

typedef char string_t[MAX_STR_LEN + 2];

typedef struct
{
    int day;
    int month;
    int year;
} date_t;

int str_to_int(char *str, int *num);
int read_date(FILE *f, date_t *date);
int is_leap_year(int year);
int is_date_this_week(date_t date, date_t date_s);

#endif