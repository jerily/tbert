cmake_minimum_required(VERSION 3.22.1)
project(tbert VERSION 1.0.0 LANGUAGES CXX C)

set(TARGET tbert)
set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_C_STANDARD_REQUIRED true)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "-DTCL_THREADS -DVERSION=${PROJECT_VERSION} ${CMAKE_CXX_FLAGS}")

add_library(tbert SHARED MODULE library.cc)
set_target_properties(tbert PROPERTIES POSITION_INDEPENDENT_CODE ON)

#add_compile_options(-g -O0 -DDEBUG -Wall -Wextra -Wconversion)

include(${CMAKE_CURRENT_LIST_DIR}/FindTCL.cmake)

link_directories(bert.cpp/build bert.cpp/build/ggml/src)
target_link_libraries(tbert PRIVATE bert ggml ${TCL_LIBRARY})
get_filename_component(TCL_LIBRARY_PATH "${TCL_LIBRARY}" PATH)

install(TARGETS ${TARGET}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${TARGET}${PROJECT_VERSION}
)

configure_file(pkgIndex.tcl.in pkgIndex.tcl @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pkgIndex.tcl
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${TARGET}${PROJECT_VERSION}
)

