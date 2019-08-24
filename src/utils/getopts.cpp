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

#include <stdexcept>

namespace murmure {

/**
 * @function getOpts
 * @description Get Murmure options
 * @param options*: pointer to options structure
 * @param int: argument count
 * @param char* argv[]: arguments passed to Murmure
 * @param std::string*: error message filled if false is returned
 * @returns bool: True if options parsing succeeded; false otherwise
**/

bool getOpts(options* optStruct, int argc, char* argv[], std::string& error) {

  if (argc < 2) {
    error = "Missing argument";
    return false;
  }
  for (int i = 1; i < argc; i++) {
    const std::string arg = argv[i];
    if (arg == "-D") {
      optStruct->command = Command::DAEMON;
    } else if (arg == "-g") {
      //Get has 1 arg => OID
      if (argc <= (i + 1)) {
        error = "Missing OID argument";
        return false;
      }
      optStruct->command = Command::GET;
      optStruct->args.reserve(1);
      optStruct->args.push_back(std::string(argv[++i]));
    } else if (arg == "-s") {
      //Set has 3 args => OID, type, value
      if (argc <= (i + 3)) {
        error = "SET requires OID TYPE VALUE";
        return false;
      }
      optStruct->command = Command::SET;
      optStruct->args.reserve(3);
      optStruct->args.push_back(std::string(argv[++i]));
      optStruct->args.push_back(std::string(argv[++i]));
      optStruct->args.push_back(std::string(argv[++i]));
    } else if (arg == "-n") {
      //GetNext has 1 arg => OID
      if (argc <= (i + 1)) {
        error = "Missing OID argument";
        return false;
      }
      optStruct->command = Command::GET_NEXT;
      optStruct->args.reserve(1);
      optStruct->args.push_back(std::string(argv[++i]));
    } else if (arg == "-M" || arg == "--parse-mib") {
      //Mib parsing has 2 arg => [rootOid, mib file]
      if (argc <= (i + 2)) {
        error = "Missing RootOID/MIB file";
        return false;
      }
      optStruct->command = Command::PARSE_MIB;
      optStruct->args.reserve(2);
      optStruct->args.push_back(std::string(argv[++i]));
      optStruct->args.push_back(std::string(argv[++i]));
    } else if (arg == "-S" || arg == "--schedule") {
      //Can have file as argument
      optStruct->command = Command::SCHEDULE;
      if (argc > (i + 1)) {
        if (std::string(argv[i + 1]).at(0) != '-') {
          optStruct->args.reserve(1);
          optStruct->args.push_back(std::string(argv[++i]));
        }
      }
    } else if (arg == "--dump-scheduling") {
      //Can have file as argument
      optStruct->command = Command::DUMP_SCHEDULE;
      if (argc > (i + 1)) {
        if (std::string(argv[i + 1]).at(0) != '-') {
          optStruct->args.reserve(1);
          optStruct->args.push_back(std::string(argv[++i]));
        }
      }
    } else if (arg == "--reset") {
      optStruct->command = Command::RESET;
    } else if (arg == "-C" || arg == "--change") {
      if (argc <= (i + 2)) {
        error = "Missing OID and value arguments";
        return false;
      }
      optStruct->command = Command::CHANGE;
      optStruct->args.reserve(2);
      optStruct->args.push_back(std::string(argv[++i]));
      optStruct->args.push_back(std::string(argv[++i]));
    } else if (arg == "-h" || arg == "--help") {
      optStruct->command = Command::HELP;
    } else if (arg == "-l") {
      if (argc <= (i + 1)) {
        error = "Missing log level argument";
        return false;
      }
      optStruct->logLevelSet = true;
      try {
        optStruct->logLevel = std::stoi(argv[++i]);
      } catch (std::invalid_argument& ex) {
        error = "log level is not a number";
        return false;
      }
      if (optStruct->logLevel > 5 && optStruct->logLevel < 0) {
        error = "log level is not in range 0-5";
        return false;
      } 
    } else if (arg == "-L") {
      if (argc <= (i + 1)) {
        error = "Missing log file argument";
        return false;
      }
      optStruct->logFileSet = true;
      optStruct->logFile = argv[++i];
    } else if (arg == "-d") {
      if (argc <= (i + 1)) {
        error = "Missing database path argument";
        return false;
      }
      optStruct->dbPathSet = true;
      optStruct->dbPath = argv[++i];
    } else {
      error = "Unknown option '" + arg + "'";
      return false;
    }
  }

  return true;
}

}
