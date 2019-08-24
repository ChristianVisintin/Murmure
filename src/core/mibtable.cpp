/**
 *   Murmure - Net-SNMP MIB Versatile Extender
 *   Developed by Christian Visintin
 * 
 * MIT License
 * Copyright (c) 2019 Christian Visintin
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
**/

#include <core/mibtable.hpp>
#include <utils/databasefacade.hpp>
#include <utils/logger.hpp>

#include <algorithm>
#include <sstream>

#define COMPONENT "MibTable"

namespace murmure {

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
  for (auto&  oid : oids) {
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

  //Select oids from database

  //Init vector of row; each vector identifies a row, which is a vector of strings,
  //which identify columns
  std::vector<std::vector<std::string>> tableEntries;
  std::string query = "SELECT * FROM oids;";
  if (!database::select(&tableEntries, query, errorString)) {
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  size_t mibTableSize = 0;
  //Fetch rows
  for (auto& row : tableEntries) {
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
    oids.resize(mibTableSize++);
    //Instance new oid
    Oid* thisOid = new Oid(oid, datatype, value, accessMode, name);
    //Check if data is nullptr
    if (!thisOid->isTypeValid()) {
      std::stringstream logStream;
      logStream << "Could not resolve type " << thisOid->getType();
      logStream << " for OID " << thisOid->getOid();
      logger::log(COMPONENT, LOG_ERROR, logStream.str());
      delete thisOid;
      return false;
    }
    //Push new oid in oids vector
    oids.push_back(thisOid);
  }

  //Sort table
  sortMibTable();

  //Mib loaded successfully
  return true;
}

/**
 * @function addOid
 * @ðescription add new OID object to mib table
 * @param Oid* pointer to new OID object
 * @returns bool: true if added successfully
**/

bool Mibtable::addOid(Oid* newOid) {
  std::string errorString;

  //Add new OID to database
  std::stringstream queryStream;
  queryStream << "INSERT INTO oids(oid, name, datatype, value, accessmode) VALUES (";
  queryStream << "\"" << newOid->getOid() << "\"";
  queryStream << ", \"" << newOid->getName() << "\"";
  queryStream << ", \"" << newOid->getType() << "\"";
  queryStream << ", \"" << newOid->getPrintableValue() << "\"";
  queryStream << ", " << newOid->getAccessModeInteger() << ");";
  std::string query = queryStream.str();

  if (!database::exec(query, errorString)) {
    //Database commit failed
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  //Finally add new OID object to mibtable vector
  oids.push_back(newOid);

  //Sort mib table
  this->sortMibTable();
  return true;
}

/**
 * @function clearMibtable
 * @description clear all mib table entries from database and vector
 * @returns bool: true if operation has been completed successfully
**/

bool Mibtable::clearMibtable() {
  std::string errorString;
  //Add new OID to database
  std::string query = "DELETE FROM oids;";
  if (!database::exec(query, errorString)) {
    //Database commit failed
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }
  //Delete oids
  for (auto& oid : oids) {
    delete oid;
  }
  //Finally clear OIDs vector
  oids.clear();
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
  std::sort(oids.begin(), oids.end(), sortByOid);
}

/**
 * @function getOidByOid
 * @description Given a OID string, this function returns the OID object associated
 * @param std::string: OID string to find
 * @returns: Oid*: pointer to Oid object associated to provided OID; 
 * NOTE: nullptr is returned if not found
**/

Oid* Mibtable::getOidByOid(const std::string& oidString) {

  for (auto& oid : oids) {
    //Check if the oid is the same
    if (oid->getOid() == oidString) {
      return oid;
    }
  }

  return nullptr;
}

/**
 * @function getOidByName
 * @description Given a OID name, this function returns the OID object associated
 * @param std::string oid name
 * @returns Oid* pointer to OID object associated to provided OID name;
 * NOTE: nullptr is returned if not found
**/

Oid* Mibtable::getOidByName(const std::string& oidName) {
  for (auto& oid : oids) {
    //Check if the oid is the same
    if (oid->getName() == oidName) {
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

std::string Mibtable::getNextOid(const std::string& oidString) {

  bool oidFound = false;
  for (auto& oid : oids) {
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

/**
 * @function getPreviousOid
 * @description given an oid, find the immediate previous oid
 * @param std::string oid string 
 * @returns std::string previous oid string
**/

std::string Mibtable::getPreviousOid(const std::string& oidString) {

  Oid* previousOid = nullptr;
  for (auto& oid : oids) {
    //Check if previousOid is not nullptr and current oid is equal to provided one
    if (oid->getOid() == oidString && previousOid != nullptr) {
      return previousOid->getOid();
    }
    previousOid = oid;
  }
  return "";
}

/**
 * @function isTableChild
 * @description check if provided oid is child of the mib table
 * @param std::string
 * @returns bool
**/

bool Mibtable::isTableChild(const std::string& oidString) {

  //Get parent Oid
  std::string parentOidStr;
  size_t lastDotPos = oidString.find_last_of(".");
  if (lastDotPos != std::string::npos) {
    parentOidStr = oidString.substr(0, lastDotPos);
  } else {
    //@! Is not a valid OID
    return false;
  }
  //Get grandParent OID
  Oid* grandParentOid = this->getOidByOid(this->getPreviousOid(parentOidStr));
  if (grandParentOid != nullptr) {
    //Check if it is table
    if (grandParentOid->getPrimitiveType() == PRIMITIVE_SEQUENCE) {
      //@! It is table child
      return true;
    } else {
      //@! Ain't table child
      return false;
    }
  } else {
    //@! gran parent does not exist
    return false;
  }
}

}
