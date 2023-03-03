#include "initialize.h"
#include <curses.h>
#include "utils.h"

WINDOW *initialize() {
    initscr();
    curs_set(0);
    start_color();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    WINDOW *win = newwin(N_ROWS + 1, N_COLS, 0, 0);
    keypad(win, TRUE);
    scrollok(win, TRUE);
    idlok(win, TRUE);
    return win;
}