/**
 *   Murmure - Net-SNMP MIB Versatile Extender
 *   Developed by Christian Visintin
 * 
 * MIT License
 * Copyright (c) 2019 Christian Visintin
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

bool strutils::startsWith(const std::string& haystack, const std::string& needle) {

  if (needle.length() > haystack.length()) {
      return false;
  }

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

bool strutils::endsWith(const std::string& haystack, const std::string& needle) {

  if (needle.length() > haystack.length()) {
      return false;
  }

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
  if (trimmed.length() > 0 && (trimmed.at(0) == 0x20 || trimmed.at(0) == 0x09)) {
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
  size_t lastPos = trimmed.length() > 0 ? trimmed.length() - 1 : 0;
  if (trimmed.length() > 0 && (trimmed.at(lastPos) == 0x20 || trimmed.at(lastPos) == 0x09)) {
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

std::string strutils::substring(const std::string& str, size_t startIndex, size_t endIndex /* = -1 */) {
  std::string out = str;
  return out.substr(startIndex, endIndex - startIndex);
}
