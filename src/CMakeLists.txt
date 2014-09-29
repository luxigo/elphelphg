cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(ElphelPHG C CXX)

set(CMAKE_CXX_COMPILER g++)
set(BUILD_SHARED_LIBS ON)

# The project version number.
set(VERSION_MAJOR   0   CACHE STRING "Project major version number.")
set(VERSION_MINOR   0   CACHE STRING "Project minor version number.")
set(VERSION_PATCH   1   CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

# phtreads
find_package(Threads)

# OpenMP
find_package(OpenMP)

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# Compiler flags
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
    set(cxx_flags "-std=c++11")
endif()

if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings} ${cxx_flags}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

# source dirs
set(LIBELPHELPHG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/elphelphg)
set(EXAMPLES_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)

# pkgconfig
find_package(PkgConfig)

# libxml2
pkg_check_modules(PC_LIBXML QUIET libxml-2.0)
set(LIBXML2_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})

find_path(LIBXML2_INCLUDE_DIR libxml/xpath.h
          HINTS ${PC_LIBXML_INCLUDEDIR} ${PC_LIBXML_INCLUDE_DIRS}
          PATH_SUFFIXES libxml2 )

find_library(LIBXML2_LIBRARY NAMES xml2 libxml2
             HINTS ${PC_LIBXML_LIBDIR} ${PC_LIBXML_LIBRARY_DIRS} )

# libboost
pkg_check_modules(PC_LIBBOOST QUIET libboost-filesystem)
set(LIBBOOST_DEFINITIONS ${PC_LIBBOOST_CFLAGS_OTHER})

find_path(LIBBOOST_INCLUDE_DIR boost/filesystem.hpp
  HINTS ${PC_LIBBOOST_INCLUDEDIR} ${PC_LIBBOOST_INCLUDE_DIRS}
          PATH_SUFFIXES boost )

find_library(LIBBOOST_FILESYSTEM_LIBRARY NAMES boost_filesystem libboost_filesystem
   HINTS ${PC_LIBBOOST_LIBDIR} ${PC_LIBBOOST_LIBRARY_DIRS} )
find_library(LIBBOOST_SYSTEM_LIBRARY NAMES boost_system libboost_system
   HINTS ${PC_LIBBOOST_LIBDIR} ${PC_LIBBOOST_LIBRARY_DIRS} )
find_library(LIBBOOST_REGEX_LIBRARY NAMES boost_regex libboost_regex
   HINTS ${PC_LIBBOOST_LIBDIR} ${PC_LIBBOOST_LIBRARY_DIRS} )

# libtiff
pkg_check_modules(PC_LIBTIFF QUIET libtiff)
set(LIBTIFF_DEFINITIONS ${PC_LIBTIFF_CFLAGS_OTHER})

find_path(LIBTIFF_INCLUDE_DIR tiff.h
  HINTS ${PC_LIBTIFF_INCLUDEDIR} ${PC_LIBTIFF_INCLUDE_DIRS}
  PATH_SUFFIXES libtiff )
find_library(LIBTIFF_LIBRARY NAMES tiff libtiff
  HINTS ${PC_LIBTIFF_LIBDIR} ${PC_LIBTIFF_LIBRARY_DIRS} )

# libelphelphg
if (BUILD_SHARED_LIBS)
  set(LIBELPHELPHG_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/elphelphg/libelphelphg.so)
else()
  set(LIBELPHELPHG_LIBRARY ${CMAKE_CURRENT_BINARY_DIR}/elphelphg/libelphelphg.a)
endif()

# includes
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIBELPHELPHG_SOURCE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${LIBBOOST_INCLUDE_DIR}
  ${LIBTIFF_INCLUDE_DIR}
)

# libraries
set(LIBELPHELPHG_LIBRARY_LIST
  ${LIBELPHELPHG_LIBRARY}
  ${LIBXML2_LIBRARY}
  ${LIBBOOST_SYSTEM_LIBRARY}
  ${LIBBOOST_FILESYSTEM_LIBRARY}
  ${LIBBOOST_REGEX_LIBRARY}
  ${LIBTIFF_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
)

# subdirectories
add_subdirectory(${LIBELPHELPHG_SOURCE_DIR})
add_subdirectory(${EXAMPLES_SOURCE_DIR})

# Doxygen documentation
option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" OFF)
if(BUILD_DOCUMENTATION)
  find_package(Doxygen)
  option(BUILD_DOCUMENTATION "Create and install the HTML based API documentation (requires Doxygen)" ${DOXYGEN_FOUND})

  if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc)
  endif()
endif()

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
