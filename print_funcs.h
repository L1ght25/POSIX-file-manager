#include <time.h>
#include <dirent.h>
#include "utils.h"

enum { SELECTED_FILE_COLOR = 7, FIFO_COLOR = 4, LINK_COLOR = 3, DIR_COLOR = 2 };

void print_directories(int begin_str, MetaFile *files, int size_of_files, int curr_dir);
