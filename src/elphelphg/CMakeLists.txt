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
