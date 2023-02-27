#include <stdlib.h>
#include <string.h>

char* get_program_command(const char* extension) {
    if (!strcmp(extension, ".txt")) {
        return "nvim";
    } else if (!strcmp(extension, ".gz")) {
        return "gunzip";
    } else {
        return NULL;
    }
}