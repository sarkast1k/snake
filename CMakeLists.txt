cmake_minimum_required(VERSION 4.0)
project(SnakeGame CXX)

# Set compiler
set(CMAKE_CXX_COMPILER "g++")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set source files
add_executable(snake main.cpp)

# Include raygui.h path
include_directories(C:/Users/sarkast1k/Downloads/code/snake/include)

# Include raylib headers
include_directories(C:/raylib/raylib/src)

# Link raylib statically
target_link_libraries(snake C:/raylib/raylib/src/libraylib.a)

# Link required Windows libraries
target_link_libraries(snake opengl32 gdi32 winmm)
