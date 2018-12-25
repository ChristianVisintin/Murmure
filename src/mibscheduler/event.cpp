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

#include <mibscheduler/event.hpp>

using namespace murmure;

/**
 * @function Event
 * @description Event class constructor
 * @param std::string oid associated to event
 * @param EventMode event mode associated
 * @param std::vector<std::string> command commandList
**/

Event::Event(std::string oid, EventMode evMode, std::vector<std::string> commandList) {

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

  for (auto command : commandList) {
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