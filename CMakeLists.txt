cmake_minimum_required(VERSION 3.14)

project(3d_engine LANGUAGES CXX)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)
include_directories("include")

file(GLOB SRC_DIR "src/*.cpp")
file(GLOB INCLUDE_DIR "include/*.h" "include/*/*.hpp" "include/*/*.h")
add_library(3d_engine ${SRC_DIR} ${INCLUDE_DIR})
target_link_libraries(3d_engine logger glfw GLEW ${OPENGL_LIBRARY})
