cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(sistema_domotico)

include_directories(include)

add_executable(sistema_domotico_tester
    src/main.cpp
)