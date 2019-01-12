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

#ifndef MODULE_HPP
#define MODULE_HPP

#include <murmure/primitives/primitive.hpp>

#include <cinttypes>
#include <string>

namespace murmure {

class Module {

public:
  virtual ~Module() {};
  virtual bool setValue(std::string value) = 0;
  virtual bool setValue(std::string oid, std::string value) = 0;
  virtual std::string getPrintableValue() = 0;
  virtual std::string getPrimitiveType() = 0;

protected:
  void* primitive;           //Void ptr to primitive instance
  std::string primitiveType; //Primitive type string
};
} // namespace murmure

#endif