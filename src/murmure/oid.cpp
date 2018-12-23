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

#include <murmure/modulefacade.hpp>
#include <murmure/oid.hpp>
#include <murmure/primitives/integer.hpp>
#include <murmure/primitives/ipaddress.hpp>
#include <murmure/primitives/octet.hpp>
#include <murmure/primitives/string.hpp>

#include <algorithm>
#include <vector>

using namespace murmure;

/**
 * @function Oid
 * @description Oid class constructor
 * @param std::string oid: oid string
 * @param std::string type: type string
 * @param int access: access level integer (as stored into database)
 * @param std::string: optional oid name
**/

Oid::Oid(std::string oid, std::string type, std::string value, int access, std::string name /* = nullptr */) {

  //Set OID string
  this->oid = oid;

  //Convert type to lower case
  std::transform(type.begin(), type.end(), type.begin(), ::tolower);
  this->dataType = type;
  this->primitiveType = type;
  //Based on type instance new Primitive type
  if (type == PRIMITIVE_INTEGER) {
    data = new Integer(value);
  } else if (type == PRIMITIVE_IPADRRESS) {
    data = new IPAddress(value);
  } else if (type == PRIMITIVE_OCTET) {
    data = new Octet(value);
  } else if (type == PRIMITIVE_STRING) {
    data = new String(value);
  } else {
    //TODO: Query module facade
    //this->primitiveType = getPrimitiveFromModule
  }

  //Set access mode
  switch (access) {
  case 0:
    this->accessMode = AccessMode::NOT_ACCESSIBLE;
    break;
  case 1:
    this->accessMode = AccessMode::READONLY;
    break;
  case 2:
    this->accessMode = AccessMode::READCREATE;
    break;
  case 3:
    this->accessMode = AccessMode::READWRITE;
    break;
  }
  //Set name
  this->name = name;
}

/**
 * @function ~Oid
 * @description Oid class destructor
**/

Oid::~Oid() {

  //Free primitive instance (data)
  delete data;
}

/**
 * @function getOid
 * @description returns oid string
 * @returns std::string
**/

std::string Oid::getOid() {
  return this->oid;
}

/**
 * @function getType
 * @description returns type string
 * @returns std::string
**/

std::string Oid::getType() {
  return this->dataType;
}

/**
 * @function getPrimitiveType
 * @description returns primitive type string
 * @returns std::string
**/

std::string Oid::getPrimtiveType() {
  return this->primitiveType;
}

/**
 * @function getName
 * @description returns oid name
 * @returns std::string
**/

std::string Oid::getName() {
  return this->name;
}

/**
 * @function getName
 * @description returns data printable value
 * @returns std::string
**/

std::string Oid::getPrintableValue() {
  return this->data->getPrintableValue();
}

/**
 * @function getName
 * @description returns oid access mode
 * @returns AccessMode
**/

AccessMode Oid::getAccessMode() {
  return this->accessMode;
}

/**
 * @function setValue
 * @description Set primitive value from a printable received by net-SNMP
 * @param std::string printable value
 * @returns bool: true if set successfully (false if operation on database failed)
**/

bool Oid::setValue(std::string printableValue) {

  //Value set operation is managed by Primitive extended class
  return this->data->setValue(printableValue);
}

/**
 * @function sortByOid
 * @description sort oid instance by their oid (to use with sort)
 * @param Oid* first oid to compare
 * @param Oid* second oid to compare
 * @returns bool: true if this oid is < passed oid
**/

bool murmure::sortByOid(Oid* firstOid, Oid* secondOid) {

  return firstOid->getOid().compare(secondOid->getOid()) < 0;

}
