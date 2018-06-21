cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11")
project(ROADEF)

add_executable(checker
    GlassDefect.cpp
    GlassItem.cpp
    GlassNode.cpp
    GlassPlate.cpp
    GlassStack.cpp
    main.cpp
    GlassDefect.h
    GlassItem.h
    GlassNode.h
    GlassPlate.h
    GlassStack.h
    main.h)
