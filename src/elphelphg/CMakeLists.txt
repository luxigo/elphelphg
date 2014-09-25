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

install(TARGETS elphelphg DESTINATION lib)

install(FILES
  cameraArray.hpp
  camera.hpp
  channel.hpp
  eqrData.hpp
  file.hpp
  imagej_elphel_preferences.hpp
  sensorData.hpp
  utils.hpp
  xml.hpp
  DESTINATION include/elphelphg)

# Generate and install pkgconfig file
include(FindPkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  # convert lists of link libraries into -lstdc++ -lm etc..
  foreach(LIB ${CMAKE_CXX_IMPLICIT_LINK_LIBRARIES} ${PLATFORM_LIBS})
    set(PRIVATE_LIBS "${PRIVATE_LIBS} -l${LIB}")
  endforeach()
  # Produce a pkg-config file for linking against the shared lib
  configure_file("elphelphg.pc.in" "elphelphg.pc" @ONLY)
  install(FILES       "${CMAKE_CURRENT_BINARY_DIR}/elphelphg.pc"
          DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
endif()
