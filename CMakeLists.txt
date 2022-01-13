cmake_minimum_required(VERSION 3.14)

project(3d_engine LANGUAGES CXX)

find_package(OpenGL REQUIRED)
if (WIN32)
    set(DEPS_LIBS glfw3_mt glew32s)
else(WIN32)
    set(DEPS_LIBS glfw GLEW)
endif(WIN32)

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
target_link_libraries(3d_engine logger ${DEPS_LIBS} ${OPENGL_LIBRARY})
if (WIN32)
    set_property(TARGET 3d_engine PROPERTY
      MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif(WIN32)
