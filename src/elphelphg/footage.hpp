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

#ifndef FOOTAGE_HPP_
#define FOOTAGE_HPP_

#include <string>
#include <vector>
#include <set>

class CImg;

#include "image.hpp"

namespace elphelphg {

class CameraArray;
class Channel;

class Footage {
public:

  // array of channel's image object instances array
  typedef Image **imageListT;  

  // cache to store image object instance lists per channel for this footage
  typedef std::pair<Image::timestampT,imageListT> cacheElemT; 
  typedef std::vector<cacheElemT> cacheT;
  cacheT cache;

  CameraArray *cameraArray;
  std::string directoryPath;

  Footage(
    CameraArray *array,
    const char *directoryPath
  );

  ~Footage() {
  }

  template <typename imageType>
  int getImage(
    const char *timestamp,
    int channel,
    Image::imageType type,
    imageType *image
  );

};

}

#endif
