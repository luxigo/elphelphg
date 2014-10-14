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

#include <vector>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "cameraArray.hpp"
#include "camera.hpp"
#include "channel.hpp"
#include "imagej_elphel_preferences.hpp"
#include "utils.hpp"
#include "sensorData.hpp"

namespace elphelphg {

/**  
 * CameraArray constructor
 * @param type camera Array Type
 * @param prefs_path imagej xml preferences path
 */  
CameraArray::CameraArray(camArrayType type,const char *prefs_path) {
	this->type=type;
	this->prefs=new ImageJ_Elphel_Preferences(prefs_path);
	this->init();
}

/**  
 * CameraArray destructor
 */  
CameraArray::~CameraArray(){
  delete prefs;
	for (std::vector<Channel*>::iterator channel=channel_list.begin(); channel!=channel_list.end(); ++channel) {
    delete channel[0];
  }
	for (std::vector<Camera*>::iterator camera=camera_list.begin(); camera!=camera_list.end(); ++camera) {
    delete camera[0];
  }
}

/**
 * CameraArray::init
 * Initialize channels (sensors) and cameras
 */
void CameraArray::init() {
  ImageJ_Elphel_Preferences *prefs=this->prefs;
	// Init channels
  for (int chan=0; chan<prefs->channelCount; ++chan) {
    this->channel_list.push_back(new Channel(this,chan));
  }
  // Init cameras
  for (int i=0; i<prefs->cameraCount; ++i) {
    this->camera_list.push_back(new Camera(this,i));
  }
}

/**
 * CameraArray::camera
 * @param num camera index
 * @return Camera object pointer
 */
Camera *CameraArray::camera(int num) {
	for (std::vector<Camera*>::iterator camera=camera_list.begin(); camera!=camera_list.end(); ++camera) {
		if (camera[0]->num==num) {
			return camera[0];
		}
	}
	throw std::string("No such camera: ")+utils::to_string(num,2);
  return NULL;
}

/**
 * CameraArray::channel
 * @param num channel index
 * @return Channel object pointer
 */
Channel *CameraArray::channel(int num) {
	for (std::vector<Channel*>::iterator channel=channel_list.begin(); channel!=channel_list.end(); ++channel) {
		if (channel[0]->sensor->channel==num) {
			return channel[0];
		}
	}
	throw std::string("No such channel: ")+utils::to_string(num,2);
  return NULL;
}

}
