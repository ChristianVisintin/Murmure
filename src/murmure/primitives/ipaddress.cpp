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

#include <murmure/primitives/ipaddress.hpp>
#include <utils/databasefacade.hpp>
#include <utils/logger.hpp>

#include <sstream>

#define COMPONENT "OID"

using namespace murmure;

/**
 * @function IPAddress
 * @description IPAddress class constructor
 * @param std::string value to convert to primitive
**/

IPAddress<std::string>::IPAddress(std::string value) {
  this->value = value;
}

/**
 * @function setValue
 * @description save new value on database and set new value to object
 * @param std::string oid associated to this value
 * @returns bool: true if set database operation succeeded
**/

bool IPAddress<std::string>::setValue(std::string oid, std::string value) {
  std::string errorString;

  //Open database
  if (!database::open(&errorString)) {
    //Database open failed
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  //Build query string sstream
  std::stringstream queryStream;
  queryStream << "UPDATE oids SET value = \"" << value << "\" WHERE oid = \"" << oid << "\";";
  std::string query = queryStream.str();

  if (!database::exec(query, &errorString)) {
    //Database query failed
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  //Query succeeded, update oid value
  this->value = value;

  //try to close database
  if (!database::close(&errorString)) {
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  return true;
}

/**
 * @function getValue
 * @returns std::string: real primitive value
**/

std::string IPAddress<std::string>::getValue() {
  return this->value;
}

/**
 * @function getPrintableValue
 * @description get printable value version
 * @returns std::string
**/

std::string IPAddress<std::string>::getPrintableValue() {

  return this->value;
}