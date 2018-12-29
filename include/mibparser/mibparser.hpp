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

#ifndef MIBPARSER_HPP
#define MIBPARSER_HPP

#include <murmure/mibtable.hpp>

namespace murmure {

class Mibparser {

public:
  Mibparser();
  ~Mibparser();
  bool parseMibFile(std::string rootOid, std::string mibfile);

private:
  bool parseLine(std::string line);
  Mibtable* mibtable;
  std::string rootOid;

};
} // namespace murmure

#endif