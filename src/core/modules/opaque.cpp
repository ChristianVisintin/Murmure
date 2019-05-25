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

#include <murmure/modules/opaque.hpp>
#include <murmure/primitives/string.hpp>

using namespace murmure;

/**
 * @function Opaque
 * @description Opaque class constructor
 * NOTE: set primitive to nullptr; set primitiveType to the primitive we'll use
**/

Opaque::Opaque() {
  primitive = nullptr;
  primitiveType = PRIMITIVE_STRING;
}

/**
 * @function ~Opaque
 * @description Opaque class destructor
**/

Opaque::~Opaque() {
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

bool Opaque::setValue(std::string value) {
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

bool Opaque::setValue(std::string oid, std::string value) {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return false;
  }
  String<std::string>* primitivePtr = reinterpret_cast<String<std::string>*>(primitive);
  return primitivePtr->setValue(oid, value);
}

/**
 * @function getPrintableValue
 * @description get printable value from its primitive
 * @returns std::string
**/

std::string Opaque::getPrintableValue() {
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

std::string Opaque::getPrimitiveType() {
  return primitiveType;
}
