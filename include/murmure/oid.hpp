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

#ifndef OID_HPP
#define OID_HPP

#include <murmure/accessmode.hpp>
#include <murmure/primitives/primitive.hpp>
#include <string>

namespace murmure {

class Oid {

public:
  Oid(std::string oid, std::string type, int access, std::string name = nullptr);
  ~Oid();

private:
  std::string oid;       //OID which identifies this instance
  std::string name;      //optional name for OID
  AccessMode accessMode; //Access level for OID
  std::string dataType;  //Type string
  Primitive<void*>* data;       //Wrapper of value
};

} // namespace murmure

#endif