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

#include <opencv/cv.h>                    
#include <opencv2/highgui/highgui_c.h>    
#include <opencv2/imgcodecs/imgcodecs_c.h>                                                                                                                    

#ifndef cimg_version
#define cimg_use_tiff
#define cimg_display 0
#include <CImg.h>
#endif         

#define IMAGE_FILE_EXTENSION

namespace elphelphg {

class Footage;

class Tile {
public:

  typedef std::string timestampT;
  typedef int channelIndexT;

  typedef enum {
    SENSOR,
    EQR,
    RECT_SENSOR,
    RECT_CONFOC,
    tileTypeCount

  } tileType;

  const char *tileTypeStr[tileTypeCount]={
    "DECONV-RGB24_EQR",
    "RECT-SENSOR",
    "RECT-CONFOC"
  };

  Footage *footage;
  timestampT timestamp;
  channelIndexT channel;
  std::string file_extension;

  Tile(Footage *footage, timestampT timestamp, channelIndexT index);

  ~Tile() {
  }

  std::string getFilename(tileType type);

  cimg_library::CImg<uint8_t> *get(tileType type);

  template <typename imagePointer>
  int get(tileType type, imagePointer *image);

  template <typename imagePointer>
  int load(std::string &filename, imagePointer *image);
  int load(std::string &filename, IplImage **image);
  int load(std::string &filename, cimg_library::CImg<uint8_t> **image);

  int save(std::string &filename, IplImage *image);
  int save(std::string &filename, cimg_library::CImg<uint8_t> *image);

  template <typename imagePointer>
  int convertTo(tileType type, imagePointer *image);

  int convertTo(tileType type, std::string &filename);

};

};

#endif
