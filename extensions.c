#include "extensions.h"
#include "utils.h"
#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>


void *get_extensions_handler(const char *path_to_lib, const char *name_of_prog, void **lib) {
    void *lib_handler = dlopen(path_to_lib, RTLD_LAZY);
    if (!lib_handler) {
        return NULL;
    }

    void *program = dlsym(lib_handler, name_of_prog);
    if (!program) {
        dlclose(lib_handler);
        return NULL;
    }
    *lib = lib_handler;
    return program;
}


open_file_t get_file_func(const char *path_to_lib, void **lib) {
    return (open_file_t)get_extensions_handler(path_to_lib, "open_file", lib);
}


int run_program(open_file_t file_func, char *path_to_file) {
    pid_t pid = fork();
    if (!pid) {
        file_func(path_to_file);
        return 1;
    } else {
        wait(NULL);
    }
    return 0;
}
