#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "person.h"
#include "tools.h"

static int read_surname(FILE *f, string_t surname)
{
    string_t buf;
    int rc;
    if (!fgets(buf, sizeof(string_t), f))
        return INCORRECT_SURNAME_ERR;

    if (rc = format_string(buf, surname, INCORRECT_SURNAME_ERR))
        return rc;

    return SUCCESS_EXIT;
}

static int read_name(FILE *f, string_t name)
{
    string_t buf;
    int rc;
    if (!fgets(buf, sizeof(string_t), f))
        return INCORRECT_NAME_ERROR;
    
    if (rc = format_string(buf, name, INCORRECT_NAME_ERROR))
        return rc;

    return SUCCESS_EXIT;
}

static int read_phone_number(FILE *f, string_t number)
{
    string_t buf;
    int rc;
    if (!fgets(buf, sizeof(string_t), f))
        return INCORRECT_NUMBER_ERROR;

    if (rc = format_number(buf, number, INCORRECT_NUMBER_ERROR))
        return rc;
    
    return SUCCESS_EXIT;
}

static int read_adress(FILE *f, adress_t *adress)
{
    int rc;
    string_t buf;
    if (!fgets(buf, sizeof(string_t), f))
        return INCORRECT_STREET_ERROR;

    if (rc = format_string(buf, adress->street, INCORRECT_STREET_ERROR))
        return rc;

    int houseNumber;
    if (!fgets(buf, sizeof(string_t), f))
        return INCORRECT_HOUSE_NUMBER_ERROR;

    if (rc = str_to_int(buf, &houseNumber))
        return INCORRECT_HOUSE_NUMBER_ERROR;

    if (houseNumber < 1 || houseNumber > 1000)
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
    int rc;

    if (!fgets(buf, sizeof(string_t), f))
        return INCORRECT_POSITION_ERROR;

    if (rc = format_string(buf, work->position, INCORRECT_POSITION_ERROR))
        return rc;

    if (!fgets(buf, sizeof(string_t), f))
        return INCORRECT_ORGANIZATION_ERROR;

    if (rc = format_string(buf, work->organization, INCORRECT_ORGANIZATION_ERROR))
        return rc;

    return SUCCESS_EXIT;
}

static int read_info(FILE *f, info_t *info, string_t status)
{
    if (!strcmp(status, "Friend"))
        return read_birthday(f, &info->birthday);
    else
        return read_work(f, &info->work);
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
    if (rc = read_name(f, person->name))
        return rc;

    if (mode == 't')
        puts("Введите номер абонента в формате +7**********:");
    if (rc = read_phone_number(f, person->phoneNumber))
        return rc;

    if (mode == 't')
        puts("Введите улицу и номер дома абонента через Enter:");
    if (rc = read_adress(f, &person->adress))
        return rc;

    if (mode == 't')
        puts("Введите статус абонента:");
    if (rc = read_status(f, person->status))
        return rc;

    if (mode == 't')
    {
        if (!strcmp(person->status, "Friend"))
            puts("Введите день, месяц и год рождения абонента через Enter:");
        else
            puts("Введите должность и организацию абонента через Enter:");
    }

    if (rc = read_info(f, &person->info, person->status))
        return rc;

    return SUCCESS_EXIT;
}