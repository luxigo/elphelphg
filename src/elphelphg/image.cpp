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

using namespace cv;

namespace elphelphg {

/** Image::Image Image constructor
 * @param footage footage pointer
 * @param timestamp image timestamp
 * @param channelIndex channel index
 */
Image::Image(Footage *footage, timestampT timestamp, channelIndexT channelIndex) {
  this->footage=footage;
  this->timestamp=timestamp;
  this->channel=channelIndex;
  this->file_extension=std::string(IMAGE_FILE_EXTENSION);
}

/** Image::getFilename get filename for specified image type
 * @param type image type, see image.hpp
 * @return image file name
 */
std::string Image::getFilename(imageType type) {
  return std::string(
    this->footage->directoryPath + "/" +
    this->timestamp +
    "-" + utils::to_string(this->channel,2) + "-" +
    this->imageTypeStr[type] + "." + this->file_extension
  );

};

/** Image::get return specified image type
 * @param type image type, see image.hpp
 * @param image pointer to return image pointer
 */
template <typename imagePointer>
int Image::get(imageType type, imagePointer *image){

  std::string &filename=this->getFilename(type);

  // convert image if needed
  if (!utils::exists(filename.c_str())) {
    return this->convertTo(type, image);
  }

  return this->load(filename, image);
}

/** Image::load return specified image type
 * @param filename image file name
 * @param image pointer to return image pointer
 * @return non null on error
 */
int Image::load(std::string &filename,IplImage **image){
  *image=cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_COLOR);
  return *image!=NULL;
}

int Image::load(std::string &filename, cimg_library::CImg<uint8_t> **image) {
  *image=new cimg_library::CImg<uint8_t>(filename.c_str());
  return *image!=NULL;
}

/** Image::save save image
 * @param filename image file name
 * @param image pointer to image
 * @return on error return non-null or throw an exception
 */
int Image::save(std::string &filename,IplImage *image){
  return cvSaveImage(filename.c_str(), image);
}

int Image::save(std::string &filename, cimg_library::CImg<uint8_t> *image) {
  image->save(filename.c_str());
  return 0;
}

/** Image::convertTo compute specified image type
 * @param type target image type, see image.hpp
 * @return non null on error
 */
template <typename imagePointer>
int Image::convertTo(imageType type, imagePointer *image){

  std::string &filename=this->getFilename(type);

  switch(type) {
  case SENSOR:
  case EQR:
  case RECT_SENSOR:
  case RECT_CONFOC:
  default:
    throw std::string("error: conversion from SENSOR to EQR needs to be implemented");
    break;
  }
  return 0;
} 

/** Image::convertTo compute and save the specified image type
 * @param type target image type, see image.hpp
 * @return non null on error
 */
int Image::convertTo(imageType type, std::string &filename) {
  IplImage *image;
  this->get(type,image);
  return this->save(filename,image);
}

} // namespace elphelphg

