#include <string.h>
#include "person.h"
#include "table.h"
#include "file.h"

#define SUCCESS_EXIT 0
#define ARR_OVERFLOW_ERROR 100
#define DELETE_ERROR 101

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


  5 + 1
---------------------------
| index |   surname      |
---------------------------
*/

static void draw_table_head(void)
{
    printf("----------------------------------------------------------------------------------------------------------------------------------\n"
           "|     Surname        |         Name       | PhoneNumber  |   Address          | Status  |                  Info                  |\n"
           "----------------------------------------------------------------------------------------------------------------------------------\n");
}

static void draw_key_table_head(void)
{
    printf("------------------------------\n"
           "| Index |     Surname        |\n"
           "------------------------------\n");
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

static void draw_key_table_element(const person_key_t person)
{
    printf("| %-5.5ld | %-18.18s |\n"
               "------------------------------\n",
               person.index, person.key);
}

void draw_table(person_array_t person_array_t)
{
    draw_table_head();
    for (size_t i = 0; i < person_array_t.size; i++)
        draw_table_element(person_array_t.array[i]);
}

void draw_key_table(person_key_array_t person_key_array)
{
    draw_key_table_head();
    for (size_t i = 0; i < person_key_array.size; i++)
        draw_key_table_element(person_key_array.array[i]);
}

static int compare_person(person_t p1, person_t p2)
{
    return strcmp(p1.surname, p2.surname);
}

static void swap_person(person_t *p1, person_t *p2)
{
    person_t tmp;
    tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

static void swap_keys(person_key_t *p1, person_key_t *p2)
{
    person_key_t tmp;
    tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

static int compare_keys(person_key_t p1, person_key_t p2)
{
    return strcmp(p1.key, p2.key);
}

void sort_table_by_surname_with_bubblesort(person_array_t *person_array)
{
    int res = 0;
    for (size_t i = 0; i < person_array->size - 1; i++)
    {
        for (size_t j = 0; j < person_array->size - 1; j++)
        {
            res = compare_person(person_array->array[j], person_array->array[j + 1]);
            if (res > 0)
                swap_person(&person_array->array[j], &person_array->array[j + 1]);
        }
    }
}

int add_person_with_terminal(person_array_t *person_array)
{
    read_person(stdin, &person_array->array[person_array->size++], 't');
}

int add_person_from_file(FILE *f, person_array_t *person_array)
{
    int rc;
    if (person_array->size < MAX_ARRAY_LEN)
    {
        if (rc = read_person(f, &person_array->array[person_array->size], 'f'))
            return rc;
        person_array->size++;
        return SUCCESS_EXIT;
    }
    else
        return ARR_OVERFLOW_ERROR;
}

int update_database(file_t file, person_array_t *person_array)
{
    int rc = 0;
    FILE *f = fopen(file, "r");
    person_array->size = 0;

    while (!rc)
    {
        rc = add_person_from_file(f, person_array);
    }

    if (feof(f))
        rc = SUCCESS_EXIT;

    fclose(f);
    return rc;
}

static int write_person_in_file(FILE *f, person_t person)
{
    fprintf(f, "%s\n", person.surname);
    fprintf(f, "%s\n", person.name);
    fprintf(f, "%s\n", person.phoneNumber);
    fprintf(f, "%s\n", person.adress.street);
    fprintf(f, "%d\n", person.adress.houseNumber);
    fprintf(f, "%s\n", person.status);
    if (!strcmp(person.status, "Друг"))
    {
        fprintf(f, "%d\n", person.info.birthday.day);
        fprintf(f, "%d\n", person.info.birthday.month);
        fprintf(f, "%d\n", person.info.birthday.year);
    }
    else
    {
        fprintf(f, "%s\n", person.info.work.position);
        fprintf(f, "%s\n", person.info.work.organization);
    }

    return SUCCESS_EXIT;
}

int update_file(file_t file, person_array_t person_array)
{
    FILE *f = fopen(file, "w");
    for (size_t i = 0; i < person_array.size; i++)
        write_person_in_file(f, person_array.array[i]);
    return SUCCESS_EXIT;
}

static void create_key(person_key_t *person_key, const string_t sn, size_t index)
{
    person_key->index = index;
    strcpy(person_key->key, sn);
}

void create_key_table(person_key_array_t *person_key_array, const person_array_t person_array)
{
    person_key_array->size = 0;
    for (size_t i = 0; i < person_array.size; i++)
    {
        create_key(&(person_key_array->array[i]), person_array.array[i].surname, i);
        person_key_array->size++;
    }
}

// при изменении исходной таблицы таблица ключей сбрасывается

void sort_key_table_with_bubblesort(person_key_array_t *person_key_array, size_t size)
{
    int res = 0;
    for (size_t i = 0; i < size - 1; i++)
    {
        for (size_t j = 0; j < size - 1; j++)
        {
            res = compare_keys(person_key_array->array[j], person_key_array->array[j + 1]);
            if (res > 0)
                swap_keys(&(person_key_array->array[j]), &(person_key_array->array[j + 1]));
        }
    }
}

int delete_person_by_surname(person_array_t *person_array)
{
    string_t surname;
    puts("Введите фамилию абонента, которого требуется удалить:");
    fgets(surname, sizeof(string_t), stdin);

        if (surname[strlen(surname) - 1] == '\n')
            surname[strlen(surname) - 1] = '\0';

    int indices[MAX_ARRAY_LEN];
    int index = -1;
    size_t count = 0;

    for (size_t i = 0; i < person_array->size; i++)
    {
        if (!strcmp(person_array->array[i].surname, surname))
        {
            indices[count++] = i;
        }
    }
    if (count > 1)
    {
        string_t name;
        puts("Найдено несколько подходящих абонентов. Уточните, пожалуйста, имя абонента:");
        fgets(name, sizeof(string_t), stdin);

        if (name[strlen(name) - 1] == '\n')
            name[strlen(name) - 1] = '\0';

        for (size_t i = 0; i < count; i++)
        {
            if (!strcmp(person_array->array[indices[i]].name, name))
                index = i;
        }
    }
    else if (count == 1)
        index = indices[0];
    else
        return DELETE_ERROR;

    person_array->size--;
    for (size_t i = index; i < person_array->size; i++)
        person_array->array[i] = person_array->array[i + 1];
    
    return index;
}