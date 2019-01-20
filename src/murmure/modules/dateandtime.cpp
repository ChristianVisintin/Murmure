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

#include <murmure/modules/dateandtime.hpp>
#include <murmure/primitives/octet.hpp>

using namespace murmure;

/**
 * @function DateAndTime
 * @description DateAndTime class constructor
 * NOTE: set primitive to nullptr; set primitiveType to the primitive we'll use
**/

DateAndTime::DateAndTime() {
  primitive = nullptr;
  primitiveType = PRIMITIVE_OCTET;
}

/**
 * @function ~DateAndTime
 * @description DateAndTime class destructor
**/

DateAndTime::~DateAndTime() {
  if (primitive != nullptr) {
    Octet<std::string>* primitivePtr = reinterpret_cast<Octet<std::string>*>(primitive);
    delete primitivePtr;
  }
  primitive = nullptr;
}

/**
 * @function setValue
 * @description initialize primitive and set its value
 * @param std::string
 * @returns bool
 * NOTE: must be called in initialization only
**/

bool DateAndTime::setValue(std::string value) {
  if (primitive == nullptr) {
    primitive = new Octet<uint8_t*>(value);
  }
  return true;
}

/**
 * @function setValue
 * @description update primitive value associated to this module
 * @param std::string
 * @param std::string
 * @returns bool
**/

bool DateAndTime::setValue(std::string oid, std::string value) {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return false;
  }
  //Check length for DateAndTime (must be 8 bytes or 11 bytes @! we use ascii repr of bytes, so use doubled values)
  if (value.length() != 16 && value.length() != 22) {
    return false;
  }
  Octet<uint8_t*>* primitivePtr = reinterpret_cast<Octet<uint8_t*>*>(primitive);
  return primitivePtr->setValue(oid, value);
}

/**
 * @function getPrintableValue
 * @description get printable value from its primitive
 * @returns std::string
**/

std::string DateAndTime::getPrintableValue() {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return "";
  }
  Octet<uint8_t*>* primitivePtr = reinterpret_cast<Octet<uint8_t*>*>(primitive);
  return primitivePtr->getPrintableValue();
}

/**
 * @function getPrimitiveType
 * @description get primitive type name associated to this module
 * @returns std::string
**/

std::string DateAndTime::getPrimitiveType() {
  return primitiveType;
}
