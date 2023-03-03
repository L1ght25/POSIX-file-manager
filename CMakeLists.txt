cmake_minimum_required(VERSION 3.22)
project(file_manager)

add_executable(file_manager main.c directories_handler.c print_funcs.c utils.c extensions.c initialize.c no_ncurses_lib/main_funcs.c)
target_link_libraries(file_manager PUBLIC dl)