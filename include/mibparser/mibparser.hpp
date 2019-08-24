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

#ifndef MIBPARSER_HPP
#define MIBPARSER_HPP

#include <murmure/mibtable.hpp>

namespace murmure {

class Mibparser {

public:
  Mibparser();
  ~Mibparser();
  bool parseMibFile(std::string rootOid, std::string mibfile);

private:
  bool parseLine(std::string line);
  bool commitPreviousOid();
  //Parsing methods
  bool handleModuleIdentity(std::string line);
  bool handleInlineObject(std::string line);
  bool handleObjectDeclaration(std::string line);
  bool handleObjectSyntax(std::string line);
  bool handleObjectStatus(std::string line);
  bool handleObjectAccess(std::string line);
  bool handleObjectGroup(std::string line);
  bool handleSequence(std::string line);
  //Mibparser attributes
  Mibtable* mibtable;
  std::string rootOidStr;
  Oid* rootOid;
  //"Current" oid attributes
  std::string currentOid;
  std::string currentType;
  int currentAccessMode;
  std::string currentName;
  std::string currentStatus;
  bool oidSaved; //Has current OID already been saved to mibtable

};
} // namespace murmure

#endif
