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

#ifndef DATABASEFACADE_HPP
#define DATABASEFACADE_HPP

#define Q(x) #x
#define QUOTE(x) Q(x)

#ifndef DBPATH
#define DATABASEPATH "/usr/local/share/mib.db"
#else
#define DATABASEPATH QUOTE(DBPATH)
#endif

#include <vector>
#include <string>

namespace database {

bool open(std::string* error);
bool close(std::string* error);
bool exec(std::string query, std::string* error);
bool select(std::vector<std::vector<std::string>>* result, std::string query, std::string* error);

}

#endif