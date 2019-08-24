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

#include <murmure/modulefacade.hpp>

/* @!::AUTO_INCLUDES::@! */

#include <algorithm>

using namespace murmure;

std::map<std::string, Module* (*)()> murmure::ModuleFacade::modules;
bool murmure::ModuleFacade::modulesLoaded;

//Get instance of module function
template <typename M>
Module* getModuleInstance() { return new M; };

/**
 * @function ModuleFacade
 * @description ModuleFacade class constructor
**/

ModuleFacade::ModuleFacade() {
  module = nullptr;
  //If modules hasn't been loaded yet, load modules
  if (!modulesLoaded) {
    loadModules();
  }
}

/**
 * @function ~ModuleFacade
 * @description ModuleFacade class destructor
**/

ModuleFacade::~ModuleFacade() {
  //Delete module if exists
  if (module != nullptr) {
    delete module;
  }
  module = nullptr;
}

/**
 * @function findModule
 * @description starting from the typename read from MIB, check if module exists. If exists instance new Module
 * @param std::string typeName
 * @returns bool: true if module has been found and successfully initialized
**/

bool ModuleFacade::findModule(std::string typeName) {

  //Check if typename is in module list
  std::map<std::string, Module* (*)()>::iterator moduleIterator;
  for (moduleIterator = modules.begin(); moduleIterator != modules.end(); moduleIterator++) {
    if (typeName == moduleIterator->first) {
      //Module found
      module = modules[typeName]();
      return true;
    }
  }
  //Module not found
  return false;
}

/**
 * @function setValue
 * @description set value to module instance
 * @param std::string value
 * @returns bool: true if value was successfully set
 * NOTE: used in initialization of ModuleFacade
**/

bool ModuleFacade::setValue(std::string value) {
  if (module == nullptr) {
    return false;
  }
  return module->setValue(value);
}

/**
 * @function setValue
 * @description set value to module instance and save it into database
 * @param std::string oid
 * @param std::string value
 * @returns bool: true if value was successfully set
**/

bool ModuleFacade::setValue(std::string oid, std::string value) {
  if (module == nullptr) {
    return false;
  }
  return module->setValue(oid, value);
}

/**
 * @function getPrintableValue
 * @description get printable value from module instance
 * @returns std::string printable value
**/

std::string ModuleFacade::getPrintableValue() {
  if (module == nullptr) {
    return "";
  }
  return module->getPrintableValue();
}

/**
 * @function getPrimitiveType
 * @description returns the primitive type name associated to module
 * @returns std::string
**/

std::string ModuleFacade::getPrimitiveType() {
  if (module == nullptr) {
    return "";
  }
  return module->getPrimitiveType();
}

/**
 * @function loadModules
 * @description load modules into 'modules' hash map
 * NOTE: this function is @!static
 * NOTE: modules["ModuleClass"] = &getModuleInstance<ModuleClass>;
 * NOTE: part of this function identified by 'AUTO_MODULES' is automatically generated during compilation
**/

void ModuleFacade::loadModules() {

  //Clear map
  modules.clear();

  //Load modules into hash map
  /* @!::AUTO_MODULES::@! */

  //Set modules loaded to true
  modulesLoaded = true;
  return;
}
