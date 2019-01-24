# Main CMakeLists.txt

cmake_minimum_required(VERSION 2.6)
include(CMakeToolsHelpers OPTIONAL)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_CXX_STANDARD 11)
project(RoadefJ5)

find_package(Boost 1.58.0 REQUIRED thread chrono)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
set(Boost_USE_STATIC_LIBS        ON) # pour ne pas avoir besoin de chercher le boost du systeme
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
set(BOOST_ALL_DYN_LINK           ON)

find_package (Boost 1.4.0 REQUIRED COMPONENTS thread chrono)
if (Boost_FOUND)
  # From the offical documentation:
  # Add include directories to the build. [...] If the SYSTEM option is given,
  # the compiler will be told the directories are meant as system include
  # directories on some platforms (signalling this setting might achieve effects
  # such as the compiler skipping warnings [...])."
  include_directories (SYSTEM ${Boost_INCLUDE_DIR})

  # From the offical documentation:
  # "Specify directories in which the linker will look for libraries. [...] Note
  # that this command is rarely necessary. Library locations returned by
  # find_package() and find_library() are absolute paths. Pass these absolute
  # library file paths directly to the target_link_libraries() command. CMake
  # will ensure the linker finds them."
  #link_directories (${Boost_LIBRARY_DIRS})
endif ()

#add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

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
    GlassKernel/GlassMoves/kPermutation.h GlassKernel/GlassMoves/kPermutation.cpp
    GlassKernel/GlassMoves/kInsert.h GlassKernel/GlassMoves/kInsert.cpp
)

target_link_libraries(glassTests ${Boost_LIBRARIES})