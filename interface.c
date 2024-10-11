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
    puts("12. Заполнить таблицу случайными записями.");
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
        return init_file(filename);
    case 2:
        return EXIT_CODE;
    default:
        puts("Некорректный номер действия!");
        action = input_start_action();
        return do_start_action(action, filename);
    }
    return SUCCESS_CODE;
}

int do_action(int action, file_t filename, person_array_t *person_array, person_key_array_t *person_key_array, file_t output)
{
    int rc;
    switch (action)
    {
    case 1:
        if (rc = update_database(filename, person_array))
            return rc;
        create_key_table(person_key_array, *person_array);
        break;
    case 2:
        return update_file(filename, *person_array);
    case 3:
        draw_table(*person_array);
        break;
    case 4:
        draw_key_table(*person_key_array);
        break;
    case 5:
        if (rc = add_person_with_terminal(person_array))
            return rc;
        create_key_table(person_key_array, *person_array);
        break;
    case 6:
        if (rc = delete_person_by_surname(person_array))
            return rc;
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
        if (rc = init_file(output))
            return rc;
        test_sort_time(person_array, person_key_array, filename, 'f', output);
        break;
    case 12:
        fill_random(filename, 0, 't');
        break;
    case 13:
        puts("Завершение программы...");
        return EXIT_CODE;
    default:
        puts("Некорректный номер действия!");
        action = input_action();
        return do_action(action, filename, person_array, person_key_array, output);
    }
    return SUCCESS_CODE;
}

void print_info(void)
{
    puts("Лабораторная работа №2 по предмету\n"
         "Типы и структуры данных. Вариант 7.\n"
         "Разработал: Коротков Б. О.\n"
         "студент группы ИУ7-34Б\n"
         "МГТУ им. Н. Э. Баумана\n\n"
         "Программа выполняет различные действия с массивом структур,\n"
         "представленном в виде таблицы. Данные считываются с файла и изменяются\n"
         "впоследствии пользователем. Основная задача программы — исследование\n"
         "времени работы сортировок при работе с исходной таблицей и таблицей ключей.\n"
         "Результат исследования записывается в файл, указанный пользователем.\n\n"
         "Используемые алгоритмы сортировок:\n"
         "1. Пузырьковая сортировка.\n"
         "2. Быстрая сортировка (qsort).\n\n"
         "Таблица с записями — информацией об абонентах, содержит:\n"
         "- фамилию абонента;\n"
         "- имя абонента;\n"
         "- номер телефона абонента;\n"
         "- улицу и номер дома абонента;\n"
         "- статус абонента;\n"
         "- день рождения или должность и место работы в зависимости от статуса абонента.\n\n"
         "Ограничения при вводе:\n"
         "- при выборе действия должно вводится целое число в диапазоне от 1 до 2 при начальном выборе\n"
         "действий и от 1 до 13 при основном;\n"
         "- имя файла не должно превышать 100 символов в длину;\n"
         "- строковые данные в программе должны не превышать 50 символов в длину,\n"
         "состоять только из латинских букв. Исключение — номер телефона абонента.\n"
         "Он должен состоять из 12 символов и иметь вид „+7**********“, где * - цифра от 0 до 9.\n"
         "При вводе статуса абонента есть только 2 корректных варианта — „Friend“ и „Partner“;\n"
         "- числовые данные должны быть в диапазоне от 0 до 2147483647,\n"
         "не содержать знаков „+“ и „-“. Однако при вводе адреса диапазон номера дома составляет\n"
         "от 1 до 1000, при вводе дня рождения введённые числовые данные в совокупности должны образовывать\n"
         "корректную дату с учетом високосного года, год рождения не должен быть меньше 1920 года и не больше 2100 года;\n"
         "- максимальный размер таблицы не должен превышать 10240 записей.\n\n"
         "ВНИМАНИЕ: при выборе действия, которое должно изменить базу данных, автоматически обновляется таблица ключей.\n"
         "Проведение исследования эффективности сортировок занимает много времени (ок. 10 минут).\n\n");
}
