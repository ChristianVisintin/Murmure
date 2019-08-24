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

#ifndef SCHEDULEDEVENT_HPP
#define SCHEDULEDEVENT_HPP

#include <mibscheduler/event.hpp>
#include <mibscheduler/eventmode.hpp>
#include <string>
#include <vector>

namespace murmure {
class ScheduledEvent : public Event {
public:
  ScheduledEvent(const std::string& oid, EventMode evMode, const std::vector<std::string>& commandList, int timeout);
  int executeCommands();
  int calcRelativeTimeout(time_t elapsedTime);
  std::string getOid();
  EventMode getMode();
  std::string getModeName();
  std::vector<std::string> getCommandList();
  int getTimeout();
  int getRelativeTimeout();

private:
  int timeout;
  int relativeTimeout;
};

bool sortByTimeout(ScheduledEvent* firstEv, ScheduledEvent* secondEv);
bool sortByRelativeTimeout(ScheduledEvent* firstEv, ScheduledEvent* secondEv);

} // namespace murmure

#endif
