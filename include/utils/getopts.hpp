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

#ifndef GETOPTS_HPP
#define GETOPTS_HPP

#include <string>
#include <vector>

namespace murmure {

//Option commands

enum class Command {
  DAEMON,
  GET,
  SET,
  GET_NEXT,
  PARSE_MIB,
  SCHEDULE,
  DUMP_SCHEDULE,
  RESET,
  CHANGE,
  HELP
};

typedef struct {
  Command command;
  std::vector<std::string> args;
  std::string logFile;
  bool logFileSet = false;
  std::string dbPath;
  bool dbPathSet = false;
  int logLevel;
  bool logLevelSet = false;
} options;

bool getOpts(options* optStruct, int argc, char* argv[], std::string& error);

} // namespace murmure

#endif
