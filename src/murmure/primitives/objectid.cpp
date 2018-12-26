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

#include <murmure/primitives/objectid.hpp>
#include <utils/databasefacade.hpp>
#include <utils/logger.hpp>

#include <sstream>

#define COMPONENT "OID"

using namespace murmure;

/**
 * @function Objectid
 * @description Objectid class constructor
 * @param std::string value to convert to primitive
**/

Objectid<std::string>::Objectid(std::string value) {
  this->value = value;
}

/**
 * @function setValue
 * @description save new value on database and set new value to object
 * @param std::string oid associated to this value
 * @returns bool: true if set database operation succeeded
**/

bool Objectid<std::string>::setValue(std::string oid, std::string value) {
  //Has no value, just return
  return true;
}

/**
 * @function getValue
 * @returns std::string: object id has no value
**/

std::string Objectid<std::string>::getValue() {
  return "";
}

/**
 * @function getPrintableValue
 * @description get printable value version
 * @returns std::string
**/

std::string Objectid<std::string>::getPrintableValue() {

  return "";
}
