/*
 * ElphelPHG - Elphel PHG cameras related developement
 *
 * Copyright (c) 2014 FOXEL SA - http://foxel.ch
 * Please read <http://foxel.ch/license> for more information.
 *
 * This file is part of the FOXEL project <http://foxel.ch>.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * Additional Terms:
 *
 *      You are required to preserve legal notices and author attributions in
 *      that material or in the Appropriate Legal Notices displayed by works
 *      containing it.
 *
 *      You are required to attribute the work as explained in the "Usage and
 *      Attribution" section of <http://foxel.ch/license>.
 */

#ifndef CHANNEL_HPP_
#define CHANNEL_HPP_

#ifndef cimg_version
#define cimg_use_tiff
#define cimg_display 0
#include <CImg.h>
#endif

#include <string>

class SensorData;
class EqrData;
class cameraArray;

class Channel {
public:
  unsigned int num;
  Camera *camera;
  std::string eqrtiff;
  std::string calibtiff;
  cimg_library::CImg<float> *eqrtiff_cimg;
  cimg_library::CImg<float> *calibtiff_cimg;
  SensorData *sensor;
  EqrData *eqr;
  int roll;
  int rotatedImageHeight;
  int rotatedImageWidth;
  int sensorHeight;
  int sensorWidth;
  double K[9];
  double Kinv[9];
  double R[9];
  double lensCenterVector[3];

  Channel(CameraArray *array, unsigned int num);
  ~Channel() {
  };
  void setImagePath(const char *imagePath, const char *timestamp);
  void getEQRPrincipalPoint(double *x0, double *y0);
  void eqrToRect_test();
  void getImageDimensions();
  void getRotation();
  void getKinv();
  void getLensCenterVector();
};

void interpolateSubPix(cimg_library::CImg<float> &calib,double (&value)[2], int order, double u, double v);
double phi(int order, double u, double v, int i, int j);


#endif /* CHANNEL_HPP_ */
