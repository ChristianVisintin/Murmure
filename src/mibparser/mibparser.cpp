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

#define MIB_ROOT ".1.3.6.1"
//Directory node
#define MIB_DIRECTORY ".1"
//Mibv2 sub-node
#define MIB_MIB2 ".1"
#define MIB_MIB2_SYSTEM ".1"
#define MIB_MIB2_INTERFACES ".2"
#define MIB_MIB2_IP ".4"

//Management node
#define MIB_MGMT ".2"

//Experimental node
#define MIB_EXPERIMENTAL ".3"

//Private node
#define MIB_PRIVATE ".4"
#define MIB_ENTERPRISE ".1"


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