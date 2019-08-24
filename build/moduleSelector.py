#!/usr/bin/env python3

"""
  Murmure - Net-SNMP MIB Versatile Extender
  Developed by Christian Visintin

MIT License
Copyright (c) 2019 Christian Visintin
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import os
from sys import exit
import itertools
from shutil import copyfile, move

useInquirer = True
try:
  import inquirer
except ImportError:
  useInquirer = False


ROOT_PATH = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
INCLUDE_PATH = ROOT_PATH + "/include/"
SRC_PATH = ROOT_PATH + "/src/"
MODULES_PATH = INCLUDE_PATH + "/core/modules/"
MODULEFACADE_PATH = SRC_PATH + "/core/modulefacade.cpp"
MODULEFACADE_BAK_PATH = SRC_PATH + "/core/modulefacade.orig.cpp"
MODULES_SRC_LIST = "/build/modulesSrc.list"
MAKEFILE_AM = SRC_PATH + "Makefile.am"
MAKEFILE_AM_BAK = SRC_PATH + "Makefile.am.bak"

if __name__ == "__main__":
  #List modules
  #Restore modulefacade if exists
  if os.path.isfile(MODULEFACADE_BAK_PATH):
    move(MODULEFACADE_BAK_PATH, MODULEFACADE_PATH)
    print("Restored %s" % MODULEFACADE_PATH)
  #Restore makefile if exists
  if os.path.isfile(MAKEFILE_AM_BAK):
    move (MAKEFILE_AM_BAK, MAKEFILE_AM)
    print("Restored %s" % MAKEFILE_AM)
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
    moduleIncludes += "#include <core/modules/%s.hpp>\n" % include
  #Prepare instances
  moduleInstances = ""
  for datatype, classname in itertools.product(moduleTypeList, moduleClassList):
    moduleInstances += "modules[\"%s\"] = &getModuleInstance<%s>;\n" % (datatype, classname)
  
  #Create modulefacade.orig.cpp
  try:
    copyfile(MODULEFACADE_PATH, MODULEFACADE_BAK_PATH)
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
  #Create backup of makefile
  copyfile(MAKEFILE_AM, MAKEFILE_AM_BAK)
  #Print sources in Makefile
  sourcesFiles = "murmure_SOURCES += "
  for module in moduleSelectionList:
    sourcesFiles += "core/modules/" + module + ".cpp "
  try:
    hnd = open(MAKEFILE_AM, 'a')
    hnd.write("%s\n" % sourcesFiles)
  except IOError as error:
    print(str(error))
    exit(1)
  hnd.close()
  print("Modules loaded successfully")
  exit(0)
