cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(sistema_domotico VERSION 1.0)
set (CMAKE_CXX_STANDARD 11)

include_directories(include)

add_library(sistema_domotico_logic STATIC
    include/home.h
    include/cycledevice.h
    include/manualdevice.h
    include/device.h
    include/logger.h
    src/home.cpp
    src/cycledevice.cpp
    src/manualdevice.cpp
    src/device.cpp
    src/logger.cpp
)

add_executable(sistema_domotico
    src/main.cpp
)

add_executable(sistema_domotico_tester
    test/main.cpp
)

target_link_libraries(sistema_domotico sistema_domotico_logic)
target_link_libraries(sistema_domotico_tester sistema_domotico_logic)