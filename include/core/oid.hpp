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

#ifndef OID_HPP
#define OID_HPP

#include <core/accessmode.hpp>
#include <core/primitives/primitive.hpp>
#include <string>

namespace murmure {

class Oid {

public:
  Oid(std::string oid, std::string type, std::string value, int access, std::string name = nullptr);
  ~Oid();
  std::string getOid();
  std::string getType();
  std::string getPrimitiveType();
  std::string getName();
  std::string getPrintableValue();
  AccessMode getAccessMode();
  int getAccessModeInteger();
  bool setValue(std::string printableValue);
  bool isTypeValid();

private:
  std::string oid;           //OID which identifies this instance
  std::string name;          //optional name for OID
  AccessMode accessMode;     //Access level for OID
  std::string dataType;      //Type string
  std::string primitiveType; //Primitive type string
  void* data;                //Wrapper of value (void pointer to Primitive extension class)
};

bool sortByOid(Oid* firstOid, Oid* secondOid);

} // namespace murmure

#endif
