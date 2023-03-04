#include "print_funcs.h"
#include "no_ncurses_lib/main_funcs.h"
#include "utils.h"
#include <stdio.h>


void print_directories(int begin_str, MetaFile *files, int size_of_files, int curr_dir) {
    int width_of_cols = -N_COLS / 3;
    printf("%*s%*s%s\n", width_of_cols, "File name", width_of_cols, "Size of file", "Last modified time");
    int cur_color = 0;
    char buf[MAX_PRINT_BUF_SIZE];
    int end_of_window = MIN(size_of_files, begin_str + N_ROWS - 1);
    for (size_t i = begin_str; i < end_of_window; ++i) {
        cur_color = 0;
        if (files[i].filetype == DT_DIR) {
            if (files[i].size == -1) {
                if (curr_dir == i) {
                    cur_color = SELECTED_FILE_COLOR;
                }
                print_color("..\n", FG_BLACK + (cur_color == SELECTED_FILE_COLOR ? SELECTED_FILE_COLOR : 1), BG_BLACK + cur_color);
                continue;
            }
            if (cur_color != SELECTED_FILE_COLOR) {
                cur_color = DIR_COLOR;
            }
        } else if (files[i].filetype == DT_LNK) {
            cur_color = LINK_COLOR;
        } else if (files[i].filetype == DT_FIFO) {
            cur_color = FIFO_COLOR;
        }
        if (curr_dir == i) {
            cur_color = SELECTED_FILE_COLOR;
        }
        char *time = ctime(&files[i].modified_time);
        snprintf(buf, MAX_PRINT_BUF_SIZE, "%*s%*d%s", width_of_cols, files[i].name, width_of_cols, files[i].size, time);
        print_color(buf, FG_BLACK + (cur_color == 0 ? 7 : cur_color), BG_BLACK + (cur_color == SELECTED_FILE_COLOR ? SELECTED_FILE_COLOR : 0));
    }
}
