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

#ifndef __IMAGE_HPP
#define __IMAGE_HPP

#include <string>

#ifndef cimg_version
#define cimg_use_tiff
#define cimg_display 0
#include <CImg.h>
#endif         

#define IMAGE_FILE_EXTENSION

namespace elphelphg {

class Footage;

class Image {
public:

  typedef std::string timestampT;
  typedef int channelIndexT;

  typedef enum {
    EQR,
    GNO
  } imageType;

  const char *imageTypeStr[2]={
    "DECONV-RGB24_EQR",
    "DECONV-RGB24_EQR_GNO"
  };

  Footage *footage;
  timestampT timestamp;
  channelIndexT channel;
  std::string file_extension;

  Image(Footage *footage, timestampT timestamp, channelIndexT index);

  ~Image() {
  }

  cimg_library::CImg<uint8_t> *get(imageType type);

};

}

#endif
