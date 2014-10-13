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

#include <string>
#ifndef cimg_version
#define cimg_use_tiff
#define cimg_display 0
#include <CImg.h>
#endif

#include "utils.hpp"
#include "footage.hpp"
#include "cameraArray.hpp"
#include "tile.hpp"

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
 * Footage::getTile get a Tile object from cache, or instantiate one
 * @param timestamp Image timestamp
 * @param channel Channel index
 * @param type Image type
 */
void Footage::getTile(const char *timestamp, int channel, Tile **tile) {

  Footage *footage=this;
  std::string t(timestamp);

  // assert channel number
  if ((unsigned int)channel>=footage->cameraArray->channel_list.size()) {
    throw std::string("Invalid channel number " + to_string(channel,2));
  }

  // find timestamp in footage tileList cache
  tileListT tileList=NULL;
  for (cacheT::iterator cacheElem=footage->cache.begin(); cacheElem!=footage->cache.end(); ++cacheElem) {

    if (cacheElem[0].first!=t) {
      continue;
    }

    tileList=cacheElem[0].second;

    if (!tileList[channel]) {
      // no Tile object for channel in cached tileList 
      // instantiate and cache Tile for this timestamp/channel pair
      tileList[channel]=new Tile(footage,timestamp,channel);
    }

    break;
  }

  if (!tileList) {
    // timestamp not found in cache
   
    // instantiate tileList for this timestamp
    tileList=new Tile*[footage->cameraArray->channel_list.size()]();
    cacheElemT cache_entry=std::make_pair(timestamp,tileList);

    // add to cache
    footage->cache.push_back(cache_entry);

    // instantiate channel Tile object for this timestamp/channel pair
    tileList[channel]=new Tile(footage,timestamp,channel);
  }

  *tile=tileList[channel];

} // Footage::getTile

/**
 * Footage::getImage Get a given image from this footage
 * @param timestamp Image timestamp
 * @param channel Channel index
 * @param type Image type
 * @param image Pointer to requested image
 */
template <typename imageType>
void Footage::getImage(const char *timestamp, int channel, Tile::tileType type, imageType *image) {

  Footage *footage=this;

  Tile *tile;
  footage->getTile(timestamp,channel,&tile);

  // return the requested image pointer
  tile->getImage(type,image);

} // Footage::getImage

} // namespace elphelphg
