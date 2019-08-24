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
