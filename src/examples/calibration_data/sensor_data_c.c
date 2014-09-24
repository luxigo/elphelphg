#include <stdio.h>
#include <stdlib.h>

#include "elphelphg_cstubs.h"

int main(int argc, char **argv) {
  if (argc!=4) {
    fprintf(stderr,"usage: %s <imagej_config> <property> <channel>\n",argv[0]);
    return 1;
  }
  if (phg_init(argv[1])) {
    return 1;
  }

  double value;
  if (phg_sensorData(&value,argv[2],atoi(argv[3]))) {
    return 1;
  }

  printf("%s: %f\n",argv[2],value);
  
  return 0;
  
}

