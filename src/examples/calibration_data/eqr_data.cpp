#include <elphelphg/cameraArray.hpp>
#include <elphelphg/channel.hpp>
#include <elphelphg/eqrData.hpp>
#include <iostream>
#include <exception>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc!=4) {
    std::cerr << "usage: " << argv[0] << " <imagej_config> <property> <channel>" << std::endl;
    return 1;
  }
  try {
  CameraArray e4pi(CameraArray::EYESIS4PI_CAMERA,argv[1]);
  std::cout << argv[2] << ": " << e4pi.channel(atoi(argv[3]))->eqr->get(argv[2]) << std::endl;
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
