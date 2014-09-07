
add_executable(sensor_data sensor_data.cpp)
add_executable(eqr_data eqr_data.cpp)

target_link_libraries(sensor_data
  ${LIBELPHELPHG_LIBRARY_LIST}
)

target_link_libraries(eqr_data
  ${LIBELPHELPHG_LIBRARY_LIST}
)

install(TARGETS
  sensor_data
  eqr_data
  DESTINATION bin
)
