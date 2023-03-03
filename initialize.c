#include "initialize.h"
#include "utils.h"
#include <termios.h>
#include <unistd.h>

struct termios initialize() {
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    return oldt;
}

void unitialize(struct termios oldt) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}