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

#include <core/modules/unsigned32.hpp>
#include <core/primitives/gauge.hpp>

using namespace murmure;

/**
 * @function Unsigned32
 * @description Unsigned32 class constructor
 * NOTE: set primitive to nullptr; set primitiveType to the primitive we'll use
**/

Unsigned32::Unsigned32() {
  primitive = nullptr;
  primitiveType = PRIMITIVE_GAUGE;
}

/**
 * @function ~Unsigned32
 * @description Unsigned32 class destructor
**/

Unsigned32::~Unsigned32() {
  if (primitive != nullptr) {
    Gauge<unsigned int>* primitivePtr = reinterpret_cast<Gauge<unsigned int>*>(primitive);
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

bool Unsigned32::setValue(std::string value) {
  if (primitive == nullptr) {
    primitive = new Gauge<unsigned int>(value);
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

bool Unsigned32::setValue(std::string oid, std::string value) {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return false;
  }
  //Check value for Unsigned32
  if (std::stoi(value) > 4294967295) {
    return false;
  }
  Gauge<unsigned int>* primitivePtr = reinterpret_cast<Gauge<unsigned int>*>(primitive);
  return primitivePtr->setValue(oid, value);
}

/**
 * @function getPrintableValue
 * @description get printable value from its primitive
 * @returns std::string
**/

std::string Unsigned32::getPrintableValue() {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return "";
  }
  Gauge<unsigned int>* primitivePtr = reinterpret_cast<Gauge<unsigned int>*>(primitive);
  return primitivePtr->getPrintableValue();
}

/**
 * @function getPrimitiveType
 * @description get primitive type name associated to this module
 * @returns std::string
**/

std::string Unsigned32::getPrimitiveType() {
  return primitiveType;
}