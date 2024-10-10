#ifndef PERSON_H
#define PERSON_H

#include <stdio.h>
#include "tools.h"

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