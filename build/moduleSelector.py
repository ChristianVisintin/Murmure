#!/usr/bin/env python3

"""
 Murmure - Net-SNMP MIB Versatile Extender
 Developed by Christian Visintin

	Copyright (C) 2018 - 2019 Christian Visintin	This file is part of "Murmure"
 Murmure is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 Murmure is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

You should have received a copy of the GNU General Public License
"""

import os
from sys import exit
import itertools
from shutil import copyfile

useInquirer = True
try:
  import inquirer
except ImportError:
  useInquirer = False


ROOT_PATH = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
INCLUDE_PATH = ROOT_PATH + "/include/"
SRC_PATH = ROOT_PATH + "/src/"
MODULES_PATH = INCLUDE_PATH + "/murmure/modules/"
MODULEFACADE_PATH = SRC_PATH + "/murmure/modulefacade.cpp"
MODUELEFACADE_BAK_PATH = SRC_PATH + "/murmure/modulefacade.orig.cpp"
MODULES_SRC_LIST = "/build/modulesSrc.list"

if __name__ == "__main__":
  #List modules
  moduleListFiles = os.listdir(MODULES_PATH)
  moduleList = []
  moduleSelectionList = []
  for module in moduleListFiles:
    if module != "module.hpp":
      moduleList.append(os.path.splitext(module)[0])

  #Let user select
  if useInquirer:
    modules = [inquirer.Checkbox(
      'modules',
      message="Select modules",
      choices=moduleList,
    )]
    moduleSelectionList = inquirer.prompt(modules)['modules']
  else: #Inquirer not supported
    for module in moduleList:
      choice = input("Module <%s> [Y/n]: " % module)
      if choice.upper() == "Y":
        moduleSelectionList.append(module)


  moduleClassList = [] #Contains class list of selected modules
  moduleTypeList = [] #Contains types list of selected modules
  #Get modules name
  for module in moduleSelectionList:
    #For each module, read its metadata
    try: 
      hnd = open("%s/%s.hpp" % (MODULES_PATH, module), "r")
      data = hnd.read()
    except IOError as error:
      print(str(error))
      exit(1)
    else:
      hnd.close()
    for line in data.splitlines():
      if line.find("@name") > -1:
        moduleTypeList.append(line.split(" ")[3])
        continue
      if line.startswith("class"):
        moduleClassList.append(line.split(" ")[1])
        break
  
  #Prepare include list
  moduleIncludes = ""
  for include in moduleSelectionList:
    moduleIncludes += "#include <murmure/modules/%s.hpp>\n" % include
  #Prepare instances
  moduleInstances = ""
  for datatype, classname in itertools.product(moduleTypeList, moduleClassList):
    moduleInstances += "modules[\"%s\"] = &getModuleInstance<%s>;\n" % (datatype, classname)
  
  #Create modulefacade.orig.cpp
  try:
    copyfile(MODULEFACADE_PATH, MODUELEFACADE_BAK_PATH)
  except IOError as error:
    print(str(error))
    exit(1)
  #Update modulefacade.cpp
  try:
    hnd = open(MODULEFACADE_PATH, 'r+')
  except IOError as error:
    print(str(error))
    exit(1)
  else:
    moduleFacade = hnd.read()
    hnd.close()
    hnd = open(MODULEFACADE_PATH, 'w')
    fileContent = ""
    #Iterate over lines
    for line in moduleFacade.splitlines():
      #Replace "auto lines"
      if line.find("::AUTO_INCLUDES::") > -1:
        fileContent += moduleIncludes
      elif line.find("::AUTO_MODULES::") > -1:
        fileContent += moduleInstances
      else:
        fileContent += line + "\n"
    #Update file
    hnd.write(fileContent)
    hnd.close()
  #Print sources in build
  sourcesFiles = ""
  for module in moduleSelectionList:
    sourcesFiles += "murmure/mdoules" + module + ".cpp "
  try:
    hnd = open("%s/%s" % (ROOT_PATH, MODULES_SRC_LIST), 'w')
    hnd.write(sourcesFiles)
  except IOError as error:
    print(str(error))
    exit(1)
  hnd.close()
  print("Modules loaded successfully")
  exit(0)
