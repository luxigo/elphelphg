# sensor_data
add_executable(sensor_data sensor_data.cpp)
target_link_libraries(sensor_data
  ${LIBELPHELPHG_LIBRARY_LIST}
)

# eqr_data
add_executable(eqr_data eqr_data.cpp)
target_link_libraries(eqr_data
  ${LIBELPHELPHG_LIBRARY_LIST}
)

# libelphelphg_cstub
add_library(elphelphg_cstub OBJECT elphelphg_cstub.cpp)

# sensor_data_c
add_executable(sensor_data_c sensor_data_c.c $<TARGET_OBJECTS:elphelphg_cstub>)
target_link_libraries(sensor_data_c
  ${LIBELPHELPHG_LIBRARY_LIST}
)

# eqr_data_c
add_executable(eqr_data_c eqr_data_c.c $<TARGET_OBJECTS:elphelphg_cstub>)
target_link_libraries(eqr_data_c
  ${LIBELPHELPHG_LIBRARY_LIST}
)

####

install(TARGETS
  sensor_data
  eqr_data
  sensor_data_c
  eqr_data_c
  DESTINATION bin
)
