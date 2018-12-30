/**
 *  Murmure - Net-SNMP MIB Versatile Extender
 *  Developed by Christian Visintin
 * 
 * 	Copyright (C) 2018 - 2019 Christian Visintin
 *
 *	This file is part of "Murmure"
 *
 *  Murmure is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  Murmure is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
**/

#include <utils/strutils.hpp>

#include <sstream>

using namespace strutils;

/**
 * @function split
 * @description split std::string into vector of string dividing each token using delimiter
 * @param std::string string to split
 * @param char delimiter
 * @returns std::vector<std::string>
**/

std::vector<std::string> strutils::split(const std::string& s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

/**
 * @function startsWith
 * @description check if a string starts with a certain string
 * @param std::string haystack
 * @param std::string needle
 * @returns bool: true if haystack starts with needle
**/

bool strutils::startsWith(const std::string& haystack, std::string needle) {

  std::string startString = haystack.substr(0, needle.length());
  return startString == needle;
}

/**
 * @function endsWith
 * @description check if a string ends with a certain string
 * @param std::string haystack
 * @param std::string needle
 * @returns bool: true if haystack ends with needle
**/

bool strutils::endsWith(const std::string& haystack, std::string needle) {

  std::string endString = haystack.substr(haystack.length() - needle.length(), needle.length());
  return endString == needle;
}

/**
 * @function itrim
 * @description trim multiple spaces (more than one) inside a string
 * @param std::string
 * @returns std::string trimmed string
**/

std::string strutils::itrim(const std::string& haystack) {

  std::string trimmed = haystack;

  size_t multiBlankPos = haystack.find("  "); //Two spaces
  if (multiBlankPos == std::string::npos) {
    return trimmed;
  } else {
    //Create string without a space
    return strutils::itrim(trimmed.erase(multiBlankPos, 1));
  }

}

/**
 * @function ltrim
 * @description trim left side of provided string
 * @param std::string
 * @returns std::string trimmed string
**/

std::string strutils::ltrim(const std::string& haystack) {
  std::string trimmed = haystack;
  //Recursive call for ltrim
  if (trimmed.at(0) == 0x20) {
    return strutils::ltrim(trimmed.substr(1));
  }
  return trimmed;
}

/**
 * @function rtrim
 * @description trim right side of provided string
 * @param std::string
 * @returns std::string trimmed string
**/

std::string strutils::rtrim(const std::string& haystack) {
  std::string trimmed = haystack;
  //Recursive call for ltrim
  size_t lastPos = trimmed.length() - 1;
  if (trimmed.at(lastPos) == 0x20) {
    return strutils::rtrim(trimmed.substr(0, lastPos));
  }
  return trimmed;
}

/**
 * @function ltrim
 * @description trim both sides of provided string
 * @param std::string
 * @returns std::string trimmed string
**/

std::string strutils::trim(const std::string& haystack) {
  std::string trimmed = haystack;
  trimmed = strutils::ltrim(trimmed);
  trimmed = strutils::rtrim(trimmed);
  return trimmed;
}

/**
 * @function substring
 * @description Returns a new string that is a substring of str. The new string is made up of the character of str between beginIndex and endIndex
 * @param std::string str
 * @param size_t startIndex
 * @param size_t endIndex
 * @returns strd::string substring
**/

std::string strutils::substring(std::string str, size_t startIndex, size_t endIndex /* = -1 */) {
  return str.substr(startIndex, endIndex - startIndex);
}
