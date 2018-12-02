cmake_minimum_required(VERSION 2.6)
include(CMakeToolsHelpers OPTIONAL)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
project(ROADEF)

add_executable(toukeute
    main.cpp
    instance.h
    instance.cpp
    red_monster.h
    red_monster.cpp
    red_point.h
    red_point.cpp
    glass_item.h
    glass_item.cpp
    glass_stack.h
    glass_stack.cpp
    glass_node.h
    
    glass_node.cpp    
    glass_plate.h
    glass_plate.cpp
    location.h
    location.cpp
    glass_defect.h
    glass_defect.cpp
    glass_algorithm.h
    glass_algorithm.cpp
    glass_structure.h
    glass_structure.cpp

    glass_cut.h
    glass_cut.cpp
)

