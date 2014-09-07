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

#include "imagej_elphel_preferences.hpp"
#include "xml.hpp"
#include "utils.hpp"

ImageJ_Elphel_Preferences::ImageJ_Elphel_Preferences(const char *path) {
	this->xmlData = new Xml(path);
	this->calibtiff_dir = this->getString(
			"CORRECTION_PARAMETERS.sensorDirectory");
	this->calibtiff_prefix = this->getString(
			"CORRECTION_PARAMETERS.sensorPrefix");
	this->calibtiff_suffix = this->getString(
			"CORRECTION_PARAMETERS.sensorSuffix");
	this->eqrtiff_dir = this->getString(
			"CORRECTION_PARAMETERS.equirectangularDirectory");
	this->eqrtiff_prefix = this->getString(
			"CORRECTION_PARAMETERS.equirectangularPrefix");
	this->eqrtiff_suffix = this->getString(
			"CORRECTION_PARAMETERS.equirectangularSuffix");
  this->cameraCount = (int)this->getDouble(
      "CAMERAS.cameraIPs.length");
  this->channelCount = (int)this->getDouble(
      "CAMERAS.channelMap.length");

  this->channel_cameraIndex = new int[this->channelCount];
  this->channel_subchannel = new int[this->channelCount];
  for (int chan=0; chan<this->channelCount; ++chan) {
    this->channel_cameraIndex[chan]=(int)this->getDouble(
      (std::string("CAMERAS.channelMap_")+utils::to_string(chan)+"_IPindex").c_str());
    this->channel_subchannel[chan]=(int)this->getDouble(
      (std::string("CAMERAS.channelMap_")+utils::to_string(chan)+"_subchannel").c_str());
  }
}

std::string ImageJ_Elphel_Preferences::getString(const char *key) {
	return this->xmlData->getString(
			(std::string("/properties/entry[@key=\"") + key + "\"]").c_str());
}

double ImageJ_Elphel_Preferences::getDouble(const char *key) {
	return this->xmlData->getDouble(
			(std::string("/properties/entry[@key=\"") + key + "\"]").c_str());
}
