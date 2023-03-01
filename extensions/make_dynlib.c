#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

enum {
    MAX_LEN_OF_COMMAND = 256
};

int main(int argc, char *argv[]) {  // util program to make .so library by file
    char command[MAX_LEN_OF_COMMAND];
    for (size_t i = 1; i < argc; ++i) {
        snprintf(command, MAX_LEN_OF_COMMAND, "gcc -c -fPIC %s.c -o tmp.o && gcc tmp.o -shared -o %s.so && rm tmp.o", argv[i], argv[i]);
        printf("current command: %s\n", command);
        system(command);
    }
}
