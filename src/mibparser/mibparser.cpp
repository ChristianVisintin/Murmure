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

#include <mibparser/mibparser.hpp>
#include <utils/databasefacade.hpp>
#include <utils/logger.hpp>
#include <utils/strutils.hpp>

using namespace murmure;

/**
 * @function Mibparser
 * @description Mibparser class constructor
**/

Mibparser::Mibparser() {
  mibtable = nullptr;
}

/**
 * @function ~Mibparser
 * @description Mibparser class destructor
**/

Mibparser::~Mibparser() {
  if (mibtable != nullptr) {
    delete mibtable;
  }
  mibtable = nullptr;
}

/**
 * @function parseMibFile
 * @description parse mib file and save OID nodes into database
 * @param std::string mibfile
 * @returns bool: true if parsing/database storage operations succeeded
**/

bool Mibparser::parseMibFile(std::string mibfile) {
  //TODO: implement this
}