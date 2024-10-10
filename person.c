#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "person.h"
#include "tools.h"

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

static int read_surname(FILE *f, string_t surname)
{
    if (!fgets(surname, sizeof(string_t), f))
        return INCORRECT_SURNAME_ERR;

    if (!strlen(surname))
        return INCORRECT_SURNAME_ERR;

    if (surname[strlen(surname) - 1] == '\n')
        surname[strlen(surname) - 1] = '\0';

    if (!strlen(surname))
        return INCORRECT_SURNAME_ERR;

    return SUCCESS_EXIT;
}

static int read_name(FILE *f, string_t name)
{
    if (!fgets(name, sizeof(string_t), f))
        return INCORRECT_NAME_ERROR;
    if (name[strlen(name) - 1] == '\n')
        name[strlen(name) - 1] = '\0';
    return SUCCESS_EXIT;
}

static int read_phone_number(FILE *f, string_t number)
{
    string_t buf;
    if (!fgets(buf, sizeof(string_t), f))
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
    int rc;
    string_t buf;
    if (!fgets(buf, sizeof(string_t), f))
        return INCORRECT_STREET_ERROR;
    //if (!isupper(buf[0]))
    //    return INCORRECT_STREET_ERROR;

    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';
    strcpy(adress->street, buf);

    int houseNumber;
    if (!fgets(buf, sizeof(string_t), f))
        return INCORRECT_HOUSE_NUMBER_ERROR;

    if (rc = str_to_int(buf, &houseNumber))
        return INCORRECT_HOUSE_NUMBER_ERROR;

    if (houseNumber < 1)
        return INCORRECT_HOUSE_NUMBER_ERROR;
    adress->houseNumber = houseNumber;

    return SUCCESS_EXIT;
}

static int read_status(FILE *f, string_t status)
{
    string_t buf;
    if (!fgets(buf, sizeof(string_t), f))
        return INCORRECT_STREET_ERROR;

    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';

    if (strcmp(buf, "Friend") && strcmp(buf, "Partner"))
        return INCORRECT_STREET_ERROR;

    strcpy(status, buf);

    return SUCCESS_EXIT;
}

static int read_birthday(FILE *f, birthday_t *birthday)
{
    return read_date(f, birthday);
}

static int read_work(FILE *f, work_t *work)
{
    string_t buf;

    if (!fgets(buf, sizeof(string_t), f))
        return INCORRECT_POSITION_ERROR;

    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';
    strcpy(work->position, buf);

    if (!fgets(buf, sizeof(string_t), f))
        return INCORRECT_ORGANIZATION_ERROR;

    if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';
    strcpy(work->organization, buf);

    return SUCCESS_EXIT;
}

static int read_info(FILE *f, info_t *info, string_t status)
{
    if (!strcmp(status, "Friend"))
        read_birthday(f, &info->birthday);
    else
        read_work(f, &info->work);

    return SUCCESS_EXIT;
}

int read_person(FILE *f, person_t *person, char mode)
{
    int rc;
    if (mode == 't')
        puts("Введите фамилию абонента:");
    if (rc = read_surname(f, person->surname))
        return rc;

    if (mode == 't')
        puts("Введите имя абонента:");
    read_name(f, person->name);

    if (mode == 't')
        puts("Введите номер абонента в формате +7**********:");
    read_phone_number(f, person->phoneNumber);

    if (mode == 't')
        puts("Введите улицу и номер дома абонента через Enter:");
    read_adress(f, &person->adress);

    if (mode == 't')
        puts("Введите статус абонента:");
    read_status(f, person->status);

    if (mode == 't')
    {
        if (!strcmp(person->status, "Friend"))
            puts("Введите день, месяц и год рождения абонента через Enter:");
        else
            puts("Введите должность и организацию абонента через Enter:");
    }
    read_info(f, &person->info, person->status);
    return SUCCESS_EXIT;
}