#ifndef PERSON_H
#define PERSON_H

#include <stdio.h>

#define MAX_STR_LEN 50
#define MAX_ARRAY_LEN 10240

typedef struct
{
    char street[MAX_STR_LEN + 2];
    int houseNumber;
} adress_t;

typedef struct
{
    int day;
    int month;
    int year;
} birthday_t;

typedef struct
{
    char position[MAX_STR_LEN + 2];
    char organization[MAX_STR_LEN + 2];
} work_t;


typedef union
{
    birthday_t birthday;
    work_t work;
} info_t;


typedef struct
{
    char surname[MAX_STR_LEN + 2];
    char name[MAX_STR_LEN + 2];
    char phoneNumber[MAX_STR_LEN + 2];
    adress_t adress;
    char status[MAX_STR_LEN + 2];
    info_t info;
} person_t;

typedef struct
{
    person_t array[MAX_ARRAY_LEN];
    size_t size;
} person_array_t;


int read_person(FILE *f, person_t *person);

#endif