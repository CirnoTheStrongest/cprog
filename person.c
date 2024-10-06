#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "person.h"

#define SUCCESS_EXIT 0
#define INCORRECT_STATUS_ERR 6
#define INCORRECT_SURNAME_ERR 7
#define INCORRECT_NAME_ERROR 8
#define INCORRECT_NUMBER_ERROR 9
#define INCORRECT_STREET_ERROR 10
#define INCORRECT_HOUSE_NUMBER_ERROR 11
#define INCORRECT_DAY_ERROR 12
#define INCORRECT_MONTH_ERROR 13
#define INCORRECT_YEAR_ERROR 14
#define INCORRECT_POSITION_ERROR 15
#define INCORRECT_ORGANIZATION_ERROR 16

static int read_surname(FILE *f, char *surname, size_t size)
{
    if (!fgets(surname, size, f))
        return INCORRECT_SURNAME_ERR;
    if (surname[strlen(surname) - 1] == '\n')
        surname[strlen(surname) - 1] = '\0';
    return SUCCESS_EXIT;
}

static int read_name(FILE *f, char *name, size_t size)
{
    if (!fgets(name, size, f))
        return INCORRECT_NAME_ERROR;
    if (name[strlen(name) - 1] == '\n')
        name[strlen(name) - 1] = '\0';
    return SUCCESS_EXIT;
}

static int read_phone_number(FILE *f, char *number, size_t size)
{
    char *buf = malloc(size);
    if (!fgets(buf, size, f))
        return INCORRECT_NUMBER_ERROR;

    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';
            
    // +79887105546 - номер телефона российский, с +7 + 10 цифр
    if (buf[0] != '+' || buf[1] != '7' || strlen(buf) != 12)
        return INCORRECT_NUMBER_ERROR;
    
    strcpy(number, buf);
    return SUCCESS_EXIT;
}

static int read_adress(FILE *f, adress_t *adress)
{
    char *buf = malloc(sizeof(adress->street));
    if (!fgets(buf, sizeof(adress->street), f))
        return INCORRECT_STREET_ERROR;
    if (!isupper(buf[0]))
        return INCORRECT_STREET_ERROR;

    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';
    strcpy(adress->street, buf);

    int houseNumber;
    if (!fscanf(f, "%d\n", &houseNumber))
        return INCORRECT_HOUSE_NUMBER_ERROR;
    if (houseNumber < 1)
        return INCORRECT_HOUSE_NUMBER_ERROR;
    adress->houseNumber = houseNumber;

    return SUCCESS_EXIT;
}

static int read_status(FILE *f, char *status, size_t size)
{
    char *buf = malloc(sizeof(size));
    if (!fgets(buf, size, f))
        return INCORRECT_STREET_ERROR;

    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';

    if (strcmp(buf, "Друг") && strcmp(buf, "Коллега"))
        return INCORRECT_STREET_ERROR;

    strcpy(status, buf);

    return SUCCESS_EXIT;
}

static int is_leap_year(int year)
{
    if (year % 4)
        return 0;
    return 1;
}

static int read_birthday(FILE *f, birthday_t *birthday)
{
    int day;
    if (fscanf(f, "%d\n", &day) != 1)
        return INCORRECT_DAY_ERROR;

    int month;
    if (fscanf(f, "%d\n", &month) != 1)
        return INCORRECT_MONTH_ERROR;

    int year;
    if (fscanf(f, "%d\n", &year) != 1)
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

    birthday->day = day;
    birthday->month = month;
    birthday->year = year;
}

static int read_work(FILE *f, work_t *work)
{
    char *buf = malloc(sizeof(work->position));

    if (!fgets(buf, sizeof(work->position), f))
        return INCORRECT_POSITION_ERROR;

    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';
    strcpy(work->position, buf);

    if (!fgets(buf, sizeof(work->position), f))
        return INCORRECT_ORGANIZATION_ERROR;

    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';
    strcpy(work->organization, buf);

    return SUCCESS_EXIT;
}

static int read_info(FILE *f, info_t *info, char *status)
{
    if (!strcmp(status, "Друг"))
        read_birthday(f, &info->birthday);
    else
        read_work(f, &info->work);
}

int read_person(FILE *f, person_t *person)
{
    read_surname(f, person->surname, sizeof(person->surname));

    read_name(f, person->name, sizeof(person->name));

    read_phone_number(f, person->phoneNumber, sizeof(person->phoneNumber));

    read_adress(f, &person->adress);

    read_status(f, person->status, sizeof(person->status));

    read_info(f, &person->info, person->status);
}