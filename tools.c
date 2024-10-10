#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "tools.h"

#define SUCCESS_EXIT 0
#define FAILURE_EXIT 1
#define INCORRECT_DAY_ERROR 12
#define INCORRECT_MONTH_ERROR 13
#define INCORRECT_YEAR_ERROR 14

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

int read_date(FILE *f, date_t *date)
{
    int rc;
    string_t buf;

    int day;
    if (!fgets(buf, sizeof(buf), f))
        return INCORRECT_DAY_ERROR;

    if (rc = str_to_int(buf, &day))
        return INCORRECT_DAY_ERROR;

    int month;
    if (!fgets(buf, sizeof(buf), f))
        return INCORRECT_MONTH_ERROR;

    if (rc = str_to_int(buf, &month))
        return INCORRECT_MONTH_ERROR;

    int year;
    if (!fgets(buf, sizeof(buf), f))
        return INCORRECT_YEAR_ERROR;

    if (rc = str_to_int(buf, &year))
        return INCORRECT_YEAR_ERROR;

    if (year < 1920)
        return INCORRECT_YEAR_ERROR;
    int is_leap = year % 4;

    if (month < 1 || month > 12)
        return INCORRECT_MONTH_ERROR;
    
    if (day < 1 || day > 31)
        return INCORRECT_DAY_ERROR;
    
    int max_days[] = {31, 28 + is_leap_year(year), 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (day > max_days[month - 1])
        return INCORRECT_DAY_ERROR;

    date->day = day;
    date->month = month;
    date->year = year;

    return SUCCESS_EXIT;
}

int is_leap_year(int year)
{
    if (year % 4)
        return 0;
    return 1;
}

/*
Функция прибавляет к дате количество дней.
Количество дней должно быть положительным, иначе функция не изменяет исходную дату.
*/
static void plus_days(date_t *date, size_t daycount)
{
    int max_days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (daycount < 1)
        return;
    
    date->day += daycount;
    while (date->day > max_days[date->month - 1])
    {
        if (date->month == 2 && is_leap_year(date->year))
        {   
            if (date->day > 29)
            {
                date->day -= 29;
                date->month++;
            }
            else
                break;
        }

        date->day -= max_days[date->month - 1];
        date->month++;

        if (date->month > 12)
        {
            date->month -= 12;
            date->year++;
        }
        
    }
}

/*
Функция возвращает 1, если date находится в промежутке от date_s до date_s + 6 дней.
*/
int is_date_this_week(date_t date, date_t date_s)
{
    date_t cur_date = {.day = date_s.day, .month = date_s.month, .year = date_s.year};

    for (size_t i = 0; i < 7; i++)
    {
        if ((cur_date.day == date.day) && (cur_date.month == date.month))
            return 1;
        plus_days(&cur_date, 1);
    }
    return 0;
}

/*
Функция форматирует строку string к требуемому программой виду и копирует ее в dst.
Возвращает 0 при успешном форматировании, иначе err
*/
int format_string(string_t string, string_t dst, int err)
{
    if (!strlen(string))
        return err;

    if (string[strlen(string) - 1] == '\n')
        string[strlen(string) - 1] = '\0';

    if (!strlen(string))
        return err;

    for (size_t i = 0; i < strlen(string); i++)
    {
        if (!(string[i] >= 'A' && string[i] <= 'Z') && !(string[i] >= 'a' && string[i] <= 'z'))
            return err;
    }

    strcpy(dst, string);
    return SUCCESS_EXIT;
}

int format_number(string_t string, string_t dst, int err)
{
    if (string[strlen(string) - 1] == '\n')
        string[strlen(string) - 1] = '\0';

    if (string[0] != '+' || string[1] != '7' || strlen(string) != 12)
        return err;

    for (size_t i = 2; i < strlen(string); i++)
    {
        if (!(string[i] >= '0' && string[i] <= '9'))
            return err;
    }

    strcpy(dst, string);
    return SUCCESS_EXIT;
}
