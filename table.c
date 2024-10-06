#include <string.h>
#include "table.h"

/*
      20                      20                14*           20                    9*            40
----------------------------------------------------------------------------------------------------------------------------------
|     Surmane        |         Name       | PhoneNumber  |   Address          | Status  |                  Info                  |
----------------------------------------------------------------------------------------------------------------------------------
| Ivanov             | Ivan               | +79887105546 | Ylitsa, 0          | Коллега | День Рождения: 16.11.2004              |
----------------------------------------------------------------------------------------------------------------------------------
| Ivanov             | Ivan               | +79887105546 | Ylitsa, 0          | Коллега | Должность: Middle                      |
|                    |                    |              |                    |         | Организация: Company                   |
----------------------------------------------------------------------------------------------------------------------------------
*/

static void draw_table_head(void)
{
    printf("----------------------------------------------------------------------------------------------------------------------------------\n"
           "|     Surname        |         Name       | PhoneNumber  |   Address          | Status  |                  Info                  |\n"
           "----------------------------------------------------------------------------------------------------------------------------------\n");
}

static void draw_table_element(const person_t person)
{
    if (!strcmp(person.status, "Друг"))
        printf("| %-18.18s | %-18.18s | %-12.12s | %-13.13s %4d |  Друг   | День Рождения: %02d.%02d.%d              |\n"
               "----------------------------------------------------------------------------------------------------------------------------------\n",
               person.surname, person.name, person.phoneNumber, person.adress.street, person.adress.houseNumber, person.info.birthday.day,
               person.info.birthday.month, person.info.birthday.year);
    else
        printf("| %-18.18s | %-18.18s | %-12.12s | %-13.13s %4d | Коллега | Должность: %-27.27s |\n"
               "|                    |                    |              |                    |         | Организация: %-25.25s |\n"
               "----------------------------------------------------------------------------------------------------------------------------------\n",
               person.surname, person.name, person.phoneNumber, person.adress.street, person.adress.houseNumber, person.info.work.position,
               person.info.work.organization);
}

void draw_table(person_array_t person_array_t)
{
    draw_table_head();
    for (size_t i = 0; i < person_array_t.size; i++)
        draw_table_element(person_array_t.array[i]);
}