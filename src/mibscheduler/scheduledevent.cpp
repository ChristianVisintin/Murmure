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

#include <mibscheduler/scheduledevent.hpp>

using namespace murmure;

/**
 * @function ScheduledEvent
 * @description ScheduledEvent class constructor
 * @param std::string oid
 * @param EventMode event evMode
 * @param std::vector<std::string> command commandList
 * @param int command timeout
**/

ScheduledEvent::ScheduledEvent(std::string oid, EventMode evMode, std::vector<std::string> commandList, int timeout) : Event(oid, evMode, commandList) {
  this->timeout = timeout;
}

/**
 * @function executeCommands
 * @description execute commands associated to this event
 * @returns int amount of executed commands
**/

int ScheduledEvent::executeCommands() {
  return Event::executeCommands();
}

/**
 * @function getTimeout
 * @description returns timeout class attribute
 * @returns int
**/

int ScheduledEvent::getTimeout() {
  return this->timeout;
}
