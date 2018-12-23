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

#include <murmure/primitives/octet.hpp>
#include <utils/databasefacade.hpp>
#include <utils/logger.hpp>

#include <sstream>

#define COMPONENT "OID"

using namespace murmure;

/**
 * @function Octet
 * @description Octet class constructor
 * @param std::string value to convert to primitive
**/

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

uint8_t* Octet<uint8_t*>::getValue() {
  return this->value;
}

/**
 * @function getPrintableValue
 * @description get printable value version
 * @returns std::string
**/

std::string Octet<uint8_t*>::getPrintableValue() {

  return valueToAscii();
}

/**
 * @function valueToHex
 * @description convert Hex string representation into real ascii value buffer
 * @param std::string ascii: ascii representation to convert
 * @returns void
**/

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
