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
 
#include "tile.hpp"
#include "footage.hpp"
#include "utils.hpp"

using namespace cv;

namespace elphelphg {

/** Tile::Tile Tile constructor
 * @param footage footage pointer
 * @param timestamp tile timestamp
 * @param channelIndex channel index
 */
Tile::Tile(Footage *footage, timestampT timestamp, channelIndexT channelIndex) {
  this->footage=footage;
  this->timestamp=timestamp;
  this->channel=channelIndex;
  this->file_extension=std::string(IMAGE_FILE_EXTENSION);
}

/** Tile::getFilename get filename for specified tile type
 * @param type tile type, see image.hpp
 * @return tile file name
 */
void Tile::getFilename(tileType type,std::string &filename) {
  Tile *tile=this;
  filename.assign(
    std::string(tile->footage->directoryPath) +
    tile->timestamp +
    "-" + utils::to_string(tile->channel,2) + "-" +
    tile->tileTypeStr[type] + "." + tile->file_extension
  );
};

/** Tile::getImage load return specified tile image type
 * @param type tile image type, see tile.hpp
 * @param image pointer to requested image pointer
 */
template <typename imagePointer>
void Tile::getImage(tileType type, imagePointer *image){

  Tile *tile=this;

  if  (tile->image[type]) {
    getImageFromCache(type,image);
    return;
  }

  std::string filename;
  tile->getFilename(type,filename);

  // convert tile if needed
  if (!utils::exists(filename.c_str())) {
    tile->convertTo(type, image);
    return;
  }

  tile->loadImage(type, filename, image);
}

/** Tile::loadImage load specified image
 * @param filename image file name
 * @param image pointer to requested image pointer
 * @return non null on error
 */
void Tile::loadImage(tileType type, std::string &filename,IplImage **image){
  *image=cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_COLOR);
  if (this->image[type]) delete this->image[type];
  this->image[type]=new imageT(IPLIMAGE,*image);
}

void Tile::loadImage(tileType type, std::string &filename, cimg_library::CImg<uint8_t> **image) {
  *image=new cimg_library::CImg<uint8_t>(filename.c_str());
  if (this->image[type]) delete this->image[type];
  this->image[type]=new imageT(CIMG_uint8,*image);
}

void Tile::getImageFromCache(tileType type, IplImage **image) {
  if (this->image[type]->first!=IPLIMAGE) {
    throw std::string("trying to read wrong image format from cache");
  }
  *image=(IplImage*)this->image[type]->second;
}

void Tile::getImageFromCache(tileType type, cimg_library::CImg<uint8_t> **image) {
  if (this->image[type]->first!=CIMG_uint8) {
    throw std::string("trying to read wrong image format from cache");
  }
  *image=(cimg_library::CImg<uint8_t>*)this->image[type]->second;
}

/** Tile::saveImage save image
 * @param filename image file name
 * @param image pointer to image
 * @return on error return non-null or throw an exception
 */
int Tile::saveImage(std::string &filename,IplImage *image){
  return cvSaveImage(filename.c_str(), image);
}

int Tile::saveImage(std::string &filename, cimg_library::CImg<uint8_t> *image) {
  image->save(filename.c_str());
  return 0;
}

/** Tile::convertTo compute specified tile type
 * @param type target tile type, see tile.hpp
 * @return non null on error
 */
template <typename imagePointer>
int Tile::convertTo(tileType type, imagePointer *image){

  Tile *tile=this;
  std::string filename;

  tile->getFilename(type,filename);

  switch(type) {
  case SENSOR:
  case EQR:
  case RECT_SENSOR:
  case RECT_CONFOC:
  default:
    image=image;
    throw std::string("error: tile conversion requested needs to be implemented");
    break;
  }
  return 0;
} 

/** Tile::convertTo compute and save the specified tile type
 * @param type target tile type, see tile.hpp
 * @return non null on error
 */
int Tile::convertTo(tileType type, std::string &filename) {
  Tile *tile=this;
  IplImage *image;

  tile->getImage(type,&image);

  return this->saveImage(filename,image);
}

} // namespace elphelphg

