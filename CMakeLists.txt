cmake_minimum_required(VERSION 3.10)
project(Domotic_System)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_library(${PROJECT_NAME} SHARED
    src/Device.cpp
    src/Manual.cpp
    src/PrefixCycle.cpp
    src/Time.cpp
    src/Scheduler.cpp
    src/DomoticSystem.cpp)

add_executable(main
    src/main.cpp)

target_link_libraries(main ${PROJECT_NAME})
