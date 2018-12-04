# GlassCutter (needs GlassData)

add_library(GlassCutter
    glassCut.h glassCut.cpp
    glassCutter.h glassCutter.cpp
    glassNode.h glassNode.cpp
    glassLocation.h glassLocation.cpp
    redPoint.h redPoint.cpp
    redMonster.h redMonster.cpp)

target_link_libraries(GlassCutter LINK_PUBLIC glassData)
target_include_directories (GlassCutter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})