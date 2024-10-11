#ifndef INTERFACE_H
#define INTERFACE_H

#include "file.h"
#include "person.h"
#include "table.h"

#define SUCCESS_CODE 0
#define EXIT_CODE 400
#define INCORRECT_ACTION_ERROR 401

int input_start_action(void);
int input_action(void);
int do_start_action(int action, file_t filename);
int do_action(int action, file_t filename, person_array_t *person_array, person_key_array_t *person_key_array, file_t output);
void print_info(void);

#endif