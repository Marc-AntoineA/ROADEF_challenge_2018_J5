# Main CMakeLists.txt

cmake_minimum_required(VERSION 2.6)
include(CMakeToolsHelpers OPTIONAL)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
project(RoadefJ5)

#find_package(Boost REQUIRED)
#include_directories(${Boost_INCLUDE_DIR})
#link_directories(${Boost_LIBRARY_DIRS})
#set(Boost_USE_STATIC_LIBS        OFF)
#set(Boost_USE_MULTITHREADED      ON)
#set(Boost_USE_STATIC_RUNTIME     OFF)
#set(BOOST_ALL_DYN_LINK           ON)
#target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

#add_subdirectory (./GlassData)
#add_subdirectory (GlassCutter)
#add_subdirectory (GlassKernel)

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

    # GlassKernel
    GlassKernel/heuristic.h GlassKernel/heuristic.cpp
    GlassKernel/main.cpp 
)

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

    # GlassKernel
    GlassKernel/heuristic.h GlassKernel/heuristic.cpp
    GlassKernel/mainTest.cpp 
)