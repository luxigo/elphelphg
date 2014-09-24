#include <elphelphg/cameraArray.hpp>
#include <elphelphg/channel.hpp>
#include <elphelphg/eqrData.hpp>
#include <elphelphg/sensorData.hpp>
#include <iostream>
#include <exception>
#include <stdlib.h>

#include "elphelphg_cstubs.h"

CameraArray *e4pi;

int phg_init(char *imagej_prefs_xml) {
  try {
    e4pi=new CameraArray(CameraArray::EYESIS4PI_CAMERA,imagej_prefs_xml);
    return 0;
  } catch(std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch(std::string &msg) {
    std::cerr << msg << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "unhandled exception\n";
    return 1;
  }
}

int phg_sensorData(double *value, char *property, int channel) {
  try {
    value[0]=e4pi->channel(channel)->sensor->get(property);
    return 0;
  } catch(std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch(std::string &msg) {
    std::cerr << msg << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "unhandled exception\n";
    return 1;
  }
}

int phg_eqrData(double *value, char *property, int channel) {
  try {
    value[0]=e4pi->channel(channel)->eqr->get(property);
    return 0;
  } catch(std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  } catch(std::string &msg) {
    std::cerr << msg << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "unhandled exception\n";
    return 1;
  }
}
