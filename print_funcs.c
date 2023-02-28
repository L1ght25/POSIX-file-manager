#include "print_funcs.h"


void init_with_color_zero(int color_back, int color_words) {  // deprecated
    start_color();
    init_pair(0, color_words, color_back);
}

void print_directories(WINDOW *win, int begin_str, MetaFile *files, int size_of_files, int curr_dir) {
    wprintw(win, "%-20s %-20s %-20s\n", "File name", "Size of file", "Last modified time");
    start_color();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    int cur_color = 0;
    int end_of_window = MIN(size_of_files, begin_str + N_ROWS - 1);
    for (int i = begin_str; i < end_of_window; ++i) {
        cur_color = 0;
        if (files[i].filetype == DT_DIR) {
            if (files[i].size == -1) {
                if (curr_dir == i) {
                    cur_color = 2;
                }
                PRINT_WITH_COLOR(win, cur_color, "..\n");
                continue;
            }
            if (cur_color != 2) {
                cur_color = 1;
            }
        } else if (files[i].filetype == DT_LNK) {
            cur_color = 3;
        } else if (files[i].filetype == DT_FIFO) {
            cur_color = 4;
        }
        if (curr_dir == i) {
            cur_color = 2;
        }
        char *time = ctime(&files[i].modified_time);
        PRINT_WITH_COLOR(win, cur_color, "%-20s %-20d %-20s", files[i].name, files[i].size, time);
    }
    wmove(win, MIN(curr_dir + 1 - begin_str, N_ROWS - 1), 0);
}