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

#include <mibscheduler/scheduler.hpp>
#include <utils/databasefacade.hpp>
#include <utils/logger.hpp>
#include <utils/strutils.hpp>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>

#define COMPONENT "Scheduler"

using namespace murmure;

std::vector<Event*> murmure::Scheduler::events;
std::vector<ScheduledEvent*> murmure::Scheduler::scheduledEvents;
std::thread* murmure::Scheduler::schedulerThread;
bool murmure::Scheduler::stopCalled;

/**
 * @function Scheduler
 * @description base Scheduler class constructor
**/

Scheduler::Scheduler() {
  schedulerThread = nullptr;
  stopCalled = false;
}

/**
 * @function Scheduler
 * @description with mibTable Scheduler class constructor
 * @param Mibtable*
**/

Scheduler::Scheduler(Mibtable* mTable) {
  mibtable = mTable;
  schedulerThread = nullptr;
  stopCalled = false;
}

/**
 * @function ~Scheduler
 * @description Scheduler class destructor
**/

Scheduler::~Scheduler() {

  stopCalled = true;

  if (schedulerThread != nullptr) {
    //Join thread before deleting it
    schedulerThread->join();
    //Delete thread
    delete schedulerThread;
    schedulerThread = nullptr;
  }

  //Free event objects
  for (auto& event : events) {
    delete event;
  }

  for (auto& event : scheduledEvents) {
    delete event;
  }

  //Do not free mibtable, since it's freed in main
}

/**
 * @function loadEvents
 * @description load events from database
 * @returns bool true if loading succeeded
**/

