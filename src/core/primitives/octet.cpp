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

#include <murmure/primitives/octet.hpp>
#include <utils/databasefacade.hpp>
#include <utils/logger.hpp>

#include <sstream>

#define COMPONENT "OID"

using namespace murmure;

/**
 * @function valueToHex
 * @description convert Hex string representation into real ascii value buffer
 * @param std::string ascii: ascii representation to convert
 * @returns void
**/

template <>
void Octet<uint8_t*>::valueToHex(std::string ascii) {

  size_t asciiLength = ascii.length();
  const char* asciiBuf = ascii.c_str();
  for (size_t i = 0; i < asciiLength; i++) {
    char digit[3];
    digit[0] = *(asciiBuf++);
    digit[1] = *(asciiBuf++);
    digit[2] = '\0';
    this->value[i] = static_cast<uint8_t>(strtol(digit, NULL, 16));
  }
}

/**
 * @function valueToAscii
 * @description convert uint8_t buffer (real hex) to Hex ascii representation string
 * @returns std::string hex representation
**/

template <>
std::string Octet<uint8_t*>::valueToAscii() {

  if (this->value == nullptr) {
    return "";
  }

  char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
  std::string hexRepr(this->dataLength * 2, ' ');
  //s.reserve(len * 2);
  for (size_t i = 0; i < this->dataLength; i++) {
    hexRepr[2 * i] = hexmap[(this->value[i] & 0xF0) >> 4];
    hexRepr[2 * i + 1] = hexmap[this->value[i] & 0x0F];
  }
  return hexRepr;
}


/**
 * @function Octet
 * @description Octet class constructor
 * @param std::string value to convert to primitive
**/

template <>
Octet<uint8_t*>::Octet(std::string value) {

  //Init value to nullptr
  this->value = nullptr;
  //data Length is value's length / 2 (because is ascii representation)
  this->dataLength = value.length() / 2;
  //Allocate value space
  this->value = new uint8_t[dataLength];
  //Convert value to byte buffer
  valueToHex(value);
}

/**
 * @function ~Octet
 * @description Octect class destructor
**/

template <>
Octet<uint8_t*>::~Octet() {

  //Free value
  if (this->value != nullptr) {
    delete[] this->value;
  }
}

/**
 * @function setValue
 * @description save new value on database and set new value to object
 * @param std::string oid associated to this value
 * @returns bool: true if set database operation succeeded
**/

template <>
bool Octet<uint8_t*>::setValue(std::string oid, std::string value) {

  std::string errorString;

  //Open database
  if (!database::open(&errorString)) {
    //Database open failed
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  //Build query string sstream
  std::stringstream queryStream;
  //NOTE: value is already ASCII representation
  queryStream << "UPDATE oids SET value = \"" << value << "\" WHERE oid = \"" << oid << "\";";
  std::string query = queryStream.str();

  if (!database::exec(query, &errorString)) {
    //Database query failed
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  //Query succeeded, update oid value

  //Delete old value
  if (this->value != nullptr) {
    delete[] this->value;
  }
  this->dataLength = value.length() / 2;
  //Allocate value space
  this->value = new uint8_t[dataLength];
  //Convert value to byte buffer
  valueToHex(value);

  //try to close database
  if (!database::close(&errorString)) {
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  return true;
}

/**
 * @function getValue
 * @returns uint8_t*: real primitive value
**/

template <>
uint8_t* Octet<uint8_t*>::getValue() {
  return this->value;
}

/**
 * @function getPrintableValue
 * @description get printable value version
 * @returns std::string
**/

template <>
std::string Octet<uint8_t*>::getPrintableValue() {

  return valueToAscii();
}

