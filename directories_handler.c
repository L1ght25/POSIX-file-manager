#include <ncurses.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "utils.h"
#include "directories_handler.h"

void print_directories(WINDOW *win, int begin_str, MetaFile *files, int size_of_files, int curr_dir);

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
            return -1;
        }
        files[i++] = (MetaFile){ .name = curr_dir->d_name, .modified_time = statbuf.st_mtime, .size = statbuf.st_size, .filetype = curr_dir->d_type};
        // files[i - 1].filetype = curr_dir->d_type == DT_DIR ? IS_DIR : IS_FILE;
    }
    qsort(files, i, sizeof(*files), comparator_of_files);
    *size = i;
    closedir(dir);
    return 0;
}

void directory_handler(WINDOW *win, const char *input_path) {
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
    char moved_path[PATH_MAX];
    char moved_name[PATH_MAX];
    MetaFile files[MAX_FILES_IN_DIR];

    size_t count_of_files = 0;
    
    int err = get_files_in_path(input_path, files, &count_of_files, all_files);
    if (err) {
        exit(1);
    }

    print_directories(win, begin_str, files, count_of_files, selected_dir);
    while (1) {
        int symb = wgetch(win);
        wclear(win);
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
                if (files[selected_dir].filetype == DT_DIR) {
                    snprintf(tmp_path, PATH_MAX, "%s/%s", current_path, files[selected_dir].name);
                    get_files_in_path(tmp_path, files, &count_of_files, all_files);
                    strcpy(current_path, tmp_path);
                    begin_str = 0;
                    selected_dir = 0;
                }
                break;
            case 'd':
                if (strcmp(files[selected_dir].name, "..")) {  // we should not delete .. directory)
                    snprintf(tmp_path, PATH_MAX, "%s/%s", current_path, files[selected_dir].name);
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
                strcpy(moved_name, files[selected_dir].name);
                break;
            case 'v':
                if (files[selected_dir].filetype == DT_DIR && move_status != WAS_NOT_SELECTED) {
                    snprintf(tmp_path, PATH_MAX, "%s/%s", current_path, files[selected_dir].name);
                    int err = move_files(moved_path, tmp_path, moved_name);
                    if (err) {
                        exit(err);
                    }
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
        }
        print_directories(win, begin_str, files, count_of_files, selected_dir);
    }
}