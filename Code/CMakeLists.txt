cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11")

project(ROADEF)
add_executable(toukeute
    main.cpp
    builddata.cpp
    Checker.cpp
    classinitialization.cpp
    globalvar.cpp
    Parser.cpp
    results.cpp
    builddata.h
    Checker.h
    classinitialization.h
    GlobalVar.h
    Parser.h
    results.h
    glass_defect.h
    glass_defect.cpp
    glass_item.h
    glass_item.cpp
    glass_node.cpp
    glass_node.h
    glass_plate.h
    glass_plate.cpp
    glass_stack.h
    glass_stack.cpp
    algorithm.h
    algorithm.cpp
    )
