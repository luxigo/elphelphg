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

#ifndef CAMERAARRAY_HPP_
#define CAMERAARRAY_HPP_

#include <vector>

namespace elphelphg {

  class ImageJ_Elphel_Preferences;
class Camera;
class Channel;

class CameraArray {
public:

  typedef enum {
    EYESIS4PI_CAMERA=1,
    PHG3_CAMERA
  } camArrayType;

  camArrayType type;

  std::vector<Camera*> camera_list;
  std::vector<Channel*> channel_list;

  ImageJ_Elphel_Preferences *prefs;

  CameraArray(camArrayType type,const char *imagej_elphel_prefs_path);
  ~CameraArray();

  void init();

  Camera *camera(int cam);
  Channel *channel(int chan);

};

}

#endif /* CAMERAARRAY_HPP_ */
