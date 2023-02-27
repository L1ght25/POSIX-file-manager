#include "extensions.h"
#include <fcntl.h>
#include <unistd.h>

void run_program(char *command, char *file_path) {
    pid_t pid = fork();
    if (!pid) {
        char *args[] = {command, file_path, NULL};
        execvp(args[0], args);
    } else {
        wait(NULL);
    }
}
