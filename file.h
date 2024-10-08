#ifndef FILE_H
#define FILE_H

#define MAX_FILENAME_LEN 100

typedef char file_t[MAX_FILENAME_LEN + 1];
int init_file(file_t file);


#endif