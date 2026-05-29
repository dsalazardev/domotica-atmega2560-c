cmake_minimum_required(VERSION 4.1)
project(Proyecto C)

set(CMAKE_C_STANDARD 11)

add_executable(Proyecto main.c
        src/Seguridad.cpp
        include/Seguridad.h)
