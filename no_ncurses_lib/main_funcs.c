#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "main_funcs.h"

void init_screen() {
    // Установить цветовую схему
    printf("\033[0m"); // сбросить цвет
    printf("\033[48;2;0;0;0m"); // установить цвет фона
    printf("\033[38;2;255;255;255m"); // установить цвет текста

    // Очистить экран
    printf("\033[2J");

    // Скрыть курсор
    printf("\033[?25l");
}

void print_text(int x, int y, const char* text) {
    // Переместить курсор в заданные координаты
    printf("\033[%d;%dH", y, x);

    // Вывести текст заданным цветом
    printf("%s", text);
}

void another_print_text(int x, int y, const char* text) {
    // Переместить курсор в заданные координаты
    printf("\033[%d;%dH", y, x);

    // Вывести текст заданным цветом
    printf("%s", text);
}

void clear_screen() {
    // Очистить экран
    printf("\033[2J");

    // Переместить курсор в верхний левый угол
    printf("\033[H");
}

void print_color(char *str, int fg_color, int bg_color) {
    printf("\033[%d;%dm%s\033[0m", fg_color, bg_color, str);
}

void simple_print(char *text) {
    print_color(text, FG_BLACK, BG_CYAN);
}

void shutdown_screen() {
    // Сбросить цвет и стиль
    printf("\033[0m");

    // Включить курсор
    printf("\033[?25h");
}
