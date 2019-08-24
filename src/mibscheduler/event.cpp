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

#include <mibscheduler/event.hpp>

namespace murmure {

/**
 * @function Event
 * @description Event class constructor
 * @param std::string oid associated to event
 * @param EventMode event mode associated
 * @param std::vector<std::string> command commandList
**/

Event::Event(const std::string& oid, EventMode evMode, const std::vector<std::string>& commandList) {

  this->oid = oid;
  this->mode = evMode;
  this->commandList = commandList;
}

/**
 * @function executeCommands
 * @description execute commands associated to this event
 * @returns int: amount of executed commands
**/

int Event::executeCommands() {

  int commandAmount = 0;

  for (auto& command : commandList) {
    system(command.c_str());
    commandAmount++;
  }

  return commandAmount;
}

/**
 * @function getOid
 * @description get oid private attribute
 * @returns std::string
**/

std::string Event::getOid() {
  return this->oid;
}

/**
 * @function getMode
 * @description get eventmode private attribute
 * @returns EventMode
**/

EventMode Event::getMode() {
  return this->mode;
}

/**
 * @function getModeName
 * @description returns mode string name
 * @returns std::string
**/

std::string Event::getModeName() {

  if (mode == EventMode::AUTO) {
    return EVENTMODE_AUTO;
  } else if (mode == EventMode::GET) {
    return EVENTMODE_GET;
  } else if (mode == EventMode::INIT) {
    return EVENTMODE_INIT;
  } else if (mode == EventMode::SET) {
    return EVENTMODE_SET;
  } else {
    return "?";
  }
}

/**
 * @function getCommandList
 * @description get command list vector
 * @returns std::vector<std::string>
**/

std::vector<std::string> Event::getCommandList() {
  return this->commandList;
}

}
