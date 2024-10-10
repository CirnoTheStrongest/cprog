#ifndef TABLE_H
#define TABLE_H

#include "person.h"
#include "file.h"

void draw_table(person_array_t person_arr);
void sort_table_by_surname_with_bubblesort(person_array_t *person_arr);
int add_person_with_terminal(person_array_t *person_array);
int update_file(file_t file, person_array_t person_array);
int update_database(file_t file, person_array_t *person_array);
int delete_person_by_surname(person_array_t *person_array);
void create_key_table(person_key_array_t *person_key_array, const person_array_t person_array);
void draw_key_table(person_key_array_t person_key_array);
void sort_key_table_with_bubblesort(person_key_array_t *person_key_array);
void draw_table_by_key_table(person_array_t *person_array, person_key_array_t *person_key_array);
void draw_nearest_birthdays(person_array_t *person_array);
void test_sort_time(person_array_t *person_array, person_key_array_t *person_key_array, file_t filename, char mode, file_t output);
void fill_random(file_t filename, int n, char mode);

#endif