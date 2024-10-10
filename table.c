#include <string.h>
#include <stdlib.h>
#include <time.h>
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
    if (!strcmp(person.status, "Friend"))
        printf("| %-18.18s | %-18.18s | %-12.12s | %-13.13s %4d |  Friend | Date of Birth: %02d.%02d.%d              |\n"
               "----------------------------------------------------------------------------------------------------------------------------------\n",
               person.surname, person.name, person.phoneNumber, person.adress.street, person.adress.houseNumber, person.info.birthday.day,
               person.info.birthday.month, person.info.birthday.year);
    else
        printf("| %-18.18s | %-18.18s | %-12.12s | %-13.13s %4d | Partner | Position: %-28.28s |\n"
               "|                    |                    |              |                    |         | Organization: %-24.24s |\n"
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

void draw_table(person_array_t person_array)
{
    draw_table_head();
    for (size_t i = 0; i < person_array.size; i++)
        draw_table_element(person_array.array[i]);
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
    if (!strcmp(person.status, "Friend"))
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

void sort_key_table_with_bubblesort(person_key_array_t *person_key_array)
{
    int res = 0;
    for (size_t i = 0; i < person_key_array->size - 1; i++)
    {
        for (size_t j = 0; j < person_key_array->size- 1; j++)
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

void draw_table_by_key_table(person_array_t *person_array, person_key_array_t *person_key_array)
{
    draw_table_head();
    for (size_t i = 0; i < person_array->size; i++)
        draw_table_element(person_array->array[person_key_array->array[i].index]);
}

int input_date(date_t *date)
{
    puts("Введите число, месяц и год через Enter:");
    return read_date(stdin, date);
}

/*
Функция считывает текущую дату D. Далее производится поиск друзей, у которых День Рождения приходится на D, ..., D + 6 дни.
*/
void draw_nearest_birthdays(person_array_t *person_array)
{
    date_t date;
    size_t count = 0;
    input_date(&date);

    for (size_t i = 0; i < person_array->size; i++)
    {
        if (!strcmp(person_array->array[i].status, "Friend"))
        {
            date_t birthday = 
            {
            .day = person_array->array[i].info.birthday.day,
            .month = person_array->array[i].info.birthday.month,
            .year = person_array->array[i].info.birthday.year
            };

            if (is_date_this_week(birthday, date) && (date.year >= birthday.year))
            {
                if (count == 0)
                    draw_table_head();
                draw_table_element(person_array->array[i]);
                count++;
            }
        }
    }

    if (count == 0)
        puts("Искомых друзей не найдено.");
}

static int qcompare_persons(const void* p1, const void* p2)
{
    return strcmp(((person_t *) p1)->surname, ((person_t *) p2)->surname);
}

static int qcompare_keys(const void* k1, const void* k2)
{
    return strcmp(((person_key_t *) k1)->key, ((person_key_t *) k2)->key);
}

static void sort_table_by_surname_with_quicksort(person_array_t *person_array)
{
    qsort(&(person_array->array[0]), person_array->size, sizeof(person_t), qcompare_persons);
}

static void sort_key_table_with_quicksort(person_key_array_t *person_key_array)
{
    qsort(&(person_key_array->array[0]), person_key_array->size, sizeof(person_key_t), qcompare_keys);
}

/*
Функция заполняет файл n случайными записями.
rand() % длина_диапазона + сдвиг
длина_диапазона вычисляется как b - a + 1
*/
void fill_random(file_t filename, int n, char mode)
{
    string_t buf;
    
    if (mode == 't')
    {
        puts("Введите количество записей:");
        fgets(buf, sizeof(string_t), stdin);
        str_to_int(buf, &n);
    }


    FILE *f = fopen(filename, "w");

    for (size_t i = 0; i < n; i++)
    {
        person_t random_person;

        // для surname
        size_t size = rand() % MAX_STR_LEN + 1;

        for (size_t j = 0; j < size; j++)
        {
            char ch = rand() % ('z' - 'A' + 1) + 'A';
            buf[j] = ch;
        }
        buf[size] = '\0';
        strcpy(random_person.surname, buf);

        // для name
        size = rand() % MAX_STR_LEN + 1;

        for (size_t j = 0; j < size; j++)
        {
            char ch = rand() % ('z' - 'A' + 1) + 'A';
            buf[j] = ch;
        }
        buf[size] = '\0';
        strcpy(random_person.name, buf);

        // для phoneNumber
        buf[0] = '+';
        buf[1] = '7';
        for (size_t j = 2; j < 12; j++)
        {
            char ch = rand() % ('9' - '0' + 1) + '0';
            buf[j] = ch;
        }
        buf[12] = '\0';
        strcpy(random_person.phoneNumber, buf);

        // для address
        size = rand() % MAX_STR_LEN + 1;

        for (size_t j = 0; j < size; j++)
        {
            char ch = rand() % ('z' - 'A' + 1) + 'A';
            buf[j] = ch;
        }
        buf[size] = '\0';
        strcpy(random_person.adress.street, buf);

        random_person.adress.houseNumber = rand() % 1000 + 1;

        // для status + info
        int bit = rand() % 2;

        if (bit)
        {
            strcpy(random_person.status, "Friend");

            random_person.info.birthday.day = rand() % 28 + 1;
            random_person.info.birthday.month = rand() % 12 + 1;
            random_person.info.birthday.year = rand() % 100 + 1921;
        }
        else
        {
            strcpy(random_person.status, "Partner");

            // для position
            size = rand() % MAX_STR_LEN + 1;

            for (size_t j = 0; j < size; j++)
            {
                char ch = rand() % ('z' - 'A' + 1) + 'A';
                buf[j] = ch;
            }
            buf[size] = '\0';
            strcpy(random_person.info.work.position, buf);

            // для organization
            size = rand() % MAX_STR_LEN + 1;

            for (size_t j = 0; j < size; j++)
            {
                char ch = rand() % ('z' - 'A' + 1) + 'A';
                buf[j] = ch;
            }
            buf[size] = '\0';
            strcpy(random_person.info.work.organization, buf);
        }

        write_person_in_file(f, random_person);
    }

    fclose(f);
}

void draw_test_head(void)
{
    printf("---------------------------------------------------------------\n"
           "|  size   | bubblesort | quicksort  | key_bsort  | key_qsort  |\n"
           "---------------------------------------------------------------\n");
}

void draw_test_result(size_t n, time_t result1, time_t result2, time_t result3, time_t result4)
{
    printf("| %7ld | %10ld | %10ld | %10ld | %10ld |\n"
           "---------------------------------------------------------------\n",
           n, result1, result2, result3, result4);
}

void test_sort_time(person_array_t *person_array, person_key_array_t *person_key_array, file_t filename, char mode, file_t output)
{
    clock_t time_s;
    clock_t time_e;
    time_t result1;
    time_t result2;
    time_t result3;
    time_t result4;

    FILE *f = fopen(output, "w");

    puts("Исследование времени сортировки.");
    draw_test_head();

    for (size_t i = 10; i < 10000; i += 100)
    {
        if (mode == 'f')
            fprintf(f, "%ld ", i);

        // сортировка пузырьком
        result1 = 0;
        for (size_t j = 0; j < 10; j++)
        {
            fill_random(filename, i, 'f');
            update_database(filename, person_array);
            
            time_s = clock();
            sort_table_by_surname_with_bubblesort(person_array);
            time_e = clock();

            result1 += (time_e - time_s) / (CLOCKS_PER_SEC / 1000000);
        }

        result1 /= 10;

        if (mode == 'f')
            fprintf(f, "%ld ", result1);

        // быстрая сортировка
        result2 = 0;
        for (size_t j = 0; j < 10; j++)
        {
            fill_random(filename, i, 'f');
            update_database(filename, person_array);

            time_s = clock();
            sort_table_by_surname_with_quicksort(person_array);
            time_e = clock();

            result2 += (time_e - time_s) / (CLOCKS_PER_SEC / 1000000);
        }

        result2 /= 10;

        if (mode == 'f')
            fprintf(f, "%ld ", result2);

        // сортировка таблицы ключей пузырьком
        result3 = 0;
        for (size_t j = 0; j < 10; j++)
        {
            fill_random(filename, i, 'f');
            update_database(filename, person_array);
            create_key_table(person_key_array, *person_array);

            time_s = clock();
            sort_key_table_with_bubblesort(person_key_array);
            time_e = clock();

            result3 += (time_e - time_s) / (CLOCKS_PER_SEC / 1000000);
        }

        result3 /= 10;

        if (mode == 'f')
            fprintf(f, "%ld ", result3);

        // сортировка таблицы ключей быстрой сортировкой
        result4 = 0;
        for (size_t j = 0; j < 10; j++)
        {
            fill_random(filename, i, 'f');
            update_database(filename, person_array);
            create_key_table(person_key_array, *person_array);

            time_s = clock();
            sort_key_table_with_quicksort(person_key_array);
            time_e = clock();

            result4 += (time_e - time_s) / (CLOCKS_PER_SEC / 1000000);
        }

        result4 /= 10;

        if (mode == 'f')
            fprintf(f, "%ld\n", result4);
        
        draw_test_result(i, result1, result2, result3, result4);
    }

    fclose(f);
}
