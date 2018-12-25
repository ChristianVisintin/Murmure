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
 * @function calcRelativeTimeout
 * @description calc relative timeout based on elapsed time and timeout
 * @param time_t elapsedTime
 * @returns int calculated relative timeout
**/

int ScheduledEvent::calcRelativeTimeout(time_t elapsedTime) {

  this->relativeTimeout = this->timeout - (elapsedTime % this->timeout);
  return this->relativeTimeout;

}

/**
 * @function getTimeout
 * @description returns timeout class attribute
 * @returns int
**/

int ScheduledEvent::getTimeout() {
  return this->timeout;
}

/**
 * @function getRelativeTimeout
 * @description returns relative timeout class attribute
 * @returns int
**/

int ScheduledEvent::getRelativeTimeout() {
  return this->relativeTimeout;
}

/**
 * @function getOid
 * @description get oid private attribute
 * @returns std::string
**/

std::string ScheduledEvent::getOid() {
  return Event::getOid();
}

/**
 * @function getMode
 * @description get eventmode private attribute
 * @returns EventMode
**/

EventMode ScheduledEvent::getMode() {
  return Event::getMode();
}

/**
 * @function getModeName
 * @description returns mode string name
 * @returns std::string
**/

std::string ScheduledEvent::getModeName() {
  return Event::getModeName();
}

/**
 * @function getCommandList
 * @description get command list vector
 * @returns std::vector<std::string>
**/

std::vector<std::string> ScheduledEvent::getCommandList() {
  return Event::getCommandList();
}

/**
 * @function sortByTimeout
 * @description sort scheduled events by their timeout in seconds
 * @param ScheduledEvent* first ScheduledEvent to compare
 * @param ScheduledEvent* second ScheduledEvent to compare
 * @returns bool: true if this ScheduledEvent's timeout is < passed ScheduledEvent's timeout
**/

bool murmure::sortByTimeout(ScheduledEvent* firstEv, ScheduledEvent* secondEv) {
  return firstEv->getTimeout() < secondEv->getTimeout();
}

/**
 * @function sortByRelativeTimeout
 * @description sort scheduled events by their relative timeout in seconds
 * @param ScheduledEvent* first ScheduledEvent to compare
 * @param ScheduledEvent* second ScheduledEvent to compare
 * @returns bool: true if this ScheduledEvent's relative timeout is < passed ScheduledEvent's timeout
**/

bool murmure::sortByRelativeTimeout(ScheduledEvent* firstEv, ScheduledEvent* secondEv) {
  return firstEv->getRelativeTimeout() < secondEv->getRelativeTimeout();
}