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

#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <mibscheduler/event.hpp>
#include <mibscheduler/scheduledevent.hpp>
#include <murmure/mibtable.hpp>

#include <thread>

namespace murmure {

class Scheduler {
public:
  Scheduler();
  Scheduler(Mibtable* mTable);
  ~Scheduler();
  bool loadEvents();
  int fetchAndExec(std::string oid, EventMode mode);
  bool startScheduler();
  //Scheduler setups
  bool parseScheduling(std::string oid, EventMode mode, std::vector<std::string> commandList, std::string* error, int timeout = 0);
  bool parseScheduling(std::string filename, std::string* error);
  bool clearEvents();
  bool dumpScheduling(std::string dumpFile = "");

private:
  static int runScheduler();
  bool addEvent(std::string oid, EventMode mode, std::vector<std::string> commandList, int timeout = 0);
  static std::vector<Event*> events;
  static std::vector<ScheduledEvent*> scheduledEvents;
  Mibtable* mibtable;
  static std::thread* schedulerThread;
  static bool stopCalled;
};
} // namespace murmure

#endif