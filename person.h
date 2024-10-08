#ifndef PERSON_H
#define PERSON_H

#include <stdio.h>
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
#define MAX_ARRAY_LEN 10240

typedef struct
{
    string_t street;
    int houseNumber;
} adress_t;

typedef date_t birthday_t;

typedef struct
{
    string_t position;
    string_t organization;
} work_t;


typedef union
{
    birthday_t birthday;
    work_t work;
} info_t;


typedef struct
{
    string_t surname;
    string_t name;
    string_t phoneNumber;
    adress_t adress;
    string_t status;
    info_t info;
} person_t;

typedef struct
{
    person_t array[MAX_ARRAY_LEN];
    size_t size;
} person_array_t;

typedef struct
{
    string_t key;
    size_t index;
} person_key_t;

typedef struct
{
    person_key_t array[MAX_ARRAY_LEN];
    size_t size;
} person_key_array_t;


// mode f - без сопровождаемого текста, t - с текcтом
int read_person(FILE *f, person_t *person, char mode);

#endif