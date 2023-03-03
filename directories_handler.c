#include <curses.h>
#include <dlfcn.h>
#include <ncurses.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>
#include "utils.h"
#include "directories_handler.h"
#include "extensions.h"
#include "print_funcs.h"

int get_files_in_path(const char *curr_path, MetaFile *files, size_t *size, bool all_files) {
    DIR *dir = opendir(curr_path);
    if (!dir) {
        return -1;
    }

    char tmp_path[PATH_MAX];
    size_t i = 0;

    struct stat statbuf;
    struct dirent *curr_dir;

    while ((curr_dir = readdir(dir)) != NULL) {
        if (!strcmp(curr_dir->d_name, "..")) {
            files[i++] = (MetaFile){.name = "..", .modified_time = -1, .size = -1, .filetype = DT_DIR};
            continue;
        }
        if (curr_dir->d_name[0] == '.' && !all_files) {
            continue;
        }
        snprintf(tmp_path, PATH_MAX, "%s/%s", curr_path, curr_dir->d_name);
        if (stat(tmp_path, &statbuf) == -1) {
            files[i++] = (MetaFile){.modified_time = -1, .size = -1, .filetype = curr_dir->d_type};
        } else {
            files[i++] = (MetaFile){.modified_time = statbuf.st_mtime, .size = statbuf.st_size, .filetype = curr_dir->d_type};
        }
        strcpy(files[i - 1].name, curr_dir->d_name);
        if (i >= MAX_FILES_IN_DIR) {
            break;
        }
    }
    qsort(files, i, sizeof(*files), comparator_of_files);
    *size = i;
    closedir(dir);
    return 0;
}

void directory_handler(WINDOW *win, const char *input_path) {
    void *lib;
    DIR *dir = opendir(input_path);
    if (!dir) {
        return;
    }

    int selected_dir = 0;
    int begin_str = 0;
    int move_status = WAS_NOT_SELECTED;
    bool all_files = false;

    char current_path[PATH_MAX];
    strcpy(current_path, input_path);
    char tmp_path[PATH_MAX];
    char tmp_path1[PATH_MAX];
    char moved_path[PATH_MAX];
    char *extensions_path = "../extensions";
    MetaFile files[MAX_FILES_IN_DIR];

    size_t count_of_files = 0;
    
    int err = get_files_in_path(input_path, files, &count_of_files, all_files);
    if (err) {
        exit(1);
    }
    
    print_directories(win, begin_str, files, count_of_files, selected_dir);
    while (1) {
        int symb = wgetch(win);
        werase(win);
        switch (symb) {
            case KEY_DOWN:
                if (selected_dir < count_of_files - 1) {
                    ++selected_dir;
                    if (selected_dir > begin_str + N_ROWS - 2) {
                        ++begin_str;
                    }
                }
                break;
            case KEY_UP:
                if (selected_dir > 0) {
                    --selected_dir;
                    if (selected_dir < begin_str) {
                        begin_str = selected_dir;
                    }
                }
                break;
            case 10:
                snprintf(tmp_path, PATH_MAX, "%s/%s", current_path, files[selected_dir].name);
                if (access(tmp_path, R_OK | W_OK) == -1) {
                    if (files[selected_dir].filetype == DT_LNK) {
                        struct stat buf;
                        if (lstat(tmp_path, &buf) || stat(tmp_path, &buf)) {
                            wprintw(win, "Invalid symlink\n");
                            break;
                        }
                    }
                    wprintw(win, "Sorry, you have not permissions to read this file :(\n");
                    break;
                }
                if (files[selected_dir].filetype == DT_DIR) {
                    snprintf(tmp_path, PATH_MAX, "%s/%s", current_path, files[selected_dir].name);
                    get_files_in_path(tmp_path, files, &count_of_files, all_files);
                    strcpy(current_path, tmp_path);
                    begin_str = 0;
                    selected_dir = 0;
                } else {
                    const char *extension = strrchr(files[selected_dir].name, '.');
                    if (extension != NULL) {
                        snprintf(tmp_path, PATH_MAX, "%s/%s.so", extensions_path, extension + 1);
                        snprintf(tmp_path1, PATH_MAX, "%s/%s", current_path, files[selected_dir].name);
                        open_file_t file_func = get_file_func(tmp_path, &lib);
                        if (!file_func) {
                            wprintw(win, "Sorry, this extension is not supported yet :(\n");
                            break;
                        }
                        endwin();
                        run_program(file_func, tmp_path1);
                        dlclose(lib);
                        initscr();
                    }
                }
                break;
            case 'd':
                if (files[selected_dir].filetype != DT_DIR) {  // we should not delete directories)
                    snprintf(tmp_path, PATH_MAX, "%s/%s", current_path, files[selected_dir].name);
                    wprintw(win, "Are you sure to delete \"%s\"?\n Type \"Y\" if yes or any key to no:", tmp_path);
                    int ans_symb = wgetch(win);
                    wclear(win);
                    if (ans_symb != 'Y') {
                        break;
                    }
                    int err = remove(tmp_path);
                    if (err) {
                        exit(err);
                    }
                    if (!strcmp(moved_path, tmp_path)) {
                        move_status = WAS_NOT_SELECTED;
                    }
                    get_files_in_path(current_path, files, &count_of_files, all_files);
                }
                break;
            case 'x':
            case 'c':  // the last operation has priority to move_buffers
                move_status = symb == 'c' ? TO_COPY : TO_MOVE;
                snprintf(moved_path, PATH_MAX, "%s/%s", current_path, files[selected_dir].name);
                // char command[PATH_MAX];
                // sprintf(command, "echo %s | xsel -b", moved_path);  // attempts to send filename to system buffer
                // system(command);
                break;
            case 'v':
                if (files[selected_dir].filetype == DT_DIR && move_status != WAS_NOT_SELECTED) {
                    snprintf(tmp_path, PATH_MAX, "%s/%s", current_path, files[selected_dir].name);
                    int err = move_files(moved_path, tmp_path, basename(moved_path));
                    if (err == HAS_SAME_PATH) {
                        wprintw(win, "Sorry, file with the same name is in this directory.\n");
                    }
                    else if (err) {
                        exit(err);
                    }
                    err = 0;
                    if (move_status == TO_MOVE) {
                        err = remove(moved_path);
                    }
                    if (err) {
                        exit(err);
                    }
                    move_status = WAS_NOT_SELECTED;
                    get_files_in_path(current_path, files, &count_of_files, all_files);
                }
                break;
            case 'h':
                all_files = !all_files;
                get_files_in_path(current_path, files, &count_of_files, all_files);
                break;
            case 'q':
                wclear(win);
                return;
        }
        print_directories(win, begin_str, files, count_of_files, selected_dir);
    }
}