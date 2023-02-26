cmake_minimum_required(VERSION 3.22)
project(file_manager)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_executable(file_manager main.c directories_handler.c print_funcs.c utils.c)
target_link_libraries(file_manager ${CURSES_LIBRARIES})
