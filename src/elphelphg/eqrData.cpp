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

#include "eqrData.hpp"
#include "xml.hpp"

EqrData::EqrData(const char *path) {
  xmlData=new Xml(path);
  channel=get("channel");
  imageFullLength=get("ImageFullLength")-1;
  imageFullWidth=get("ImageFullWidth");
  latitudeBottom=get("latitudeBottom");
  latitudeTop=get("latitudeTop");
  longitudeLeft=get("longitudeLeft");
  longitudeRight=get("longitudeRight");
  xPosition=get("XPosition");
  yPosition=get("YPosition");
  angularStep=360./imageFullWidth;
}

double EqrData::get(const char *property) {
	std::string expr=std::string("/properties//")+std::string(property);
	return xmlData->getDouble(expr.c_str());
}
