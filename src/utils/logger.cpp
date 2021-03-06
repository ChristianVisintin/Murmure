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

#include <utils/logger.hpp>

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace logger;

int logger::logLevel;
std::string logger::logfile;
bool logger::toStdout;

/**
 * @function getLevelName
 * @description given integer log level, get the log level name
 * @param int level
 * @returns std::string: log level name
**/

std::string getLevelName(int level) {

  switch (level) {
  case LOG_DEBUG:
    return "DEBUG";
  case LOG_INFO:
    return "INFO";
  case LOG_FATAL:
    return "FATAL";
  case LOG_WARN:
    return "WARN";
  case LOG_ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}

/**
 * @function log
 * @description write log message to file
 * @param std::string component
 * @param int log level
 * @param std::string logContent
**/

void logger::log(const std::string& component, int level, const std::string& logContent) {

  //Check if this message has to be logged
  if (level > logLevel) {
    return;
  }

  std::stringstream logstream;
  std::ofstream logFileStream;

  //Prepare data to write
  auto now = std::chrono::system_clock::now();
  time_t in_time_t = std::chrono::system_clock::to_time_t(now);

  logstream << std::put_time(std::localtime(&in_time_t), "%Y/%m/%d-%H:%M:%S [") << getLevelName(level) << "] " << component << ": " << logContent << "\n";
  std::string logStr = logstream.str();

  logFileStream.open(logfile, std::ofstream::out | std::ofstream::app);
  if (logFileStream.is_open()) {
    logFileStream << logStr;
    logFileStream.close();
  }
  if (toStdout) {
    std::cout << logStr;
  }
  return;
}
