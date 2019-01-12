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

#include <utils/getopts.hpp>

using namespace murmure;

/**
 * @function getOpts
 * @description Get Murmure options
 * @param options*: pointer to options structure
 * @param int: argument count
 * @param char* argv[]: arguments passed to Murmure
 * @param std::string*: error message filled if false is returned
 * @returns bool: True if options parsing succeeded; false otherwise
**/

bool murmure::getOpts(options* optStruct, int argc, char* argv[], std::string* error) {

  if (argc < 2) {
    *error = "Missing argument";
    return false;
  }

  std::string arg = argv[1];

  if (arg == "-D") {
    optStruct->command = Command::DAEMON;
  } else if (arg == "-g") {
    //Get has 1 arg => OID
    if (argc < 3) {
      *error = "Missing OID argument";
      return false;
    }
    optStruct->command = Command::GET;
    optStruct->args.reserve(1);
    optStruct->args.push_back(std::string(argv[2]));
  } else if (arg == "-s") {
    //Set has 3 args => OID, type, value
    if (argc < 5) {
      *error = "SET requires OID TYPE VALUE";
      return false;
    }
    optStruct->command = Command::SET;
    optStruct->args.reserve(3);
    optStruct->args.push_back(std::string(argv[2]));
    optStruct->args.push_back(std::string(argv[3]));
    optStruct->args.push_back(std::string(argv[4]));
  } else if (arg == "-n") {
    //GetNext has 1 arg => OID
    if (argc < 3) {
      *error = "Missing OID argument";
      return false;
    }
    optStruct->command = Command::GET_NEXT;
    optStruct->args.reserve(1);
    optStruct->args.push_back(std::string(argv[2]));
  } else if (arg == "-M" || arg == "--parse-mib") {
    //Mib parsing has 2 arg => [rootOid, mib file]
    if (argc < 4) {
      *error = "Missing RootOID/MIB file";
      return false;
    }
    optStruct->command = Command::PARSE_MIB;
    optStruct->args.reserve(2);
    optStruct->args.push_back(std::string(argv[2]));
    optStruct->args.push_back(std::string(argv[3]));
  } else if (arg == "-S" || arg == "--schedule") {
    //Can have file as argument
    optStruct->command = Command::SCHEDULE;
    if (argc > 2) {
      optStruct->args.reserve(1);
      optStruct->args.push_back(std::string(argv[2]));
    }
  } else if (arg == "--dump-scheduling") {
    //Can have file as argument
    optStruct->command = Command::DUMP_SCHEDULE;
    if (argc > 2) {
      optStruct->args.reserve(1);
      optStruct->args.push_back(std::string(argv[2]));
    }
  } else if (arg == "--reset") {
    optStruct->command = Command::RESET;
  } else if (arg == "-C" || arg == "--change") {
    if (argc < 4) {
      *error = "Missing OID and value arguments";
      return false;
    }
    optStruct->command = Command::CHANGE;
    optStruct->args.reserve(2);
    optStruct->args.push_back(std::string(argv[2]));
    optStruct->args.push_back(std::string(argv[3]));
  } else if (arg == "-h" || arg == "--help") {
    optStruct->command = Command::HELP;
  } else {
    *error = "Unknown option";
    return false;
  }

  return true;
}