# Main CMakeLists.txt

cmake_minimum_required(VERSION 2.6)
include(CMakeToolsHelpers OPTIONAL)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_CXX_STANDARD 11)
project(RoadefJ5)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
set(BOOST_ALL_DYN_LINK           ON)

add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

add_executable(glassTests
    # GlassData
    GlassData/glassDefect.h GlassData/glassDefect.cpp
    GlassData/glassInstance.h GlassData/glassInstance.cpp
    GlassData/glassItem.h GlassData/glassItem.cpp
    GlassData/glassPlate.h GlassData/glassPlate.cpp
    GlassData/glassConstants.h

    # GlassCutter
    GlassCutter/glassCut.h GlassCutter/glassCut.cpp
    GlassCutter/glassCutter.h GlassCutter/glassCutter.cpp
    GlassCutter/glassLocation.h GlassCutter/glassLocation.cpp
    GlassCutter/glassNode.h GlassCutter/glassNode.cpp
    GlassCutter/redMonster.h GlassCutter/redMonster.cpp
    GlassCutter/redPoint.h GlassCutter/redPoint.cpp
    GlassCutter/glassStack.h GlassCutter/glassStack.cpp

    # GlassKernel
    GlassKernel/heuristic.h GlassKernel/heuristic.cpp
    GlassKernel/glassMove.h GlassKernel/glassMove.cpp
    GlassKernel/glassRand.h
    GlassKernel/mainTest.cpp 

    # Moves
    GlassKernel/GlassMoves/swap.h GlassKernel/GlassMoves/swap.cpp
    GlassKernel/GlassMoves/kConsecutivePermutation.h GlassKernel/GlassMoves/kConsecutivePermutation.cpp
)

add_executable(tookeute
    # GlassData
    GlassData/glassDefect.h GlassData/glassDefect.cpp
    GlassData/glassInstance.h GlassData/glassInstance.cpp
    GlassData/glassItem.h GlassData/glassItem.cpp
    GlassData/glassPlate.h GlassData/glassPlate.cpp
    GlassData/glassConstants.h

    # GlassCutter
    GlassCutter/glassCut.h GlassCutter/glassCut.cpp
    GlassCutter/glassCutter.h GlassCutter/glassCutter.cpp
    GlassCutter/glassLocation.h GlassCutter/glassLocation.cpp
    GlassCutter/glassNode.h GlassCutter/glassNode.cpp
    GlassCutter/redMonster.h GlassCutter/redMonster.cpp
    GlassCutter/redPoint.h GlassCutter/redPoint.cpp
    GlassCutter/glassStack.h GlassCutter/glassStack.cpp

    # GlassKernel
    GlassKernel/heuristic.h GlassKernel/heuristic.cpp
    GlassKernel/glassMove.h GlassKernel/glassMove.cpp
    GlassKernel/glassRand.h
    GlassKernel/main.cpp 

    # Moves
    GlassKernel/GlassMoves/swap.h GlassKernel/GlassMoves/swap.cpp
    GlassKernel/GlassMoves/kConsecutivePermutation.h GlassKernel/GlassMoves/kConsecutivePermutation.cpp
)


target_link_libraries(tookeute ${Boost_LIBRARIES})
target_link_libraries(glassTests ${Boost_LIBRARIES})