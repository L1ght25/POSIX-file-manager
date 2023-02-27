#ifndef _UTILS_H
#define _UTILS_H 1

#include <fcntl.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define MIN(first_arg, second_arg) ((first_arg) > (second_arg) ? (second_arg) : (first_arg))
#define MAX(first_arg, second_arg) ((first_arg) > (second_arg) ? (first_arg) : (second_arg))

enum { N_COLS = 80, N_ROWS = 24, MAX_FILES_IN_DIR = 100, MAX_BUF_SIZE = 8192 };

enum CUT_STATUS {
    WAS_NOT_SELECTED,
    TO_MOVE,
    TO_COPY
};

typedef struct {
    const char *name;
    long modified_time;
    int size;
    unsigned char filetype;
} MetaFile;

int comparator_of_files(const void *first, const void *second);

int move_files(const char *src, const char *dest_path, const char *dest_name);

void *get_extensions_handler(const char *path_to_lib, const char *name_of_prog, void *lib);

#endif
