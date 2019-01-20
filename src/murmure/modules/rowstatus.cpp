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

#include <murmure/modules/rowstatus.hpp>
#include <murmure/primitives/string.hpp>

using namespace murmure;

/**
 * @function RowStatus
 * @description RowStatus class constructor
 * NOTE: set primitive to nullptr; set primitiveType to the primitive we'll use
**/

RowStatus::RowStatus() {
  primitive = nullptr;
  primitiveType = PRIMITIVE_STRING;
}

/**
 * @function ~RowStatus
 * @description RowStatus class destructor
**/

RowStatus::~RowStatus() {
  if (primitive != nullptr) {
    String<std::string>* primitivePtr = reinterpret_cast<String<std::string>*>(primitive);
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

bool RowStatus::setValue(std::string value) {
  if (primitive == nullptr) {
    primitive = new String<std::string>(value);
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

bool RowStatus::setValue(std::string oid, std::string value) {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return false;
  }
  String<std::string>* primitivePtr = reinterpret_cast<String<std::string>*>(primitive);
  //Check syntax for rowStatus
  if (value != "active" && value != "notInService" && value != "notReady" && value != "createAndGo" && value != "createAndWait" && value != "destroy") {
    return false;
  }
  return primitivePtr->setValue(oid, value);
}

/**
 * @function getPrintableValue
 * @description get printable value from its primitive
 * @returns std::string
**/

std::string RowStatus::getPrintableValue() {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return "";
  }
  String<std::string>* primitivePtr = reinterpret_cast<String<std::string>*>(primitive);
  return primitivePtr->getPrintableValue();
}

/**
 * @function getPrimitiveType
 * @description get primitive type name associated to this module
 * @returns std::string
**/

std::string RowStatus::getPrimitiveType() {
  return primitiveType;
}
