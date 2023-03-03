#include <curses.h>
#include <linux/limits.h>
#include <ncurses.h>
#include <unistd.h>
#include "directories_handler.h"
#include "initialize.h"


int main() {
    WINDOW *win = initialize();
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("invalid directory");
    }
    directory_handler(win, cwd);
    delwin(win);
    endwin();
    return 0;
}