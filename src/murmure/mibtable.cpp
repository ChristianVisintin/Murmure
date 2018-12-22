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

#include <murmure/mibtable.hpp>
#include <utils/databasefacade.hpp>
#include <utils/logger.hpp>

#include <algorithm>

#define COMPONENT "MibTable"

using namespace murmure;

/**
 * @function Mibtable
 * @description class constructor
**/

Mibtable::Mibtable() {
}

/**
 * @function ~Mibtable
 * @description class destructor; free oid vector
**/

Mibtable::~Mibtable() {

  //Delete oids in mibtable
  for (auto oid : oids) {
    delete oid;
  }
}

/**
 * @function loadMibTable
 * @description load mib table from murmure database
 * @returns bool: true if loaded successfully
**/

bool Mibtable::loadMibTable() {

  std::string errorString;

  //Open database
  if (!database::open(&errorString)) {
    //Database open failed
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  //Select oids from database

  //Init vector of row; each vector identifies a row, which is a vector of strings,
  //which identify columns
  std::vector<std::vector<std::string>> tableEntries;
  std::string query = "SELECT * FROM oids;";
  if (!database::select(&tableEntries, query, &errorString)) {
    logger::log(COMPONENT, LOG_ERROR, errorString);
    //try to close database
    if (!database::close(&errorString)) {
      logger::log(COMPONENT, LOG_ERROR, errorString);
      return false;
    }
    return false;
  }

  //try to close database
  if (!database::close(&errorString)) {
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  size_t mibTableSize = 0;
  //Fetch rows
  for (auto row : tableEntries) {
    if (row.size() != 5) {
      logger::log(COMPONENT, LOG_ERROR, "Unexpected oid row size");
      return false;
    }
    std::string oid = row.at(0);
    std::string name = row.at(1);
    std::string datatype = row.at(2);
    std::string value = row.at(3);
    std::string accessModeStr = row.at(4);
    int accessMode = std::stoi(accessModeStr);
    //Increase mibtable size
    oids.resize(++mibTableSize);
    //Instance new oid
    Oid* thisOid = new Oid(oid, datatype, accessMode, name);
    //Push new oid in oids vector
    oids.push_back(thisOid);
  }

  //Sort table
  sortMibTable();

  //Mib loaded successfully
  return true;
}

/**
 * @function sortMibTable
 * @description sort mib table oids from 
**/

void Mibtable::sortMibTable() {

  /*
  Sort oids, sorting is made from "least" to "greatest"
  which means for example
  1) .1.3.6.1.4.1.1994
  2) .1.3.6.1.4.1.1994.102.1
  3) .1.3.6.1.4.1.1994.102.1.1
  4) .1.3.6.1.4.1.1994.102.2
  5) .1.3.6.1.4.1.1994.102.2.1
  6) .1.3.6.1.4.1.1994.102.3
  7) .1.3.6.1.4.1.1994.102.3.1
  */
  std::sort(oids.begin(), oids.end());
}

/**
 * @function getOid
 * @description Given a OID string, this function returns the OID object associated
 * @param std::string: OID string to find
 * @returns: Oid*: pointer to Oid object associated to provided OID; nullptr is returned if not found
**/

Oid* Mibtable::getOid(std::string oidString) {

  for (auto oid : oids) {
    //Check if the oid is the same
    if (oid->getOid() == oidString) {
      return oid;
    }
  }

  return nullptr;
}

/**
 * @function getNextOid
 * @description given an oid, find the immediate next oid
 * @param std::string oid string 
 * @returns std::string next oid string
**/

std::string Mibtable::getNextOid(std::string oidString) {

  bool oidFound = false;
  for (auto oid : oids) {
    //If oid has been found in the previous cycle, return oid (which is the 'next')
    if (oidFound) {
      return oid->getOid();
    }
    //Check if the oid is the same
    if (oid->getOid() == oidString) {
      //Set oidFound
      oidFound = true;
    }
  }
  //Otherwise return ""
  return "";
}
