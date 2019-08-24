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

#include <core/modules/opaque.hpp>
#include <core/primitives/string.hpp>

namespace murmure {

/**
 * @function Opaque
 * @description Opaque class constructor
 * NOTE: set primitive to nullptr; set primitiveType to the primitive we'll use
**/

Opaque::Opaque() {
  primitive = nullptr;
  primitiveType = PRIMITIVE_STRING;
}

/**
 * @function ~Opaque
 * @description Opaque class destructor
**/

Opaque::~Opaque() {
  if (primitive != nullptr) {
    String<std::string>* primitivePtr = reinterpret_cast<String<std::string>*>(primitive);
    delete primitivePtr;
  }
  primitive = nullptr;
}

/**
 * @function setValue
 * @description initialize primitive and set its value
 * @param const std::string&
 * @returns bool
 * NOTE: must be called in initialization only
**/

bool Opaque::setValue(const std::string& value) {
  if (primitive == nullptr) {
    primitive = new String<std::string>(value);
  }
  return true;
}

/**
 * @function setValue
 * @description update primitive value associated to this module
 * @param const std::string&
 * @param const std::string&
 * @returns bool
**/

bool Opaque::setValue(const std::string& oid, const std::string& value) {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return false;
  }
  String<std::string>* primitivePtr = reinterpret_cast<String<std::string>*>(primitive);
  return primitivePtr->setValue(oid, value);
}

/**
 * @function getPrintableValue
 * @description get printable value from its primitive
 * @returns std::string
**/

std::string Opaque::getPrintableValue() {
  //Check if primitive is initialized
  if (primitive == nullptr) {
    return "";
  }
  String<std::string>* primitivePtr = reinterpret_cast<String<std::string>*>(primitive);
  return primitivePtr->getPrintableValue();
}

/**
 * @function getPrimitiveType
 * @description get primitive type name associated to this module
 * @returns std::string
**/

std::string Opaque::getPrimitiveType() {
  return primitiveType;
}

}
