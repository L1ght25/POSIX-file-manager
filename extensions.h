#ifndef _EXTENSIONS_H
#define _EXTENSIONS_H 1

#include <sys/wait.h>
#include <dlfcn.h>
#include <stdio.h>

typedef int (*open_file_t)(char* path);

open_file_t get_file_func(const char *path_to_lib, void **lib);

int run_program(open_file_t file_func, char *path_to_file);

void *get_extensions_handler(const char *path_to_lib, const char *name_of_prog, void **lib);

#endif