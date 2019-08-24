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

#include <core/modulefacade.hpp>
#include <core/oid.hpp>
#include <core/primitives/counter.hpp>
#include <core/primitives/gauge.hpp>
#include <core/primitives/integer.hpp>
#include <core/primitives/ipaddress.hpp>
#include <core/primitives/objectid.hpp>
#include <core/primitives/octet.hpp>
#include <core/primitives/sequence.hpp>
#include <core/primitives/string.hpp>
#include <core/primitives/timeticks.hpp>

#include <algorithm>
#include <vector>

namespace murmure {

/**
 * @function Oid
 * @description Oid class constructor
 * @param std::string oid: oid string
 * @param std::string type: type string
 * @param int access: access level integer (as stored into database)
 * @param std::string: optional oid name
**/

Oid::Oid(const std::string& oid, const std::string& type, const std::string& value, const int access, const std::string& name /* = "" */) {

  //Set OID string
  this->oid = oid;

  //Init data to nullptr
  data = nullptr;

  //Convert type to upper case
  std::string upperType = type;
  std::transform(upperType.begin(), upperType.end(), upperType.begin(), ::toupper);
  this->dataType = type;
  this->primitiveType = type;
  //Based on type instance new Primitive type
  if (type == PRIMITIVE_COUNTER) {
    data = new Counter<unsigned int>(value);
  } else if (type == PRIMITIVE_GAUGE) {
    data = new Gauge<unsigned int>(value);
  } else if (type == PRIMITIVE_INTEGER) {
    data = new Integer<int>(value);
  } else if (type == PRIMITIVE_IPADRRESS) {
    data = new IPAddress<std::string>(value);
  } else if (type == PRIMITIVE_OBJECTID) {
    data = new Objectid<std::string>(value);
  } else if (type == PRIMITIVE_OCTET) {
    data = new Octet<uint8_t*>(value);
  } else if (type == PRIMITIVE_SEQUENCE) {
    data = new Sequence<std::string>(value);
  } else if (type == PRIMITIVE_STRING) {
    data = new String<std::string>(value);
  } else if (type == PRIMITIVE_TIMETICKS) {
    data = new Timeticks<unsigned int>(value);
  } else {
    //Could be a module, in case instance data as moduleFacade
    data = new ModuleFacade();
    ModuleFacade* module = reinterpret_cast<ModuleFacade*>(this->data);
    //Try to instance module
    if (module->findModule(this->dataType)) {
      //Module has been found!
      //Get primitive type from module
      this->primitiveType = module->getPrimitiveType();
      //Set value
      module->setValue(value);
    } else {
      //Module hasn't been found, delete data and reset to nullptr
      delete module;
      data = nullptr;
    }
  }

  //Set access mode
  switch (access) {
  case ACCESSMODE_NOTACCESSIBLE:
    this->accessMode = AccessMode::NOT_ACCESSIBLE;
    break;
  case ACCESSMODE_READONLY:
    this->accessMode = AccessMode::READONLY;
    break;
  case ACCESSMODE_READCREATE:
    this->accessMode = AccessMode::READCREATE;
    break;
  case ACCESSMODE_READWRITE:
    this->accessMode = AccessMode::READWRITE;
    break;
  }
  //Set name
  this->name = name;
}

/**
 * @function ~Oid
 * @description Oid class destructor
**/

Oid::~Oid() {
  //Delete data if exists
  if (data != nullptr) {
    if (this->dataType == PRIMITIVE_COUNTER) {
      Counter<unsigned int>* realData = reinterpret_cast<Counter<unsigned int>*>(this->data);
      delete realData;
    } else if (this->dataType == PRIMITIVE_GAUGE) {
      Gauge<unsigned int>* realData = reinterpret_cast<Gauge<unsigned int>*>(this->data);
      delete realData;
    } else if (this->dataType == PRIMITIVE_INTEGER) {
      Integer<int>* realData = reinterpret_cast<Integer<int>*>(this->data);
      delete realData;
    } else if (this->dataType == PRIMITIVE_IPADRRESS) {
      IPAddress<std::string>* realData = reinterpret_cast<IPAddress<std::string>*>(this->data);
      delete realData;
    } else if (this->dataType == PRIMITIVE_OBJECTID) {
      Objectid<std::string>* realData = reinterpret_cast<Objectid<std::string>*>(this->data);
      delete realData;
    } else if (this->dataType == PRIMITIVE_OCTET) {
      Octet<uint8_t*>* realData = reinterpret_cast<Octet<uint8_t*>*>(this->data);
      delete realData;
    } else if (this->dataType == PRIMITIVE_SEQUENCE) {
      Sequence<std::string>* realData = reinterpret_cast<Sequence<std::string>*>(this->data);
      delete realData;
    } else if (this->dataType == PRIMITIVE_STRING) {
      String<std::string>* realData = reinterpret_cast<String<std::string>*>(this->data);
      delete realData;
    } else if (this->dataType == PRIMITIVE_TIMETICKS) {
      Timeticks<unsigned int>* realData = reinterpret_cast<Timeticks<unsigned int>*>(this->data);
      delete realData;
    } else {
      ModuleFacade* module = reinterpret_cast<ModuleFacade*>(this->data);
      delete module;
    }
  }
}

/**
 * @function getOid
 * @description returns oid string
 * @returns std::string
**/

std::string Oid::getOid() {
  return this->oid;
}

/**
 * @function getType
 * @description returns type string
 * @returns std::string
**/

std::string Oid::getType() {
  return this->dataType;
}

/**
 * @function getPrimitiveType
 * @description returns primitive type string
 * @returns std::string
**/

std::string Oid::getPrimitiveType() {
  return this->primitiveType;
}

/**
 * @function getName
 * @description returns oid name
 * @returns std::string
**/

std::string Oid::getName() {
  return this->name;
}

/**
 * @function getName
 * @description returns data printable value
 * @returns std::string
**/

std::string Oid::getPrintableValue() {
  if (this->dataType == PRIMITIVE_COUNTER) {
    Counter<unsigned int>* realData = reinterpret_cast<Counter<unsigned int>*>(this->data);
    return realData->getPrintableValue();
  } else if (this->dataType == PRIMITIVE_GAUGE) {
    Gauge<unsigned int>* realData = reinterpret_cast<Gauge<unsigned int>*>(this->data);
    return realData->getPrintableValue();
  } else if (this->dataType == PRIMITIVE_INTEGER) {
    Integer<int>* realData = reinterpret_cast<Integer<int>*>(this->data);
    return realData->getPrintableValue();
  } else if (this->dataType == PRIMITIVE_IPADRRESS) {
    IPAddress<std::string>* realData = reinterpret_cast<IPAddress<std::string>*>(this->data);
    return realData->getPrintableValue();
  } else if (this->dataType == PRIMITIVE_OBJECTID) {
    Objectid<std::string>* realData = reinterpret_cast<Objectid<std::string>*>(this->data);
    return realData->getPrintableValue();
  } else if (this->dataType == PRIMITIVE_OCTET) {
    Octet<uint8_t*>* realData = reinterpret_cast<Octet<uint8_t*>*>(this->data);
    return realData->getPrintableValue();
  } else if (this->dataType == PRIMITIVE_SEQUENCE) {
    Sequence<std::string>* realData = reinterpret_cast<Sequence<std::string>*>(this->data);
    return realData->getPrintableValue();
  } else if (this->dataType == PRIMITIVE_STRING) {
    String<std::string>* realData = reinterpret_cast<String<std::string>*>(this->data);
    return realData->getPrintableValue();
  } else if (this->dataType == PRIMITIVE_TIMETICKS) {
    Timeticks<unsigned int>* realData = reinterpret_cast<Timeticks<unsigned int>*>(this->data);
    return realData->getPrintableValue();
  } else {
    ModuleFacade* module = reinterpret_cast<ModuleFacade*>(this->data);
    return module->getPrintableValue();
  }
}

/**
 * @function getName
 * @description returns oid access mode
 * @returns AccessMode
**/

AccessMode Oid::getAccessMode() {
  return this->accessMode;
}

/**
 * @function getAccessModeInteger
 * @description returns the integer associated to access mode (as to be stored in the database)
 * @returns int
**/

int Oid::getAccessModeInteger() {
  if (this->accessMode == AccessMode::NOT_ACCESSIBLE) {
    return ACCESSMODE_NOTACCESSIBLE;
  } else if (this->accessMode == AccessMode::READONLY) {
    return ACCESSMODE_READONLY;
  } else if (this->accessMode == AccessMode::READCREATE) {
    return ACCESSMODE_READCREATE;
  } else if (this->accessMode == AccessMode::READWRITE) {
    return ACCESSMODE_READWRITE;
  } else {
    return ACCESSMODE_NOTACCESSIBLE;
  }
}

/**
 * @function setValue
 * @description Set primitive value from a printable received by net-SNMP
 * @param std::string printable value
 * @returns bool: true if set successfully (false if operation on database failed)
**/

bool Oid::setValue(std::string printableValue) {

  //Value set operation is managed by Primitive extended class
  if (this->dataType == PRIMITIVE_COUNTER) {
    Counter<unsigned int>* realData = reinterpret_cast<Counter<unsigned int>*>(this->data);
    return realData->setValue(this->oid, printableValue);
  } else if (this->dataType == PRIMITIVE_GAUGE) {
    Gauge<unsigned int>* realData = reinterpret_cast<Gauge<unsigned int>*>(this->data);
    return realData->setValue(this->oid, printableValue);
  } else if (this->dataType == PRIMITIVE_INTEGER) {
    Integer<int>* realData = reinterpret_cast<Integer<int>*>(this->data);
    return realData->setValue(this->oid, printableValue);
  } else if (this->dataType == PRIMITIVE_IPADRRESS) {
    IPAddress<std::string>* realData = reinterpret_cast<IPAddress<std::string>*>(this->data);
    return realData->setValue(this->oid, printableValue);
  } else if (this->dataType == PRIMITIVE_OBJECTID) {
    Objectid<std::string>* realData = reinterpret_cast<Objectid<std::string>*>(this->data);
    return realData->setValue(this->oid, printableValue);
  } else if (this->dataType == PRIMITIVE_OCTET) {
    Octet<uint8_t*>* realData = reinterpret_cast<Octet<uint8_t*>*>(this->data);
    return realData->setValue(this->oid, printableValue);
  } else if (this->dataType == PRIMITIVE_SEQUENCE) {
    Sequence<std::string>* realData = reinterpret_cast<Sequence<std::string>*>(this->data);
    return realData->setValue(this->oid, printableValue);
  } else if (this->dataType == PRIMITIVE_STRING) {
    String<std::string>* realData = reinterpret_cast<String<std::string>*>(this->data);
    return realData->setValue(this->oid, printableValue);
  } else if (this->dataType == PRIMITIVE_TIMETICKS) {
    Timeticks<unsigned int>* realData = reinterpret_cast<Timeticks<unsigned int>*>(this->data);
    return realData->setValue(this->oid, printableValue);
  } else {
    ModuleFacade* module = reinterpret_cast<ModuleFacade*>(this->data);
    return module->setValue(this->oid, printableValue);
  }
}

/**
 * @function isTypeValid
 * @description check if data is set, which means type has been correctly resolved
 * @returns bool: true if valid
*/

bool Oid::isTypeValid() {
  return (data != nullptr);
}

/**
 * @function sortByOid
 * @description sort oid instance by their oid (to use with sort)
 * @param Oid* first oid to compare
 * @param Oid* second oid to compare
 * @returns bool: true if this oid is < passed oid
**/

bool sortByOid(Oid* firstOid, Oid* secondOid) {
  return firstOid->getOid().compare(secondOid->getOid()) < 0;
}

}
