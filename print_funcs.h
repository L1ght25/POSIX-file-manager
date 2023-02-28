#include <curses.h>
#include <ncurses.h>
#include <time.h>
#include <dirent.h>
#include "utils.h"

#define PRINT_WITH_COLOR(win, color, fmt, ...) wattron(win, COLOR_PAIR(color));   \
wprintw(win, fmt, ##__VA_ARGS__);                                                 \
wattroff(win, COLOR_PAIR(color))                                                  \


void print_directories(WINDOW *win, int begin_str, MetaFile *files, int size_of_files, int curr_dir);

void init_with_color_zero(int color_back, int color_words);
