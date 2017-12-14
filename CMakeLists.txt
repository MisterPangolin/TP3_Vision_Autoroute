cmake_minimum_required ( VERSION 2.8)
project ( VSION-auto )

# setting the compiler to c++11
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# sources extension
set(SRCEXT "cpp")

file(
    GLOB_RECURSE
    source_files
    *.${SRCEXT}
)

# required packages
find_package ( OpenCV REQUIRED )

# adding the source files to the executable
add_executable(VSION-auto main.cpp)

# linking other libraries
target_link_libraries(VSION-auto  ${OpenCV_LIBS})
