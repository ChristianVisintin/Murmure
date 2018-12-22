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

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>

#define LOG_DEBUG 5
#define LOG_INFO 4
#define LOG_WARN 3
#define LOG_ERROR 2
#define LOG_FATAL 1

namespace logger {

extern std::string logfile = nullptr;
extern bool stdout = true;
extern int logLevel = LOG_INFO;

void log(std::string component, int level, std::string logContent);

} // namespace logger

#endif