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

#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <string>
#include <vector>
#include <boost/regex.hpp>

#ifndef PI
#define PI 3.1415926535897932384626433832795028841971693993751058209
#endif

inline double rad(double x) {
  return x*PI/180.;
}

inline double deg(double x) {
  return x*180./PI;
}

namespace elphelphg {
namespace utils {

// dir, timestamp, channel, attributes list, extension
#define IMG_FILENAME_REGEX "^(.*/)?([0-9]+_[0-9]+)\\-([0-9]+)\\-([^\\.]+)\\.(.*)$"
#define IMG_FILENAME_REGEX_NMATCH 6

  typedef struct imagefile_info {
    char *filePath;
    char *dir;
    char *timestamp;
    char *channel;
    char *attributes;
    char *extension;
  } imagefile_info;

//  template < typename T > std::string to_string( const T& v );
  std::string to_string(const int& v);
  std::string to_string(const double& v);
  std::string to_string(const int& v, const unsigned int width);
  std::string basename(const std::string &filename);

  char **regexp(const char *re, int nmatch, const char *str);
  struct imagefile_info *imagefile_parsename(const char *filePath);
  void imagfileInfo_dispose(struct imagefile_info *fileInfo);
  int getFileList(std::vector<std::string> &fileList,const char *directory, boost::regex *filter);

}
}
#endif /* UTILS_HPP_ */
