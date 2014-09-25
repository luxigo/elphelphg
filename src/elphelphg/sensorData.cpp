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
#include "sensorData.hpp"
#include "xml.hpp"

namespace elphelphg {

SensorData::SensorData(const char *path) {
  xmlData=new Xml(path);
  subCamera=get("subcamera");
  channel=get("channel");
  pixelSize=get("pixelSize");
  radius=get("radius");
  azimuth=get("azimuth");
  height=get("height");
  focalLength=get("focalLength");
  entrancePupilForward=get("entrancePupilForward");
  heading=get("heading");
  elevation=get("elevation");
  roll=get("roll");
  px0=get("px0");
  py0=get("py0");
  distortionA=get("distortionA");
  distortionB=get("distortionB");
  distortionC=get("distortionC");
  distortionA5=get("distortionA5");
  distortionA6=get("distortionA6");
  distortionA7=get("distortionA7");
  distortionA8=get("distortionA8");
  distortionRadius=get("distortionRadius");
  pixelCorrectionHeight=get("pixelCorrectionHeight");
  pixelCorrectionWidth=get("pixelCorrectionWidth");
  pixelCorrectionDecimation=get("pixelCorrectionDecimation");
}

double SensorData::get(const char *property) {
	std::string expr=std::string("/properties//")+property;
  return xmlData->getDouble(expr.c_str());
}

}
