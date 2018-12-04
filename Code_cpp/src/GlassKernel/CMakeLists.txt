# glassKernel

# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found. Any number of sources could be listed here.
add_library (GlassKernel 
    glassDefect.h glassDefect.cpp
    glassItem.h glassItem.cpp
    glassPlate.h glassPlate.cpp
    glassStack.h glassStack.cpp
    glassInstance.h glassInstance.cpp)

# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_link_libraries(GlassKernel LINK_PUBLIC GlassKernel)
