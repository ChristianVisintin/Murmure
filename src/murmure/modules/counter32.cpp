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

#include <murmure/modules/counter32.hpp>
#include <murmure/primitives/gauge.hpp>

using namespace murmure;

/**
 * @function Counter32
 * @description Counter32 class constructor
 * NOTE: set primitive to nullptr; set primitiveType to the primitive we'll use
**/

Counter32::Counter32() {
  primitive = nullptr;
  primitiveType = PRIMITIVE_GAUGE;
}

/**
 * @function ~Counter32
 * @description Counter32 class destructor
**/

Counter32::~Counter32() {
  if (primitive != nullptr) {
    Gauge<unsigned int>* primitivePtr = reinterpret_cast<Gauge<unsigned int>*>(primitive);
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

bool Counter32::setValue(std::string value) {
  if (primitive == nullptr) {
    primitive = new Gauge<unsigned int>(value);
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

bool Counter32::setValue(std::string oid, std::string value) {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return false;
  }
  //Check value for counter32
  if (std::stoi(value) > 4294967295) {
    return false;
  }
  Gauge<unsigned int>* primitivePtr = reinterpret_cast<Gauge<unsigned int>*>(primitive);
  return primitivePtr->setValue(oid, value);
}

/**
 * @function getPrintableValue
 * @description get printable value from its primitive
 * @returns std::string
**/

std::string Counter32::getPrintableValue() {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return "";
  }
  Gauge<unsigned int>* primitivePtr = reinterpret_cast<Gauge<unsigned int>*>(primitive);
  return primitivePtr->getPrintableValue();
}

/**
 * @function getPrimitiveType
 * @description get primitive type name associated to this module
 * @returns std::string
**/

std::string Counter32::getPrimitiveType() {
  return primitiveType;
}
