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

// Author(s):
//
//     Luc Deschenaux <l.deschenaux@foxel.ch>
//

#include "utils.hpp"
#include "footage.hpp"

#ifndef cimg_version
#define cimg_use_tiff
#define cimg_display 0
#include <CImg.h>
#endif

namespace elphelphg {

using namespace utils;

/**
 * Footage constructor
 * @param array CameraArray pointer
 * @param directoryPath Footage directory path
 * @return Footage object
 */
Footage::Footage(CameraArray *array, const char *directoryPath) {
  this->cameraArray=array;
  this->directoryPath=directoryPath;
}

/**
 * Get a given image from this footage
 * @param timestamp Image timestamp
 * @param channel Channel index
 * @param type Image type
 * @return Requested image
 */
cimg_library::CImg<uint8_t> *Footage::getImage(const char *timestamp, int channel,Image::imageType type) {
  Footage *footage=this;
  std::string t(timestamp);
  if (channel>=footage->cameraArray.channel_list.length()) {
    throw std::string("Invalid channel number " + to_string(channel,2));
  }
  ImageListT imageList=NULL;
  for (cacheT::iterator cacheElem=footage->cache.begin(); cacheElem!=footage->cache.end(); ++cacheElem) {
    if (cacheElem[0].first()!=t) {
      continue;
    }
    imageList=cacheElem[0].second();
    if (!imageList[channel]) {
      imageList[channel]=new Image(footage,timestamp,channel);
    }
    break;
  }
  if (!imageList[channel]) {
    imageList=new Image*[footage->cameraArray.channel_list.length()]();
    imageList[channel]=new Image(footage,timestamp,channel);
  }
  return imageList[channel]->get(type);
}

}
