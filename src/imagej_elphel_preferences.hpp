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

#ifndef IMAGEJ_ELPHEL_PREFERENCES_HPP_
#define IMAGEJ_ELPHEL_PREFERENCES_HPP_

#include <string>
class Xml;

class ImageJ_Elphel_Preferences {
public:
	Xml *xmlData;
	std::string calibtiff_dir;
	std::string calibtiff_prefix;
	std::string calibtiff_suffix;
	std::string eqrtiff_dir;
	std::string eqrtiff_prefix;
	std::string eqrtiff_suffix;
  int cameraCount;
  int channelCount;
  int *channel_cameraIndex;
  int *channel_subchannel;

	ImageJ_Elphel_Preferences(const char *preferences_path);
	~ImageJ_Elphel_Preferences() {
	};

	std::string getString(const char *key);
	double getDouble(const char *key);
};


#endif /* IMAGEJ_ELPHEL_PREFERENCES_HPP_ */
