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

#ifndef cimg_version
#define cimg_use_tiff
#define cimg_display 0
#include <CImg.h>
#endif
 
#include "image.hpp"
#include "footage.hpp"
#include "utils.hpp"

namespace elphelphg {

Image::Image(Footage *footage, timestampT timestamp, channelIndexT index) {
  this->footage=footage;
  this->timestamp=timestamp;
  this->channel=index;
  this->file_extension=std::string(IMAGE_FILE_EXTENSION);

};

cimg_library::CImg<uint8_t> *Image::get(imageType type){
  std::string filename(
    this->footage->directoryPath + "/" +
    this->timestamp +
    "-" + utils::to_string(this->channel,2) + "-" +
    this->imageTypeStr[type] + "." + this->file_extension
  );
  return new cimg_library::CImg<uint8_t>(filename.c_str());
}

}


