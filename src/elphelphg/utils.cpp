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

#include <boost/filesystem.hpp>
/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include <string>
#include <sstream>
*/
#include <regex.h>
#include <string>                                                               
#include <vector>                                                               
#include <boost/regex.hpp>                                                      

#include "utils.hpp"

namespace fs = boost::filesystem;

namespace elphelphg {

namespace utils {

std::string to_string(const int& v) {
	std::ostringstream stm;
	return (stm << v) ? stm.str() : "{*** error ***}";
}

std::string to_string(const double& v) {
	std::ostringstream stm;
	return (stm << v) ? stm.str() : "{*** error ***}";
}

std::string to_string(const int& v, const unsigned int width) {
	std::ostringstream stm;
	std::string result;
	if (stm << v) {
		result = stm.str();
		while (result.length() < width) {
			result = "0" + result;
		}
		return result.c_str();
	} else {
		return "{*** error ***}";
	}
}

std::string basename(const std::string &filename) {
	size_t lastdot = filename.find_last_of(".");
	if (lastdot == std::string::npos) {
		return filename;
	}
	return filename.substr(0, lastdot);
}

/**
 * regexp match regular exception and extract substrings
 * @param re extended POSIX regular expression
 * @param nmatch maximum number of matches
 * @param str string to match
 * @return An array of char pointers. You have to free() every element. The first element is the string itself, then come the matches.
 */
char **regexp(const char *re, int nmatch, const char *str) {
	char **result;
	char *string;
	regex_t regex;
	regmatch_t *match;
	int i;

  // allocate memory for matches
	match = (regmatch_t*) malloc(nmatch * sizeof(*match));
	if (!match) {
		fprintf(stderr, "Out of memory !");
		return NULL;
	}

  // compile regex
	if (regcomp(&regex, re, REG_EXTENDED) != 0) {
		fprintf(stderr, "Failed to compile regex '%s'\n", re);
		return NULL;
	}

  string=strdup(str);
	if (!string) {
		fprintf(stderr, "Out of memory !");
		return NULL;
	}

  // get matches
	if (regexec(&regex, string, nmatch, match, 0)) {
#ifdef DEBUG
		fprintf(stderr, "String '%s' does not match regex '%s'\n",str,re);
#endif
		free(string);
		return NULL;
	}

  // allocate memory for return value (array of char*)
	result = (char**) calloc(nmatch,sizeof(*result));
	if (!result) {
		fprintf(stderr, "Out of memory !");
		free(string);
		return NULL;
	}

  // extract substrings
	for (i = 0; i < nmatch; ++i) {
    regoff_t so=match[i].rm_so;
		if (so >= 0) {
      regoff_t eo=match[i].rm_eo;
      // allocate storage for result and copy substring
      char *storage=(char*)malloc(eo-so+1);
      if (!storage) {
        fprintf(stderr, "Out of memory !");
        return NULL;
      }
      memcpy(storage, string+so, eo-so);
      storage[eo-so]=0;
			((char**)result)[i]=storage;
#ifdef DEBUG
			printf("%s\n",storage);
#endif
		} else {
			((char**) result)[i] = (char*)"";
		}
	}

	result[0] = string;

	return result;

}

/**
 * imagefile_parsename Extract informations from Elphel PHG image file name
 * @param absolutePath Absolute path of the file
 * @param nmatch maximum number of matches
 * @param str string to match
 */
struct imagefile_info *imagefile_parsename(const char *absolutePath) {
	imagefile_info *fileInfo=(imagefile_info *) regexp(IMG_FILENAME_REGEX, 6, absolutePath);
  if (fileInfo) {
    for (int i=0; i<6; ++i) {
      if (((char**)fileInfo)[i]==NULL) {
        throw std::string("Error: unable to parse image absolute path ")+absolutePath;
      }
    }
  } else {
    throw std::string("Error: unable to parse image absolute path ")+absolutePath;
  }
  return fileInfo;
}

void imagefileInfo_dispose(struct imagefile_info *fileInfo) {
  for (int i=0; i<6; ++i) {
    if (((char**)fileInfo)[i]) free(((void**)fileInfo)[i]);
  }
  free(fileInfo);
}

int getFileList(std::vector<std::string> &fileList, const char *directory,
		boost::regex *filter) {
	fs::path dir(directory);
	fs::directory_iterator eoi;

	// boost::regex format(regex?"^[0-9]+\.xml$",boost::regex::icase);
	if (!fs::exists(dir) || !fs::is_directory(dir)) {
		throw std::string("Error: no such directory: ") + directory;
	}

	for (fs::directory_iterator iterator(dir); iterator != eoi; ++iterator) {
		if (fs::is_regular_file(iterator->status())
				&& (!filter
						|| boost::regex_match(iterator->path().string(),
								*filter))) {
			fileList.push_back(iterator->path().string());
		}
	}
	return 0;
}

}
}
