cmake_minimum_required(VERSION 3.22.1)
project(tbert LANGUAGES CXX C)

set(TARGET tbert)
set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_C_STANDARD_REQUIRED true)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

get_filename_component(TCL_INCLUDE_PATH_PARENT "${TCL_INCLUDE_PATH}" PATH)
get_filename_component(TK_INCLUDE_PATH_PARENT "${TK_INCLUDE_PATH}" PATH)

get_filename_component(TCL_LIBRARY_PATH "${TCL_LIBRARY}" PATH)
get_filename_component(TCL_LIBRARY_PATH_PARENT "${TCL_LIBRARY_PATH}" PATH)

set(TCL_POSSIBLE_LIB_PATHS
  "${TCL_INCLUDE_PATH_PARENT}/lib"
  "${TCL_LIBRARY_PATH}"
  "${TCL_TCLSH_PATH_PARENT}/lib"
)

set(TCL_POSSIBLE_LIB_PATH_SUFFIXES
  lib/tcl/tcl8.7
  lib/tcl/tk8.7
  lib/tcl/tcl8.6
  lib/tcl/tk8.6
  lib/tcl/tcl8.5
  lib/tcl/tk8.5
  lib/tcl/tcl8.4
  lib/tcl/tk8.4
)

find_library(TCL_LIBRARY
  NAMES
  tcl
  tcl87 tcl8.7 tcl87t tcl8.7t
  tcl86 tcl8.6 tcl86t tcl8.6t
  tcl85 tcl8.5
  tcl84 tcl8.4
  tcl83 tcl8.3
  tcl82 tcl8.2
  tcl80 tcl8.0
  PATHS ${TCLTK_POSSIBLE_LIB_PATHS}
  PATH_SUFFIXES ${TCL_POSSIBLE_LIB_PATH_SUFFIXES}
  )

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
    target_link_libraries(tbert PRIVATE bert ggml ${TCL_LIBRARY} nsd nsthread)
    target_compile_options(tbert PRIVATE -DUSE_NAVISERVER)
    install(TARGETS ${TARGET}
            LIBRARY DESTINATION ${NAVISERVER}/lib
            )
else ()
    include_directories(bert.cpp/ggml/include bert.cpp/)
    link_directories(bert.cpp/build bert.cpp/build/ggml/src)
    target_link_libraries(tbert PRIVATE bert ggml ${TCL_LIBRARY})
endif()

