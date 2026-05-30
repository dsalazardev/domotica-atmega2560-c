cmake_minimum_required(VERSION 4.1)
project(Proyecto C CXX)

set(CMAKE_CXX_STANDARD 11)

add_executable(Proyecto
        src/main.ino
        src/Seguridad.cpp
        include/Seguridad.h
)

set_source_files_properties(src/main.ino PROPERTIES LANGUAGE CXX)

target_include_directories(Proyecto PRIVATE include)