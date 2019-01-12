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
