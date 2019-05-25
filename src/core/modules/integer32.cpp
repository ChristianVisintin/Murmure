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

#include <murmure/modules/integer32.hpp>
#include <murmure/primitives/integer.hpp>

using namespace murmure;

/**
 * @function Integer32
 * @description Integer32 class constructor
 * NOTE: set primitive to nullptr; set primitiveType to the primitive we'll use
**/

Integer32::Integer32() {
  primitive = nullptr;
  primitiveType = PRIMITIVE_INTEGER;
}

/**
 * @function ~Integer32
 * @description Integer32 class destructor
**/

Integer32::~Integer32() {
  if (primitive != nullptr) {
    Integer<int>* primitivePtr = reinterpret_cast<Integer<int>*>(primitive);
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

bool Integer32::setValue(std::string value) {
  if (primitive == nullptr) {
    primitive = new Integer<int>(value);
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

bool Integer32::setValue(std::string oid, std::string value) {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return false;
  }
  //Check value for Integer32
  if (std::stoi(value) > 2147483647 || std::stoi(value) < -2147483648) {
    return false;
  }
  Integer<int>* primitivePtr = reinterpret_cast<Integer<int>*>(primitive);
  return primitivePtr->setValue(oid, value);
}

/**
 * @function getPrintableValue
 * @description get printable value from its primitive
 * @returns std::string
**/

std::string Integer32::getPrintableValue() {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return "";
  }
  Integer<int>* primitivePtr = reinterpret_cast<Integer<int>*>(primitive);
  return primitivePtr->getPrintableValue();
}

/**
 * @function getPrimitiveType
 * @description get primitive type name associated to this module
 * @returns std::string
**/

std::string Integer32::getPrimitiveType() {
  return primitiveType;
}
