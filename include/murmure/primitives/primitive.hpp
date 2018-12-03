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

#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <string>

namespace murmure {

//Data string definition CHECK: if correct
#define PRIMITIVE_INTEGER "integer"
#define PRIMITIVE_OCTET "octet"
#define PRIMITIVE_STRING "string"
#define PRIMITIVE_IPADRRESS "ipaddress"

//Template for generic primitive value
template <typename primitiveType>

/**
 * Interface for all primitives types
 * 
 * All primitives must implement setValue, which converts a string (passed by net-snmp)
 * to primitiveType. Also getPrintableValue must be implemented, which converts the data
 * back to a printable value. getValue just returns the value
**/

class Primitive {
public:
  virtual bool setValue(std::string value) = 0;
  virtual primitiveType getValue() = 0;
  virtual std::string getPrintableValue() = 0;

protected:
  virtual primitiveType value;
};
} // namespace murmure

#endif