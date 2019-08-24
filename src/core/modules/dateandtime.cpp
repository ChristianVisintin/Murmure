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

#include <murmure/modules/dateandtime.hpp>
#include <murmure/primitives/octet.hpp>

using namespace murmure;

/**
 * @function DateAndTime
 * @description DateAndTime class constructor
 * NOTE: set primitive to nullptr; set primitiveType to the primitive we'll use
**/

DateAndTime::DateAndTime() {
  primitive = nullptr;
  primitiveType = PRIMITIVE_OCTET;
}

/**
 * @function ~DateAndTime
 * @description DateAndTime class destructor
**/

DateAndTime::~DateAndTime() {
  if (primitive != nullptr) {
    Octet<uint8_t*>* primitivePtr = reinterpret_cast<Octet<uint8_t*>*>(primitive);
    delete primitivePtr;
  }
  primitive = nullptr;
}

/**
 * @function setValue
 * @description initialize primitive and set its value
 * @param std::string
 * @returns bool
 * NOTE: must be called in initialization only
**/

bool DateAndTime::setValue(std::string value) {
  if (primitive == nullptr) {
    primitive = new Octet<uint8_t*>(value);
  }
  return true;
}

/**
 * @function setValue
 * @description update primitive value associated to this module
 * @param std::string
 * @param std::string
 * @returns bool
**/

bool DateAndTime::setValue(std::string oid, std::string value) {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return false;
  }
  //Check length for DateAndTime (must be 8 bytes or 11 bytes @! we use ascii repr of bytes, so use doubled values)
  if (value.length() != 16 && value.length() != 22) {
    return false;
  }
  Octet<uint8_t*>* primitivePtr = reinterpret_cast<Octet<uint8_t*>*>(primitive);
  return primitivePtr->setValue(oid, value);
}

/**
 * @function getPrintableValue
 * @description get printable value from its primitive
 * @returns std::string
**/

std::string DateAndTime::getPrintableValue() {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return "";
  }
  Octet<uint8_t*>* primitivePtr = reinterpret_cast<Octet<uint8_t*>*>(primitive);
  return primitivePtr->getPrintableValue();
}

/**
 * @function getPrimitiveType
 * @description get primitive type name associated to this module
 * @returns std::string
**/

std::string DateAndTime::getPrimitiveType() {
  return primitiveType;
}
