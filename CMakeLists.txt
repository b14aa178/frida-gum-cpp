cmake_minimum_required(VERSION 3.20)

project(frida-gum-cpp CXX)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "x64")
else ()
    message(STATUS "x86")
endif ()

set(CMAKE_CXX_STANDARD 17)

add_library(frida-gum-cpp fridacpp.cpp)
target_include_directories(frida-gum-cpp PRIVATE libs)
target_link_libraries(frida-gum-cpp PRIVATE "${CMAKE_SOURCE_DIR}/libs/x86/frida-gum.lib" shlwapi)


add_executable(example example.cpp)
target_link_libraries(example PRIVATE frida-gum-cpp)
target_include_directories(example PRIVATE libs)
target_compile_options(example PRIVATE /utf-8)