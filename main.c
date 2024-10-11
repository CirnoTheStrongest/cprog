#include <stdlib.h>
#include "interface.h"

int main(void)
{   
    int rc;
    int action;

    person_array_t person_array;
    person_array.size = 0;
    person_key_array_t person_key_array;
    person_key_array.size = 0;

    file_t filename;
    file_t output;

    print_info();

    action = input_start_action();

    if (rc = do_start_action(action, filename))
    {
        if (rc == EXIT_CODE)
            return EXIT_SUCCESS;
        return rc;
    }
    puts("");

    while (1)
    {
        action = input_action();

        if (rc = do_action(action, filename, &person_array, &person_key_array, output))
        {
            if (rc == EXIT_CODE)
                return EXIT_SUCCESS;
            return rc;
        }
        puts("");
    }

    return EXIT_SUCCESS;
}