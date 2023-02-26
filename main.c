#include <curses.h>
#include <linux/limits.h>
#include <unistd.h>
#include "utils.h"
#include "directories_handler.h"


int main() {
    initscr();
    WINDOW *win = newwin(N_ROWS + 1, N_COLS, 0, 0);
    // WINDOW *win = stdscr;
    keypad(win, TRUE);
    scrollok(win, TRUE);
    idlok(win, TRUE);
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("invalid directory");
    }
    directory_handler(win, cwd);
    delwin(win);
    endwin();
    return 0;
}