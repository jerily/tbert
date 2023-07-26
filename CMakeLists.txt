cmake_minimum_required(VERSION 3.22.1)
project(tbert LANGUAGES CXX C)

set(TARGET tbert)
set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_C_STANDARD_REQUIRED true)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(bert.cpp)

add_library(tbert SHARED library.cc)
set_target_properties(tbert PROPERTIES POSITION_INDEPENDENT_CODE ON)

#add_compile_options(-g -O0 -DDEBUG -Wall -Wextra -Wconversion)
add_compile_options(-O2 -Wall -Wextra -Wconversion)

if (NAVISERVER)
    include_directories(bert.cpp/ggml/include bert.cpp/ ${NAVISERVER}/include)
    link_directories(bert.cpp/build bert.cpp/build/ggml/src)
    target_link_directories(tbert PRIVATE ${NAVISERVER}/lib)
    target_link_libraries(tbert PRIVATE bert ggml tcl8.6 nsd nsthread)
    target_compile_options(tbert PRIVATE -DUSE_NAVISERVER)
    install(TARGETS ${TARGET}
            LIBRARY DESTINATION ${NAVISERVER}/lib
            )
else ()
    include_directories(bert.cpp/ggml/include bert.cpp/)
    link_directories(bert.cpp/build bert.cpp/build/ggml/src)
    target_link_libraries(tbert PRIVATE bert ggml tcl8.6)
endif()

