cmake_minimum_required(VERSION 3.22)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
project(extensions DESCRIPTION "extensions lib for opening files in vim, etc")

add_library(extensions SHARED extensions_lib.c)
