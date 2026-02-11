cmake_minimum_required(VERSION 4.1)
project(canimation C)

set(CMAKE_C_STANDARD 11)

add_executable(canimation main.c
        main.c)

# ncurses'i direkt linkliyoruz, find_package YOK
target_link_libraries(canimation ncursesw tinfo)
