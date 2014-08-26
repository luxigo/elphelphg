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

#ifndef SENSORDATA_HPP_
#define SENSORDATA_HPP_

class Xml;

class SensorData {
  Xml *xmlData;

public:
  const char *sensorDataPath;

  int subCamera;
  int channel;
  double pixelSize;
  // origin
  double radius;
  double azimuth;
  double height;
  double focalLength;
  double entrancePupilForward;
  // sensor pose
  double heading;
  double elevation;
  double roll;

  // sensor principal point
  double px0;
  double py0;
  // radial distortion
  double distortionA;
  double distortionB;
  double distortionC;
  double distortionA5;
  double distortionA6;
  double distortionA7;
  double distortionA8;
  double distortionRadius;
  double pixelCorrectionHeight;
  double pixelCorrectionWidth;
  double pixelCorrectionDecimation;

  SensorData(const char *path);
  ~SensorData(){
  }

  double get(const char *property);
};

#endif /* SENSORDATA_HPP_ */
