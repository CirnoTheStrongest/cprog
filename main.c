#include <stdlib.h>
#include "interface.h"

int main(void)
{   
    int rc;
    person_array_t person_array;
    person_key_array_t person_key_array;
    file_t filename;
    file_t output;
    int action;

    action = input_start_action();

    if (rc = do_start_action(action, filename))
    {
        if (rc == EXIT_CODE)
            return 0;
        return rc;
    }

    while (1)
    {
        action = input_action();

        if (rc = do_action(action, filename, &person_array, &person_key_array, output))
        {
            if (rc == EXIT_CODE)
                return 0;
            return rc;
        }
    }

    return 0;
}