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

#include "xml.hpp"
#include "file.hpp"

#include <string.h>

Xml::Xml(const char *data, long length) {
 doc=xmlReadMemory(data,length, "", NULL, 0);
 if (doc == NULL) {
   throw "Xml::parse - Failed to parse data";
 }
 ctx=xmlXPathNewContext(doc);
}

Xml::Xml(const char *filename) {
  File f(filename);
 // doc=xmlReadFile(filename, "UTF-8", 0);
  doc=xmlReadMemory(f.content,f.length,"",NULL,0);
  if (doc == NULL) {
  	throw "Xml::load - Cannot parse "+std::string(filename);
  }
  ctx=xmlXPathNewContext(doc);
}

double Xml::getDouble(const char *expr) {
  xmlXPathObjectPtr obj;
  obj=xmlXPathEvalExpression((xmlChar*)expr, ctx);
  if(obj == NULL || obj->type!=XPATH_NODESET) {
      throw "Xml::eval: unable to evaluate xpath expression "+std::string(expr);
  }
  double ret=xmlXPathCastNodeSetToNumber(obj->nodesetval);
  xmlXPathFreeObject(obj);
  return ret;
}

std::string Xml::getString(const char *expr) {
  xmlXPathObjectPtr obj;
  obj=xmlXPathEvalExpression((xmlChar*)expr, ctx);
  if(obj == NULL || obj->type!=XPATH_NODESET) {
      throw "Xml::eval: unable to evaluate xpath expression "+std::string(expr);
  }
  const unsigned char *stringval=xmlXPathCastNodeSetToString(obj->nodesetval);
  std::string ret((const char*)stringval);
  xmlXPathFreeObject(obj);
  return ret;
}
