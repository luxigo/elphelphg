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

#include <stdio.h>
#include <string>

#include "file.hpp"

 File::File(const char *path) {
	this->path=std::string(path);
  FILE *f=fopen(path,"r");
  if (!f) {
    throw "Unable to open file: "+this->path;
  }
  if (fseek(f,0,SEEK_END)) {
    throw "fseek failed !";
  }
  long length=ftell(f);
  if (length<0) {
    throw("ftell failed !");
  }
  if (fseek(f,0,SEEK_SET)) {
    throw "fseek failed !";
  }
  content=new char[length+1];
  size_t count=fread(content,1,(size_t)length,f);
  if (count<(size_t)length) {
    delete content;
    throw "file read error: "+this->path;
  }
}
