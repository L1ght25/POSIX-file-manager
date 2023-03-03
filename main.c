#include <curses.h>
#include <linux/limits.h>
#include <unistd.h>
#include "directories_handler.h"
#include "initialize.h"
#include "no_ncurses_lib/main_funcs.h"


int main() {
    struct termios oldt = initialize();  // to raw terminal and save state
    init_screen();
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("invalid directory");
    }
    directory_handler(cwd);
    clear_screen();
    unitialize(oldt);  // to usual terminal
    return 0;
}
