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

#include <murmure/primitives/integer.hpp>
#include <utils/databasefacade.hpp>
#include <utils/logger.hpp>

#include <sstream>

#define COMPONENT "OID"

using namespace murmure;

/**
 * @function Integer
 * @description Integer class constructor
 * @param std::string value to convert to primitive
**/

Integer<int>::Integer(std::string value) {
  this->value = std::stoi(value);
}

/**
 * @function setValue
 * @description save new value on database and set new value to object
 * @param std::string oid associated to this value
 * @returns bool: true if set database operation succeeded
**/

bool Integer<int>::setValue(std::string oid, std::string value) {
  std::string errorString;

  //Open database
  if (!database::open(&errorString)) {
    //Database open failed
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  //Get value to set
  int newValue = std::stoi(value);
  //Build query string sstream
  std::stringstream queryStream;
  queryStream << "UPDATE oids SET value = \"" << newValue << "\" WHERE oid = \"" << oid << "\";";
  std::string query = queryStream.str();


  if (!database::exec(query, &errorString)) {
    //Database query failed
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  //Query succeeded, update oid value
  this->value = newValue;

  //try to close database
  if (!database::close(&errorString)) {
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  return true;

}

/**
 * @function getValue
 * @returns int: real primitive value
**/

int Integer<int>::getValue() {
  return this->value;
}

/**
 * @function getPrintableValue
 * @description get printable value version
 * @returns std::string
**/

std::string Integer<int>::getPrintableValue() {

  return std::to_string(this->value);
}