cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project("elphelphg")

set(CMAKE_CXX_COMPILER g++)

find_package(OpenMP)

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    set(warnings "-Wall -Wextra -Werror")
    set(cxx_flags "-std=c++11")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()

if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings} ${cxx_flags}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

#find_package(libxml2)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
  /usr/include/libxml2
)

add_library(elphelphg
  cameraArray.cpp
  camera.cpp
  channel.cpp
  eqrData.cpp
  file.cpp
  imagej_elphel_preferences.cpp
  sensorData.cpp
  utils.cpp
  xml.cpp
)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
