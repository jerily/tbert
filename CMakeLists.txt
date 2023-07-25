cmake_minimum_required(VERSION 3.22.1)
project(tbert LANGUAGES CXX C)

set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_C_STANDARD_REQUIRED true)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_subdirectory(bert.cpp)

#add_compile_options(-g -O0 -DDEBUG -Wall -Wextra -Wconversion)
add_compile_options(-O2 -Wall -Wextra -Wconversion)

include_directories(bert.cpp/ggml/include bert.cpp/)
link_directories(bert.cpp/build bert.cpp/build/ggml/src)
add_library(tbert SHARED library.cc)
target_link_libraries(tbert PRIVATE bert ggml tcl8.6)