bool Scheduler::loadEvents() {

  std::string errorString;
  //Select events from database

  //Init vector of row; each vector identifies a row, which is a vector of strings,
  //which identify columns
  std::vector<std::vector<std::string>> tableEntries;
  std::string query = "SELECT event_id, oid, mode, timeout FROM scheduled_events;";
  if (!database::select(&tableEntries, query, errorString)) {
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  size_t eventListSize = 0;
  size_t scheduledEventListSize = 0;

  //Fetch rows
  for (auto& row : tableEntries) {
    if (row.size() != 4) {
      logger::log(COMPONENT, LOG_ERROR, "Unexpected scheduled_event row size");
      return false;
    }

    std::string evId = row.at(0);
    std::string oid = row.at(1);
    std::string mode = row.at(2);
    //Select commands for this event sorting by execution_order asc
    std::vector<std::vector<std::string>> commandListRows;
    query = "SELECT command FROM events_commands WHERE event_id = \"" + evId + "\" ORDER BY execution_order ASC;";
    if (!database::select(&commandListRows, query, errorString)) {
      logger::log(COMPONENT, LOG_ERROR, errorString);
      return false;
    }
    //We have our commands in commandList row 0 now
    if (commandListRows.size() == 0) {
      std::stringstream logS;
      logS << "Event_id " << evId << " has no command associated";
      logger::log(COMPONENT, LOG_WARN, logS.str());
      continue;
    }
    //Okay, we have our command list now
    std::vector<std::string> commandList;
    for (auto& row : commandListRows) {
      if (row.size() > 0) {
        commandList.push_back(row.at(0));
      }
    }

    //Check mode, based on it we'll insert our event in a different vector
    if (mode == EVENTMODE_AUTO) {
      //Scheduledevent vector
      int timeout = std::stoi(row.at(3)); //Get timeout, which is 4th arg
      //Instance new scheduledEvent
      ScheduledEvent* newEv = new ScheduledEvent(oid, EventMode::AUTO, commandList, timeout);
      //Push to array new scheduledEvent element
      scheduledEvents.resize(scheduledEventListSize++);
      scheduledEvents.push_back(newEv);
    } else {
      //Event vector
      //Get eventMode enum
      EventMode evMode;
      if (mode == EVENTMODE_GET) {
        evMode = EventMode::GET;
      } else if (mode == EVENTMODE_SET) {
        evMode = EventMode::SET;
      } else if (mode == EVENTMODE_INIT) {
        evMode = EventMode::INIT;
      } else {
        std::stringstream logS;
        logS << "Unknown event mode " << mode;
        logger::log(COMPONENT, LOG_WARN, logS.str());
        continue;
      }
      //Instance new Event
      Event* newEv = new Event(oid, evMode, commandList);
      //Push to array new event element
      events.resize(eventListSize++);
      events.push_back(newEv);
    }
  }
  return true;
}

/**
 * @function fetchAndExec
 * @description search for event associated to provided oid and mode; if found execute associated commands
 * @param std::string oid to search
 * @param EventMode command mode
 * @returns int: amount of executed commands; 0 if no event is associated
**/

int Scheduler::fetchAndExec(const std::string& oid, EventMode mode) {

  //NOTE: Mode cannot be auto! Automatic event (scheduled events) can only be executed by the scheduler thread

  for (auto& event : events) {
    //If oid and mode mathces with event then execute associated events
    if (oid == event->getOid() && mode == event->getMode()) {
      logger::log(COMPONENT, LOG_INFO, "Executing events for OID " + event->getOid());
      return event->executeCommands();
    }
  }

  //No event associated to this oid
  return 0;
}

/**
 * @function startScheduler
 * @description start new scheduler thread and execute all INIT events
 * @returns bool: true if thread started
**/

bool Scheduler::startScheduler() {

  //Execute all INIT commands
  for (auto& event : events) {
    if (event->getMode() == EventMode::INIT) {
      event->executeCommands();
    }
  }

  //Start scheduler thread
  if (schedulerThread != nullptr) {
    //It's already running
    logger::log(COMPONENT, LOG_ERROR, "Could not start thread, Scheduler thread is already running!");
    return false;
  }

  schedulerThread = new std::thread(runScheduler);
  return true;
}

/**
 * @function parseScheduling
 * @description parse single scheduling entry, if valid save it into event database
 * @param std::string oid associated to new event
 * @param EventMode mode for event
 * @param std::vector<std::string> list of commands
 * @param std::string& error string pointer
 * @param int optional timeout (for scheduled events)
 * @returns bool: true if entry is valid
 * NOTE: an entry is valid if:
 * a) oid exists; 
 * b) if mode is 'AUTO' timeout is > 0;
 * c) command list has at least one element
**/

bool Scheduler::parseScheduling(const std::string& oid, EventMode mode, const std::vector<std::string>& commandList, std::string& error, int timeout /*= 0*/) {

  //Try to get oid
  if (mibtable->getOidByOid(oid) == nullptr) {
    error = "Could not find OID " + oid;
    return false;
  }

  //Check if timeout is set
  if (mode == EventMode::AUTO && timeout <= 0) {
    error = "Unset timeout for scheduled event";
    return false;
  }

  //Check if command list is empty
  if (commandList.size() == 0) {
    error = "Command list is empty";
    return false;
  }

  //Try to add new event to database
  return addEvent(oid, mode, commandList, timeout);
}

/**
 * @function parseScheduling
 * @description parse a scheduling file
 * @param std::string filename
 * @param std::string& error string pointer
 * @returns bool: true if parsing succeeded
 * NOTE: check documentation for scheduling file syntax
**/

bool Scheduler::parseScheduling(const std::string& filename, std::string& error) {

  std::ifstream schedulingStream;

  //Open scheduling stream
  schedulingStream.open(filename);
  if (!schedulingStream.is_open()) {
    error = "Could not open scheduling file " + filename;
    return false;
  }

  //Iterate over rows
  std::string line;
  int lineNumber = 1;
  while (getline(schedulingStream, line)) {

    //Tokenize line into tokens divided by ';'
    std::vector<std::string> eventTokens = strutils::split(line, ';');
    if (eventTokens.size() < 3) {
      std::stringstream errSs;
      errSs << "Error at line " << std::to_string(lineNumber) << ". Not enough arguments";
      error = errSs.str();
      return false;
    }
    //Get parts
    std::string oid = eventTokens.at(0);
    std::string modeStr = eventTokens.at(1);
    std::string cmdStr = eventTokens.at(2);
    int timeout = 0;
    EventMode mode;

    //Get real event mode
    if (modeStr == EVENTMODE_AUTO) {
      mode = EventMode::AUTO;
    } else if (modeStr == EVENTMODE_GET) {
      mode = EventMode::GET;
    } else if (modeStr == EVENTMODE_INIT) {
      mode = EventMode::INIT;
    } else if (modeStr == EVENTMODE_SET) {
      mode = EventMode::SET;
    } else {
      std::stringstream errSs;
      errSs << "Error at line " << std::to_string(lineNumber) << ". Unknown mode " << modeStr;
      error = errSs.str();
      return false;
    }

    //Check for timeout
    if (modeStr == EVENTMODE_AUTO && eventTokens.size() > 3) {
      timeout = std::stoi(eventTokens.at(3));
      //Check if timeout is valid
      if (timeout == 0) {
        std::stringstream errSs;
        errSs << "Error at line " << std::to_string(lineNumber) << ". Timeout 0 for event mode 'AUTO'";
        error = errSs.str();
        return false;
      }
    } else if (modeStr == EVENTMODE_AUTO && eventTokens.size() < 4) {
      std::stringstream errSs;
      errSs << "Error at line " << std::to_string(lineNumber) << ". Timeout not provided for event mode 'AUTO'";
      error = errSs.str();
      return false;
    }

    //Get command list
    std::vector<std::string> commandList = strutils::split(cmdStr, ',');

    //Add new event
    bool parseRes = parseScheduling(oid, mode, commandList, error, timeout);
    if (!parseRes) {
      std::stringstream errSs;
      errSs << "Error at line " << std::to_string(lineNumber) << " " << error;
      error = errSs.str();
      return false;
    }
    //Increase line counter
    lineNumber++;
  }

  schedulingStream.close();
  return true;
}

/**
 * @function clearEvents
 * @description clear all events and commands from database
 * @returns bool: true if operation succeeded
**/

bool Scheduler::clearEvents() {

  //Open database
  std::string errorString;
  //Prepare DELETE query of scheduled_events
  std::string query = "DELETE FROM scheduled_events";
  if (!database::exec(query, errorString)) {
    //Database query failed
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  //DELETE from events commands
  query = "DELETE FROM events_commands";
  if (!database::exec(query, errorString)) {
    //Database query failed
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }

  //Delete from sequences
  query = "DELETE FROM sqlite_sequence WHERE name = \"scheduled_events\"";
  if (!database::exec(query, errorString)) {
    //Database query failed
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }
  query = "DELETE FROM sqlite_sequence WHERE name = \"events_commands\"";
  if (!database::exec(query, errorString)) {
    //Database query failed
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }
  //Delete objects
  for (auto& event : events) {
    delete event;
  }

  for (auto& event : scheduledEvents) {
    delete event;
  }

  return true;
}

/**
 * @function dumpScheduling
 * @description dump scheduling previously loaded from database
 * @param std::string filename optional filename where to write dump; stdout is used otherwise
 * @returns bool: true if successfully dumped 
**/

bool Scheduler::dumpScheduling(const std::string& filename /* = "" */) {

  bool toStdout = filename.length() == 0 ? true : false;
  std::ofstream dumpStream;
  if (!toStdout) {
    //Truncate file
    dumpStream.open(filename, std::ofstream::out | std::ofstream::trunc);
    if (dumpStream.is_open()) {
      dumpStream.close();
    } else {
      logger::log(COMPONENT, LOG_ERROR, "Could not open dump file");
      return false;
    }
  }

  for (auto& event : events) {
    //Prepare dump line
    std::stringstream lineStream;
    lineStream << event->getOid() << ";";
    lineStream << event->getModeName() << ";";
    std::vector<std::string> cmdList = event->getCommandList();
    for (size_t commandIndex = 0; commandIndex < cmdList.size();) {
      std::string cmd = cmdList.at(commandIndex);
      lineStream << cmd;
      //If it is not last command add comma
      if (++commandIndex != cmdList.size()) {
        lineStream << ",";
      }
    }
    lineStream << std::endl;
    std::string line = lineStream.str();
    if (toStdout) {
      std::cout << line;
    } else {
      //Append line to file
      dumpStream.open(filename, std::ofstream::out | std::ofstream::app);
      if (!dumpStream.is_open()) {
        logger::log(COMPONENT, LOG_ERROR, "Could not open dump file");
        return false;
      }
      dumpStream << line;
      dumpStream.close();
    }
  }

  for (auto& event : scheduledEvents) {
    //Prepare dump line
    std::stringstream lineStream;
    lineStream << event->getOid() << ";";
    lineStream << event->getModeName() << ";";
    std::vector<std::string> cmdList = event->getCommandList();
    for (size_t commandIndex = 0; commandIndex < cmdList.size();) {
      std::string cmd = cmdList.at(commandIndex);
      lineStream << cmd;
      //If it is not last command add comma
      if (++commandIndex != cmdList.size()) {
        lineStream << ",";
      }
    }
    lineStream << ";" << event->getTimeout();
    lineStream << std::endl;
    std::string line = lineStream.str();
    if (toStdout) {
      std::cout << line;
    } else {
      //Append line to file
      dumpStream.open(filename, std::ofstream::out | std::ofstream::app);
      if (!dumpStream.is_open()) {
        logger::log(COMPONENT, LOG_ERROR, "Could not open dump file");
        return false;
      }
      dumpStream << line;
      dumpStream.close();
    }
  }

  return true;
}

/**
 * @function runScheduler
 * @description 'run' method executed by scheduler thread
 * @returns int 0 when terminates
**/

int Scheduler::runScheduler() {

  //Do not execute thread if no scheduled events is set
  if (scheduledEvents.size() == 0) {
    return 1;
  }

  time_t elapsedTime = 0; //Stores elapsed seconds since thread started
  //Sort events by their 'absolute' timeout
  std::sort(scheduledEvents.begin(), scheduledEvents.end(), sortByTimeout);
  time_t nextEventTime = scheduledEvents.at(0)->getTimeout();
  //Stop called is set at scheduler destructor
  while (!stopCalled) {
    //If elapsed time % nextEventTime == 0, exec command's id
    if (elapsedTime != 0 && elapsedTime % nextEventTime == 0) {
      //Execute commands for all events which satisfy previous condition
      //That because there could be more than one event with same relative timeout
      for (auto& event : scheduledEvents) {
        //Check condition
        if (elapsedTime % event->getTimeout() == 0) {
          time_t tNow = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
          //If true execute event's commands
          logger::log(COMPONENT, LOG_INFO, "Executing scheduling events for OID " + event->getOid());
          event->executeCommands();
          //Get elapsed time between before and after execution
          time_t evElapsedTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() - tNow;
          //Add to elapsed time event execution elapsed time
          elapsedTime += evElapsedTime;
        }
        //Then calculate event relative timeout
        event->calcRelativeTimeout(elapsedTime);
      }
      std::sort(scheduledEvents.begin(), scheduledEvents.end(), sortByRelativeTimeout);
      //Reset nextEventTime
      nextEventTime = scheduledEvents.at(0)->getTimeout();
      logger::log(COMPONENT, LOG_INFO, "Next scheduled event in " + std::to_string(nextEventTime) + " seconds");
    }
    //Sleep for one second
    std::this_thread::sleep_for(std::chrono::seconds(1));
    elapsedTime++;
  }

  return 0;
}

/**
 * @function addEvent
 * @description add event to database (and to event vector)
 * @param std::string oid
 * @param EventMode mode
 * @param std::vector<std::string> command list
 * @param int optional timeout (for scheduled event)
 * @returns bool: true if add successfully
**/

bool Scheduler::addEvent(const std::string& oid, EventMode mode, const std::vector<std::string>& commandList, int timeout /*= 0*/) {

  //Try to open database
  std::string errorString;
  //Check if event doesn't already exist
  std::stringstream queryStr;
  Event* dummyEv = new Event(oid, mode, commandList);
  queryStr << "SELECT event_id FROM scheduled_events WHERE oid = \"" << oid << "\" AND mode = \"" << dummyEv->getModeName() << "\";";
  delete dummyEv;
  std::vector<std::vector<std::string>> result;
  if (!database::select(&result, queryStr.str(), errorString)) {
    logger::log(COMPONENT, LOG_ERROR, errorString);
    return false;
  }
  //Get eventId if result.size > 0
  if (result.size() > 0) {
    //@! Event already exists
    int eventId = std::stoi(result.at(0).at(0));
    queryStr.str(std::string());
    queryStr << "SELECT execution_order FROM events_commands WHERE event_id = " << eventId << " ORDER BY execution_order DESC LIMIT 1;";
    result.clear();
    if (!database::select(&result, queryStr.str(), errorString)) {
      logger::log(COMPONENT, LOG_ERROR, errorString);
    }
    //Get execution order
    int executionOrder = 0;
    if (result.size() > 0) {
      executionOrder = std::stoi(result.at(0).at(0));
    }
    //Let's find the execution order
    for (auto& cmd : commandList) {
      std::stringstream cmdQueryStr;
      cmdQueryStr << "INSERT INTO events_commands(command, execution_order, event_id) VALUES(\"";
      cmdQueryStr << cmd << "\", ";
      cmdQueryStr << ++executionOrder << ", ";
      cmdQueryStr << eventId << ");";
      std::string query = cmdQueryStr.str();
      if (!database::exec(query, errorString)) {
        //Database query failed
        logger::log(COMPONENT, LOG_ERROR, errorString);
        return false;
      }
    }
  } else {
    //@!New event

    //Event id is the ID the event will take into the database
    //We know it because it's an auto incremented key
    //It's the total amount of events + 1
    int nextEventId = scheduledEvents.size() + events.size() + 1;

    //Instance and create new event
    if (mode == EventMode::AUTO) {
      ScheduledEvent* newEv = new ScheduledEvent(oid, mode, commandList, timeout);
      std::stringstream evQueryStr;
      evQueryStr << "INSERT INTO scheduled_events(mode, timeout, oid) VALUES (\"";
      evQueryStr << newEv->getModeName() << "\",";
      evQueryStr << newEv->getTimeout() << ",\"";
      evQueryStr << newEv->getOid() << "\");";
      std::string query = evQueryStr.str();
      if (!database::exec(query, errorString)) {
        //Database query failed
        logger::log(COMPONENT, LOG_ERROR, errorString);
        delete newEv;
        return false;
      }
      //Add event to scheduled events vector
      scheduledEvents.push_back(newEv);
    } else {
      Event* newEv = new Event(oid, mode, commandList);
      std::stringstream evQueryStr;
      evQueryStr << "INSERT INTO scheduled_events(mode, oid) VALUES (\"";
      evQueryStr << newEv->getModeName() << "\",\"";
      evQueryStr << newEv->getOid() << "\");";
      std::string query = evQueryStr.str();
      if (!database::exec(query, errorString)) {
        //Database query failed
        logger::log(COMPONENT, LOG_ERROR, errorString);
        delete newEv;
        return false;
      }
      //Add event to scheduled vector
      events.push_back(newEv);
    }
    //Add commands to database
    int executionOrder = 0;
    for (auto& cmd : commandList) {
      std::stringstream cmdQueryStr;
      cmdQueryStr << "INSERT INTO events_commands(command, execution_order, event_id) VALUES(\"";
      cmdQueryStr << cmd << "\", ";
      cmdQueryStr << ++executionOrder << ", ";
      cmdQueryStr << nextEventId << ");";
      std::string query = cmdQueryStr.str();
      if (!database::exec(query, errorString)) {
        //Database query failed
        logger::log(COMPONENT, LOG_ERROR, errorString);
        return false;
      }
    }
  }

  return true;
}
