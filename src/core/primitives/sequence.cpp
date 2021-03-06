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

#include <core/primitives/sequence.hpp>
#include <utils/databasefacade.hpp>
#include <utils/logger.hpp>

#include <sstream>

#define COMPONENT "OID"

namespace murmure {

/**
 * @function Sequence
 * @description Sequence class constructor
 * @param const std::string& value to convert to primitive
**/

template <>
Sequence<std::string>::Sequence(const std::string& value) : Objectid(value) {
}

/**
 * @function setValue
 * @description save new value on database and set new value to object
 * @param const std::string& oid associated to this value
 * @returns bool: true if set database operation succeeded
**/

template <>
bool Sequence<std::string>::setValue(const std::string& oid, const std::string& value) {
  //Has no value, just return
  return true;
}

/**
 * @function getValue
 * @returns std::string: object id has no value
**/

template <>
std::string Sequence<std::string>::getValue() {
  return "";
}

/**
 * @function getPrintableValue
 * @description get printable value version
 * @returns std::string
**/

template <>
std::string Sequence<std::string>::getPrintableValue() {

  return "";
}

}
