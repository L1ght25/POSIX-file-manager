#ifndef _MAIN_FUNCS_H
#define _MAIN_FUNCS_H 1

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>

void init_screen();
void print_text(int x, int y, const char* text);
void another_print_text(int x, int y, const char* text);
void print_color(char *str, int fg_color, int bg_color);
void shutdown_screen();
void clear_screen();
void simple_print(char *text);

enum {
    FG_BLACK = 30, FG_RED, FG_GREEN, FG_YELLOW, FG_BLUE,
    FG_MAGENTA, FG_CYAN, FG_WHITE
};

enum {
    BG_BLACK = 40, BG_RED, BG_GREEN, BG_YELLOW, BG_BLUE,
    BG_MAGENTA, BG_CYAN, BG_WHITE
};

#endif
