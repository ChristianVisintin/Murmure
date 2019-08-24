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

#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <mibscheduler/event.hpp>
#include <mibscheduler/scheduledevent.hpp>
#include <core/mibtable.hpp>

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
