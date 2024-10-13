cmake_minimum_required(VERSION 3.22)
project(CL4AF)

set(CMAKE_CXX_STANDARD 17)

#add_definitions(-DDEBUG)
add_library(lib
        Clause.cpp
        Solver.cpp
        Heap.cpp
        log.h
)
add_executable(main main.cpp)
target_link_libraries(main lib)
target_compile_options(lib PRIVATE -O3)
target_compile_options(main PRIVATE -O3)
