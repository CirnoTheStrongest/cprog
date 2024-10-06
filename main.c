#include "person.h"
#include "table.h"

int main(void)
{
    FILE *f = fopen("table.txt", "r");
    person_array_t person_array;
    person_array.size = 0;
    read_person(f, &person_array.array[person_array.size++]);
    read_person(f, &person_array.array[person_array.size++]);
    draw_table(person_array);
}