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

#ifndef ACCESSMODE_HPP
#define ACCESSMODE_HPP

#define ACCESSMODE_NOTACCESSIBLE 0
#define ACCESSMODE_READONLY 1
#define ACCESSMODE_READCREATE 2
#define ACCESSMODE_READWRITE 3

namespace murmure {
enum class AccessMode {
  NOT_ACCESSIBLE, //0
  READONLY,       //1
  READCREATE,     //2
  READWRITE       //3
};
} /* namespace murmure */

#endif