#include "person.h"

#define INCORRECT_STATUS_ERR 6

int read_person(FILE *f, person_t *person)
{
    fgets(person->lastName, sizeof(person->lastName), f);
    fgets(person->firstName, sizeof(person->firstName), f);
    fgets(person->phoneNumber, sizeof(person->phoneNumber), f);

    fgets(person->adress.street, sizeof(person->adress.street), f);
    fscanf(f, "%d\n", person->adress.houseNumber);
    fgets(person->status, sizeof(person->status), f);

    if (!strcmp(person->status, "Друг"))
    {
        fscanf(f, "%d\n", person->info.birthday.day);
        fscanf(f, "%d\n", person->info.birthday.month);
        fscanf(f, "%d\n", person->info.birthday.year);
    }
    else if (!strcmp(person->status, "Коллега"))
    {
        fgets(person->info.work.position, sizeof(person->info.work.position), f);
        fgets(person->info.work.organization, sizeof(person->info.work.organization), f);
    }
    else
    {
        return INCORRECT_STATUS_ERR;
    }
}