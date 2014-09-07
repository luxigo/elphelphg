
add_executable(sensor_data sensor_data.cpp)
add_executable(eqr_data eqr_data.cpp)

target_link_libraries(sensor_data
  ${LIBELPHELPHG_LIBRARY}
  ${LIBXML2_LIBRARY}
  ${LIBBOOST_SYSTEM_LIBRARY}
  ${LIBBOOST_FILESYSTEM_LIBRARY}
  ${LIBBOOST_REGEX_LIBRARY}
  ${LIBTIFF_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(eqr_data
  ${LIBELPHELPHG_LIBRARY}
  ${LIBXML2_LIBRARY}
  ${LIBBOOST_SYSTEM_LIBRARY}
  ${LIBBOOST_FILESYSTEM_LIBRARY}
  ${LIBBOOST_REGEX_LIBRARY}
  ${LIBTIFF_LIBRARY}
  ${CMAKE_THREAD_LIBS_INIT}
)

install (TARGETS
  sensor_data
  eqr_data
  DESTINATION bin)

set(CONFIGURED_ONCE TRUE CACHE INTERNAL
    "A flag showing that CMake has configured at least once.")
