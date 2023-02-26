#ifndef _DIRS_HANDLER_H
#define _DIRS_HANDLER_H 1

#include "utils.h"
#include <ncurses.h>
#include <stddef.h>

int get_files_in_path(const char *curr_path, MetaFile *files, size_t *size, bool all_files);

void directory_handler(WINDOW *win, const char *curr_path);

#endif
