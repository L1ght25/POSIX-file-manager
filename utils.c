#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "utils.h"


int comparator_of_files(const void *first, const void *second) {
    return strcmp(((const MetaFile *)first)->name, ((const MetaFile *)second)->name);
}

int move_files(const char *src, const char *dest_path, const char *dest_name) {
    char buf[MAX_BUF_SIZE];
    snprintf(buf, PATH_MAX, "%s/%s", dest_path, dest_name);
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