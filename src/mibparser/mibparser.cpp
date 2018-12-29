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
#include <utils/logger.hpp>
#include <utils/strutils.hpp>

#include <fstream>
#include <sstream>

using namespace murmure;

#define COMPONENT "MIBParser"

/**
 * @function Mibparser
 * @description Mibparser class constructor
**/

Mibparser::Mibparser() {
  mibtable = nullptr;
  rootOid = nullptr;
  currentName = "";
  currentOid = "";
  currentType = "";
  currentAccessMode = -1;
  oidSaved = false;
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

bool Mibparser::parseMibFile(std::string rootOid, std::string mibfile) {
  this->rootOidStr = rootOid;
  //Try to open file
  std::ifstream mibfileStream;
  mibfileStream.open(mibfile);

  if (!mibfileStream.is_open()) {
    std::stringstream logStream;
    logStream << "Could not open mibfile " << mibfile;
    logger::log(COMPONENT, LOG_ERROR, logStream.str());
    return false;
  }

  //Create mibtable instance
  mibtable = new Mibtable();
  //Clear mib table
  if (!mibtable->clearMibtable()) {
    logger::log(COMPONENT, LOG_ERROR, "Could not clear old MIBtable");
    //Close file
    mibfileStream.close();
    return false;
  }

  //Iterate over lines
  std::string line;
  while (getline(mibfileStream, line)) {
    //DEBUG line
    logger::log(COMPONENT, LOG_DEBUG, ">> " + line);
    //Trim line from mib file
    std::string trimmedLine = strutils::trim(line);
    //Line parsing
    bool parseResult = parseLine(trimmedLine);
    if (!parseResult) {
      //Clear mib table
      mibtable->clearMibtable();
      //Close file
      mibfileStream.close();
      return false;
    }
  }

  //Commit any last unsaved OID
  if (!commitPreviousOid()) {
    logger::log(COMPONENT, LOG_ERROR, "Could not commit last OID");
    mibtable->clearMibtable();
    mibfileStream.close();
    return false;
  }

  //Close file
  mibfileStream.close();

  return true;
}

/**
 * @function parseLine
 * @description parse line from configuration
 * @param std::string line
 * @returns bool: false if there is a syntax error
**/

bool Mibparser::parseLine(std::string line) {
  //Ignore comments
  if (strutils::startsWith(line, "--")) {
    return true;
  }
  //Ignore description strings
  if (strutils::startsWith(line, "\"")) {
    return true;
  }

  //Module-identity check (identifies MIB root OID)
  if (strutils::endsWith(line, "MODULE-IDENTITY")) {
    return handleModuleIdentity(line);
  }

  //In line OBJECT IDENTIFIER declaration
  if (line.find("::= {") != std::string::npos && line.find("OBJECT IDENTIFIER") != std::string::npos) {
    return handleInlineObject(line);
  }

  //Object-Type declaration
  if (strutils::endsWith(line, "OBJECT-TYPE")) {
    return handleObjectDeclaration(line);
  }

  //Object-Type data type syntax
  if (strutils::startsWith(line, "SYNTAX")) {
    return handleObjectSyntax(line);
  }

  //Object-Type max access
  if (strutils::startsWith(line, "MAX-ACCESS")) {
    return handleObjectAccess(line);
  }

  //Unhandled lines are ignored
  return true;
}

/**
 * @function commitPreviousOid
 * @description check if there is any previous unsaved OID; if there is, save it into mibtable
 * @returns bool: true if previous oid has been saved successfully
 * NOTE: true is returned even if there was no OID to save
**/

bool Mibparser::commitPreviousOid() {

  if (oidSaved) {
    //Already saved OID
    return true;
  }

  //Check if attributes are set
  if (currentName.length() == 0 or currentOid.length() == 0 or currentAccessMode == -1 or currentType.length() == 0) {
    logger::log(COMPONENT, LOG_ERROR, "Missing Object parameter");
    return false;
  }

  Oid* newOid = new Oid(currentOid, currentType, "0", currentAccessMode, currentName);
  //Try to add new OID to mibtable
  if (mibtable->addOid(newOid)) {
    std::stringstream logStream;
    logStream << "OID " << currentName << " (" << currentOid << ") added successfully to mibtable";
    logger::log(COMPONENT, LOG_INFO, logStream.str());
  } else {
    std::stringstream logStream;
    logStream << "Failed to add OID " << currentName << " (" << currentOid << ") to mibtable";
    logger::log(COMPONENT, LOG_ERROR, logStream.str());
    delete newOid;
    return false;
  }

  //Set oidSaved to true
  oidSaved = true;
  return true;
}

// Mibparser specialized parsing methods

/**
 * @function handleModuleIdentity
 * @description handle module identity parameter (name of MIB root object)
 * @param std::string line
 * @returns bool: true if syntax is valid
**/

bool Mibparser::handleModuleIdentity(std::string line) {
  //Check if rootOID is already set
  if (rootOid != nullptr) {
    logger::log(COMPONENT, LOG_ERROR, "Syntax error: MODULE-IDENTITY has already been set");
    return false;
  }
  //Find name
  size_t moduleNameDiv;
  moduleNameDiv = line.find(' ');
  if (moduleNameDiv == std::string::npos) {
    logger::log(COMPONENT, LOG_ERROR, "Syntax error: Missing MODULE name");
    return false;
  }
  std::string moduleName = line.substr(0, moduleNameDiv);
  rootOid = new Oid(rootOidStr, PRIMITIVE_OBJECTID, "", ACCESSMODE_NOTACCESSIBLE, moduleName);
  //Add rootOID to mibtable
  if (mibtable->addOid(rootOid)) {
    std::stringstream logStream;
    logStream << "Root OID " << moduleName << " (" << rootOidStr << ") added successfully to mibtable";
    logger::log(COMPONENT, LOG_INFO, logStream.str());
    return true;
  } else {
    logger::log(COMPONENT, LOG_ERROR, "Failed to add root OID to mibtable");
    delete rootOid;
    rootOid = nullptr;
    return false;
  }
  return true;
}

/**
 * @function handleInlineObject
 * @description handle object inline declaration (groups)
 * @param std::string line
 * @returns bool: true if syntax is valid
**/

bool Mibparser::handleInlineObject(std::string line) {
  //Root OID must be set
  if (rootOid == nullptr) {
    logger::log(COMPONENT, LOG_ERROR, "Syntax error: MODULE-IDENTITY is not declared");
    return false;
  }
  //Find name
  size_t nameDiv;
  nameDiv = line.find(' ');
  if (nameDiv == std::string::npos) {
    logger::log(COMPONENT, LOG_ERROR, "Syntax error: Missing OBJECT name");
    return false;
  }
  std::string name = line.substr(0, nameDiv);
  //Find parent and OID
  size_t oidInfoDiv = line.find("::= {") + 5;
  size_t oidInfoEndDiv = line.find("}");
  if (oidInfoDiv > oidInfoEndDiv) {
    logger::log(COMPONENT, LOG_ERROR, "Syntax error: Missing '}' in object identifier");
    return false;
  }
  //Do not check, we already know that this string exists in line
  std::string oidInfoStr = strutils::substring(line, oidInfoDiv, oidInfoEndDiv);
  //Trim object info string
  oidInfoStr = strutils::trim(oidInfoStr);
  //Tokenize info
  std::vector<std::string> oidInfo = strutils::split(oidInfoStr, ' ');
  //Check if there are two tokens
  if (oidInfo.size() != 2) {
    std::stringstream logStream;
    logStream << "Syntax error: OID INFO has size " << std::to_string(oidInfo.size()) << " instead of 2";
    logger::log(COMPONENT, LOG_ERROR, logStream.str());
    return false;
  }
  Oid* parentOid = mibtable->getOidByName(oidInfo.at(0));
  //Check if parentOID exists
  if (parentOid == nullptr) {
    logger::log(COMPONENT, LOG_ERROR, "Could not find any OID with name " + oidInfo.at(0));
    return false;
  }
  //Get OID string
  std::string oid = parentOid->getOid() + "." + oidInfo.at(1);
  //Instance new OID
  Oid* newOid = new Oid(oid, PRIMITIVE_OBJECTID, "", ACCESSMODE_NOTACCESSIBLE, name);
  if (mibtable->addOid(newOid)) {
    std::stringstream logStream;
    logStream << "OID " << name << " (" << oid << ") added successfully to mibtable";
    logger::log(COMPONENT, LOG_INFO, logStream.str());
    return true;
  } else {
    std::stringstream logStream;
    logStream << "Failed to add OID " << name << " (" << oid << ") to mibtable";
    logger::log(COMPONENT, LOG_ERROR, logStream.str());
    delete newOid;
    return false;
  }
}

/**
 * @function handleObjectDeclaration
 * @description handle multi-line object declaration
 * @param std::string line
 * @returns bool: true if syntax is valid
**/

bool Mibparser::handleObjectDeclaration(std::string line) {

  //Check for any previous oid to commit
  if (!commitPreviousOid()) {
    logger::log(COMPONENT, LOG_ERROR, "Could not commit previous OID");
    return false;
  }

  //Remove multiple whitespaces
  line = strutils::itrim(line);
  //Get name
  std::vector<std::string> lineTokens = strutils::split(line, ' ');
  if (lineTokens.size() != 2) {
    std::stringstream logStream;
    logStream << "Syntax error: OID declaration has size " << std::to_string(lineTokens.size()) << " instead of 2";
    logger::log(COMPONENT, LOG_ERROR, logStream.str());
    return false;
  }

  //Set current name
  currentName = lineTokens.at(0);

  //Reset oid attributes
  currentOid = "";
  currentType = "";
  currentAccessMode = -1;

  logger::log(COMPONENT, LOG_INFO, "Set name to " + currentName);

  //Set oidSaved to 'false'
  oidSaved = false;
  return true;
}

/**
 * @function handleObjectSyntax
 * @description get object datatype from syntax declaration
 * @param std::string line
 * @returns bool: true if parsing was successful
**/

bool Mibparser::handleObjectSyntax(std::string line) {

  //Remove multiple whitespaces
  line = strutils::itrim(line);
  //Get syntax
  std::vector<std::string> lineTokens = strutils::split(line, ' ');
  if (lineTokens.size() < 2) {
    std::stringstream logStream;
    logStream << "Syntax error: OID SYNTAX has size " << std::to_string(lineTokens.size()) << " but must be at least 2";
    logger::log(COMPONENT, LOG_ERROR, logStream.str());
    return false;
  }

  //Set current data type
  currentType = lineTokens.at(1);
  logger::log(COMPONENT, LOG_INFO, "Set type to " + currentType);

  return true;

}

/**
 * @function handleObjectAccess
 * @description get object access mode from max-access declaration
 * @param std::string line
 * @returns bool: true if parsing was successful
**/

bool Mibparser::handleObjectAccess(std::string line) {

  //Remove multiple whitespaces
  line = strutils::itrim(line);
  //Get max access
  std::vector<std::string> lineTokens = strutils::split(line, ' ');
  if (lineTokens.size() < 2) {
    std::stringstream logStream;
    logStream << "Syntax error: OID MAX-ACCESS has size " << std::to_string(lineTokens.size()) << " but must be at least 2";
    logger::log(COMPONENT, LOG_ERROR, logStream.str());
    return false;
  }

  //Set current max-access
  std::string access = lineTokens.at(1);
  if (access == "not-accessible") {
    currentAccessMode = ACCESSMODE_NOTACCESSIBLE;
  } else if (access == "read-only") {
    currentAccessMode = ACCESSMODE_READONLY;
  } else if (access == "read-create") {
    currentAccessMode = ACCESSMODE_READCREATE;
  } else if (access == "read-write") {
    currentAccessMode = ACCESSMODE_READWRITE;
  } else {
    logger::log(COMPONENT, LOG_ERROR, "Unknown max access " + access);
    return false;
  }

  logger::log(COMPONENT, LOG_INFO, "Set AccessMode to " + currentAccessMode);
  return true;

}