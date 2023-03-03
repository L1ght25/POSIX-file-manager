#include "print_funcs.h"
#include "utils.h"
#include <ncurses.h>


void init_with_color_zero(int color_back, int color_words) {  // deprecated
    start_color();
    init_pair(0, color_words, color_back);
}

void print_directories(WINDOW *win, int begin_str, MetaFile *files, int size_of_files, int curr_dir) {
    int width_of_cols = -N_COLS / 3;
    wprintw(win, "%*s%*s%s\n", width_of_cols, "File name", width_of_cols, "Size of file", "Last modified time");
    int cur_color = 0;
    int end_of_window = MIN(size_of_files, begin_str + N_ROWS - 1);
    for (size_t i = begin_str; i < end_of_window; ++i) {
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
        PRINT_WITH_COLOR(win, cur_color, "%*s%*d%s", width_of_cols, files[i].name, width_of_cols, files[i].size, time);
    }
}
