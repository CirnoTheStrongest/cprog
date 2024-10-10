#include "interface.h"
#include "file.h"
#include "person.h"
#include "table.h"
#include "tools.h"

#define SUCCESS_CODE 0
#define EXIT_CODE 400
#define INCORRECT_ACTION_ERROR 401

static void show_start_actions(void)
{
    puts("Доступные действия:");
    puts("1. Инициализировать файл.");
    puts("2. Выйти из программы.");
}

int input_start_action(void)
{
    string_t buf;
    int action;
    show_start_actions();
    puts("Введите требуемое действие:");
    if (!fgets(buf, sizeof(string_t), stdin))
        return INCORRECT_ACTION_ERROR;

    if (str_to_int(buf, &action))
        return INCORRECT_ACTION_ERROR;
    return action;
}

static void show_actions(void)
{
    puts("Доступные действия:");
    puts("1. Прочитать файл в базу данных.");
    puts("2. Записать в файл базу данных.");
    puts("3. Вывести базу данных.");
    puts("4. Вывести таблицу ключей.");
    puts("5. Добавить запись в базу данных.");
    puts("6. Удалить запись из базы данных.");
    puts("7. Отсортировать базу данных по фамилии абонента.");
    puts("8. Вывести базу данных по таблице ключей.");
    puts("9. Отсортировать таблицу ключей.");
    puts("10. Вывести список друзей, которых надо поздравить с Днём Рождения.");
    puts("11. Исследование времени работы сортировок.");
    puts("12. Заполнить таблицу случайными числами.");
    puts("13. Выйти из программы.");
}

int input_action(void)
{
    string_t buf;
    int action;
    show_actions();
    puts("Введите требуемое действие:");
    if (!fgets(buf, sizeof(string_t), stdin))
        return INCORRECT_ACTION_ERROR;

    if (str_to_int(buf, &action))
        return INCORRECT_ACTION_ERROR;
    
    return action;
}

int do_start_action(int action, file_t filename)
{
    switch (action)
    {
    case 1:
        init_file(filename);
        break;
    case 2:
        return EXIT_CODE;
    default:
        return INCORRECT_ACTION_ERROR;
    }
    return SUCCESS_CODE;
}

int do_action(int action, file_t filename, person_array_t *person_array, person_key_array_t *person_key_array, file_t output)
{
    switch (action)
    {
    case 1:
        update_database(filename, person_array);
        create_key_table(person_key_array, *person_array);
        break;
    case 2:
        update_file(filename, *person_array);
        break;
    case 3:
        draw_table(*person_array);
        break;
    case 4:
        draw_key_table(*person_key_array);
        break;
    case 5:
        add_person_with_terminal(person_array);
        create_key_table(person_key_array, *person_array);
        break;
    case 6:
        delete_person_by_surname(person_array);
        create_key_table(person_key_array, *person_array);
        break;
    case 7:
        sort_table_by_surname_with_bubblesort(person_array);
        create_key_table(person_key_array, *person_array);
        break;
    case 8:
        draw_table_by_key_table(person_array, person_key_array);
        break;
    case 9:
        sort_key_table_with_bubblesort(person_key_array);
        break;
    case 10:
        draw_nearest_birthdays(person_array);
        break;
    case 11:
        init_file(output);
        test_sort_time(person_array, person_key_array, filename, 'f', output);
        break;
    case 12:
        fill_random(filename, 0, 't');
        break;
    case 13:
        puts("Завершение программы...");
        return EXIT_CODE;
    default:
        return INCORRECT_ACTION_ERROR;
    }
    return SUCCESS_CODE;
}