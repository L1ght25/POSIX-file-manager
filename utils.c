#include "utils.h"
#include <dirent.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>


int comparator_of_files(const void *first, const void *second) {
    return strcmp(((const MetaFile *)first)->name, ((const MetaFile *)second)->name);
}

int move_files(const char *src, const char *dest_path, const char *dest_name, bool is_dir) {
    char buf[MAX_BUF_SIZE];
    if (is_dir) {
        snprintf(buf,  sizeof(buf), "%s/%s", dest_path, dest_name);
        DIR *dir = opendir(buf);
        if (dir != NULL) {
            return HAS_SAME_PATH;
        }
        snprintf(buf, sizeof(buf), "cp -r %s %s/%s", src, dest_path, dest_name);
        return system(buf);
    }
    snprintf(buf, PATH_MAX, "%s/%s", dest_path, dest_name);
    
    FILE * file = fopen(buf, "r");

    if (file != NULL) {
        fclose(file);
        return HAS_SAME_PATH;
    }

    int fd_in = open(src, O_RDWR);
    int fd_out = open(buf, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    size_t size_of_read;
    size_t size_of_write;
    while ((size_of_read = read(fd_in, buf, MAX_BUF_SIZE))) {
        size_of_write = write(fd_out, buf, size_of_read);
        if (size_of_read != size_of_write) {
            return 1;
        }
    }
    close(fd_in);
    close(fd_out);
    return 0;
}

int check_perms(char *path, unsigned char filetype) {
    if (access(path, R_OK | W_OK) == -1) {
        if (filetype == DT_LNK) {
            struct stat buf;
            if (lstat(path, &buf) || stat(path, &buf)) {
                simple_print("Invalid symlink\n");
                return 1;
            }
        }
        simple_print("Sorry, you have not permissions to read this file/directory :(\n");
        return 1;
    }
    return 0;
}
