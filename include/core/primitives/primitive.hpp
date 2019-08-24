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

#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <string>

namespace murmure {

//Data string definition
#define PRIMITIVE_COUNTER "COUNTER"
#define PRIMITIVE_GAUGE "GAUGE"
#define PRIMITIVE_INTEGER "INTEGER"
#define PRIMITIVE_IPADRRESS "IPADDRESS"
#define PRIMITIVE_OBJECTID "OBJECT"
#define PRIMITIVE_OCTET "OCTET"
#define PRIMITIVE_SEQUENCE "SEQUENCE"
#define PRIMITIVE_STRING "STRING"
#define PRIMITIVE_TIMETICKS "TIMETICKS"

//Template for generic primitive value
template <typename primitiveType>

/**
 * Interface for all primitives types
 * 
 * All primitives must implement setValue, which converts a string (passed by net-snmp)
 * to primitiveType. Also getPrintableValue must be implemented, which converts the data
 * back to a printable value. getValue just returns the value
**/

class Primitive {
public:
  virtual ~Primitive() {};
  virtual bool setValue(const std::string& oid, const std::string& value) = 0;
  virtual primitiveType getValue() = 0;
  virtual std::string getPrintableValue() = 0;

protected:
  primitiveType value;
};
} // namespace murmure

#endif
