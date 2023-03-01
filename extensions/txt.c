#include <unistd.h>


int open_file(char *path) {
    char *args[] = {"nvim", path, NULL};
    execvp(args[0], args);
    return 1;
}