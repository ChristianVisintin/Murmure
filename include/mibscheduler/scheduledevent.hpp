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

#ifndef SCHEDULEDEVENT_HPP
#define SCHEDULEDEVENT_HPP

#include <mibscheduler/event.hpp>
#include <mibscheduler/eventmode.hpp>
#include <string>
#include <vector>

namespace murmure {
class ScheduledEvent : public Event {
public:
  ScheduledEvent(std::string oid, EventMode evMode, std::vector<std::string> commandList, int timeout);
  int getTimeout();

private:
  int timeout;
};
} // namespace murmure

#endif