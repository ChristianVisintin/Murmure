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

#define MURMURE_VERSION "0.1"

#define USAGE \
  "\
Usage:\n\
\t-D\t\tStart Murmure as daemon (use for NET-SNMP pass_persist)\n\
\t-g <OID>\t\tissue 'get' on specified OID\n\
\t-n <OID>\t\tissue 'get next' on specified OID\n\
\t-s <OID> <type> <value>\t\tissue 'set' on specified OID\n\
\t-M --parse-mib <MIBfile>\t\tParse and configure Murmure for selected MIB\n\
\t-S --schedule [schedule file]\t\tConfigure Murmure schedulation. If schedule file is specified, configuration will be taken from it, otherwise will be configured using command line\n\
\t--dump-scheduling [outfile]\t\tDump scheduling; if file is specified will be dumped to the specified file, otherwise will be dumped on stdout\n\
\t--reset\t\tReset entire mib and event tables\n\
\t-h --help\t\tShow this page\n\
"

#include <algorithm>
#include <iostream>
#include <sstream>

#include <utils/getopts.hpp>
#include <utils/logger.hpp>
#include <murmure/mibtable.hpp>

#include <mibparser/mibparser.hpp>
#include <mibscheduler/scheduler.hpp>

//Logger
#ifndef MURMURE_LOGFILE
#define MURMURE_LOGFILE "/var/log/murmure.log"
#endif

#define COMPONENT "Main"

using namespace murmure;

/**
 * @function snmp_get
 * @description Issue GET request and print output
 * @param Mibtable*: pointer to MIB-table
 * @param Scheduler*: pointer to scheduler
 * @param std::string: requested OID to get
**/

inline void snmp_get(Mibtable* mibtab, Scheduler* mibScheduler, std::string requestedOid) {

  //Try to get OID from mibtable
  Oid* reqOid = mibtab->getOid(requestedOid);
  if (reqOid == nullptr) {
    std::stringstream ss;
    ss << "OID " << requestedOid << " does not exist";
    logger::log(COMPONENT, LOG_WARN, ss.str());
    //Output no-such-name
    std::cout << "no-such-name" << std::endl;
    return;
  }

  //Check access mode
  if (reqOid->getAccessMode() == AccessMode::NOT_ACCESSIBLE) {
    std::stringstream ss;
    ss << "OID " << requestedOid << " is NOT-ACCESSIBLE";
    logger::log(COMPONENT, LOG_WARN, ss.str());
    //Output no-access
    std::cout << "no-access" << std::endl;
    return;
  }

  //Exec GET commands
  mibScheduler->fetchAndExec(requestedOid, EventMode::GET);

  //Else output OID, type, value
  std::cout << reqOid->getOid() << std::endl;
  std::cout << reqOid->getType() << std::endl;
  std::cout << reqOid->getPrintableValue() << std::endl;

  return;
}

/**
 * @function snmp_getnext
 * @description Issue GETNEXT request and print output
 * @param Mibtable*: pointer to MIB-table
 * @param Scheduler*: pointer to scheduler
 * @param std::string: requested OID to get
**/

inline void snmp_getnext(Mibtable* mibtab, Scheduler* mibScheduler, std::string requestedOid) {

  //Get nextOid
  std::string nextOid = mibtab->getNextOid(requestedOid);
  if (nextOid == "") {
    //Output no-such-name
    std::cout << "no-such-name" << std::endl;
    return;
  }
  //Get on next oid
  snmp_get(mibtab, mibScheduler, nextOid);
  return;
}

/**
 * @function snmp_set
 * @description Issue SET request and print output
 * @param Mibtable*: pointer to MIB-table
 * @param Scheduler*: pointer to scheduler
 * @param std::string: requested OID to get
 * @param std::string: string data type
 * @param std::string: printable value to set
**/

inline void snmp_set(Mibtable* mibtab, Scheduler* mibScheduler, std::string requestedOid, std::string datatype, std::string value) {

  //Try to get OID from mibtable
  Oid* reqOid = mibtab->getOid(requestedOid);
  if (reqOid == nullptr) {
    //TODO: check if table element OID...
    std::stringstream ss;
    ss << "OID " << requestedOid << " does not exist";
    logger::log(COMPONENT, LOG_WARN, ss.str());
    //Output no-such-name
    std::cout << "no-such-name" << std::endl;
    return;
  }

  //Check access mode
  if (reqOid->getAccessMode() != AccessMode::READWRITE) {
    std::stringstream ss;
    ss << "OID " << requestedOid << " is not READWRITE";
    logger::log(COMPONENT, LOG_WARN, ss.str());
    //Output read-only
    std::cout << "read-only" << std::endl;
    return;
  }

  //Check types
  std::string expectedType = reqOid->getPrimtiveType();
  if (expectedType != datatype) {
    std::stringstream ss;
    ss << "Wrong type for OID " << requestedOid << "; expected " << expectedType << " got " << datatype;
    logger::log(COMPONENT, LOG_WARN, ss.str());
    //Output read-only
    std::cout << "wrong-type" << std::endl;
    return;
  }

  //Try to set value
  if (!reqOid->setValue(value)) {
    std::stringstream ss;
    ss << "Unable to set value for OID " << requestedOid;
    logger::log(COMPONENT, LOG_ERROR, ss.str());
    //Output read-only
    std::cout << "commit-failed" << std::endl;
    return;
  }

  //Else output OID, type, value
  std::cout << reqOid->getOid() << std::endl;
  std::cout << reqOid->getType() << std::endl;
  std::cout << reqOid->getPrintableValue() << std::endl;
  return;
}

int main(int argc, char* argv[]) {

  //Exitcode declaration
  int exitcode = 0;

  //Getopts
  /**
   * Command line options
   * M filename: parse MIB file
   * S schedule: parse scheduling file
   * D daemon: murmure as pass_persist application
   * g get: get option
   * s set: set Command
   * n: get-next: command
   * h: help
   **/

  options cmdLineOpts;
  std::string optError = nullptr;
  if (!getOpts(&cmdLineOpts, argc, argv, &optError)) {
    std::cout << optError << std::endl;
    std::cout << USAGE << std::endl;
    return 255;
  }

  //Options are valid

  //Init logger
  logger::logfile = std::string(MURMURE_LOGFILE);
  logger::stdout = true;

  if (cmdLineOpts.command == Command::HELP) {
    std::cout << "Murmure " << MURMURE_VERSION << " - Developed by Christian Visintin" << std::endl;
    std::cout << "<https://github.com/ChristianVisintin/Murmure> © 2018" << std::endl;
    std::cout << USAGE << std::endl;
  } else if (cmdLineOpts.command == Command::DAEMON) {
    //Daemon Mode
    //Set silent mode
    logger::stdout = false;
    //Instance new mibtable
    Mibtable* mibtab = new Mibtable();
    //Load mibtable
    if (!mibtab->loadMibTable()) {
      logger::log(COMPONENT, LOG_FATAL, "MIB table loading failed; execution aborted");
      delete mibtab;
      return 1;
    }
    //Instance scheduler
    Scheduler* mibScheduler = new Scheduler(mibtab);
    //Load scheduler events
    if (!mibScheduler->loadEvents()) {
      logger::log(COMPONENT, LOG_FATAL, "Could not load scheduler events; execution aborted");
      delete mibtab;
      delete mibScheduler;
      return 2;
    }
    //Start scheduler
    if (!mibScheduler->startScheduler()) {
      logger::log(COMPONENT, LOG_FATAL, "Could not start scheduler; execution aborted");
      delete mibtab;
      delete mibScheduler;
      return 2;
    }
    logger::log(COMPONENT, LOG_INFO, "Murmure daemon started");
    std::string command;
    //Daemon terminates when command == ""
    while (true) {
      //Get line
      std::getline(std::cin, command);

      if (command == "") { //Terminate daemon
        break;
      } else if (command == "PING") {
        //Output PONG (part of the "secret" net-snmp handshaking)
        std::cout << "PONG" << std::endl;
      } else if (command == "get") {
        //Read from stdout requested OID
        std::string requestedOid;
        std::getline(std::cin, requestedOid);
        logger::log(COMPONENT, LOG_INFO, "Received GET for OID " + requestedOid);
        snmp_get(mibtab, mibScheduler, requestedOid);
      } else if (command == "getnext") {
        std::string requestedOid;
        std::getline(std::cin, requestedOid);
        logger::log(COMPONENT, LOG_INFO, "Received GETNEXT for OID " + requestedOid);
        snmp_getnext(mibtab, mibScheduler, requestedOid);
      } else if (command == "set") {
        //Two lines; first is OID, second is: 'datatype' 'value'
        std::string requestedOid;
        std::getline(std::cin, requestedOid);
        //Get params
        std::string setParams;
        std::getline(std::cin, setParams);
        //Split params into two tokens
        size_t sepPos = setParams.find(' ');
        if (sepPos == std::string::npos) {
          logger::log(COMPONENT, LOG_ERROR, "Invalid SET parameters");
          //Wait for next command
          continue;
        }
        std::string datatype = setParams.substr(0, sepPos);
        std::string value = setParams.substr(sepPos + 1);
        //Convert datatype to lower case - just to be sure
        std::transform(datatype.begin(), datatype.end(), datatype.begin(), ::tolower);
        std::stringstream setStream;
        setStream << "Received SET for OID " << requestedOid << "; Type: " << datatype << "; Value: " << value;
        logger::log(COMPONENT, LOG_INFO, setStream.str());
        snmp_set(mibtab, mibScheduler, requestedOid, datatype, value);
      }
    }
    delete mibtab;       //Free mibtab
    delete mibScheduler; //Free scheduler
    logger::log(COMPONENT, LOG_INFO, "Murmure daemon terminated");
  } else if (cmdLineOpts.command == Command::GET) {
    //Instance new mibtable
    Mibtable* mibtab = new Mibtable();
    //Load mibtable
    if (!mibtab->loadMibTable()) {
      logger::log(COMPONENT, LOG_FATAL, "MIB table loading failed; execution aborted");
      delete mibtab;
      return 1;
    }
    //Instance scheduler
    Scheduler* mibScheduler = new Scheduler(mibtab);
    //Load scheduler events
    if (!mibScheduler->loadEvents()) {
      logger::log(COMPONENT, LOG_FATAL, "Could not load scheduler events; execution aborted");
      delete mibtab;
      delete mibScheduler;
      return 2;
    }
    //Start scheduler
    if (!mibScheduler->startScheduler()) {
      logger::log(COMPONENT, LOG_FATAL, "Could not start scheduler; execution aborted");
      delete mibtab;
      delete mibScheduler;
      return 2;
    }
    std::string requestedOid = cmdLineOpts.args.at(0);
    logger::log(COMPONENT, LOG_INFO, "Received GET for OID " + requestedOid);
    snmp_get(mibtab, mibScheduler, requestedOid);
    delete mibtab;       //Free mibtab
    delete mibScheduler; //Free scheduler
  } else if (cmdLineOpts.command == Command::GET_NEXT) {
    //Instance new mibtable
    Mibtable* mibtab = new Mibtable();
    //Load mibtable
    if (!mibtab->loadMibTable()) {
      logger::log(COMPONENT, LOG_FATAL, "MIB table loading failed; execution aborted");
      delete mibtab;
      return 1;
    }
    //Instance scheduler
    Scheduler* mibScheduler = new Scheduler(mibtab);
    //Load scheduler events
    if (!mibScheduler->loadEvents()) {
      logger::log(COMPONENT, LOG_FATAL, "Could not load scheduler events; execution aborted");
      delete mibtab;
      delete mibScheduler;
      return 2;
    }
    //Start scheduler
    if (!mibScheduler->startScheduler()) {
      logger::log(COMPONENT, LOG_FATAL, "Could not start scheduler; execution aborted");
      delete mibtab;
      delete mibScheduler;
      return 2;
    }
    std::string requestedOid = cmdLineOpts.args.at(0);
    logger::log(COMPONENT, LOG_INFO, "Received GETNEXT for OID " + requestedOid);
    snmp_getnext(mibtab, mibScheduler, requestedOid);
    delete mibtab;       //Free mibtab
    delete mibScheduler; //Free scheduler
  } else if (cmdLineOpts.command == Command::SET) {
    //Instance new mibtable
    Mibtable* mibtab = new Mibtable();
    //Load mibtable
    if (!mibtab->loadMibTable()) {
      logger::log(COMPONENT, LOG_FATAL, "MIB table loading failed; execution aborted");
      delete mibtab;
      return 1;
    }
    //Instance scheduler
    Scheduler* mibScheduler = new Scheduler(mibtab);
    //Load scheduler events
    if (!mibScheduler->loadEvents()) {
      logger::log(COMPONENT, LOG_FATAL, "Could not load scheduler events; execution aborted");
      delete mibtab;
      delete mibScheduler;
      return 2;
    }
    //Start scheduler
    if (!mibScheduler->startScheduler()) {
      logger::log(COMPONENT, LOG_INFO, "Could not start scheduler; execution aborted");
      delete mibtab;
      delete mibScheduler;
      return 2;
    }
    std::string requestedOid = cmdLineOpts.args.at(0);
    std::string datatype = cmdLineOpts.args.at(1);
    std::string value = cmdLineOpts.args.at(2);
    std::transform(datatype.begin(), datatype.end(), datatype.begin(), ::tolower);
    std::stringstream setStream;
    setStream << "Received SET for OID " << requestedOid << "; Type: " << datatype << "; Value: " << value;
    snmp_set(mibtab, mibScheduler, requestedOid, datatype, value);
    delete mibtab;       //Free mibtab
    delete mibScheduler; //Free scheduler
  } else if (cmdLineOpts.command == Command::PARSE_MIB) {
    std::string mibFile = cmdLineOpts.args.at(0);
    Mibparser* mibParser = new Mibparser();
    if (mibParser->parseMibFile(mibFile)) {
      logger::log(COMPONENT, LOG_INFO, "MIB parsed successfully");
    } else {
      logger::log(COMPONENT, LOG_ERROR, "MIB parsing failed");
      exitcode = 1;
    }
    delete mibParser;
  } else if (cmdLineOpts.command == Command::SCHEDULE) {
    //Instance mibtable
    Mibtable* mibtab = new Mibtable();
    //Load mibtable
    if (!mibtab->loadMibTable()) {
      logger::log(COMPONENT, LOG_FATAL, "MIB table loading failed; execution aborted");
      delete mibtab;
      return 1;
    }
    //Instance new scheduler
    Scheduler* mibScheduler = new Scheduler(mibtab);
    //Check if a file has to be parsed or if user wants to insert scheduling manually
    if (cmdLineOpts.args.size() == 1) {
      //Parse file
      std::string schedulingFile = cmdLineOpts.args.at(0);
      std::string errorString;
      if (mibScheduler->parseScheduling(schedulingFile, &errorString)) {
        logger::log(COMPONENT, LOG_INFO, "Scheduling file parsed successfully");
      } else {
        logger::log(COMPONENT, LOG_ERROR, "Unable to parse scheduling file: " + errorString);
        exitcode = 1;
      }
    } else {
      //Prompt manually
      std::cout << "Murmure scheduler CLI" << std::endl;
      std::cout << "Type QUIT to exit" << std::endl;
      while (true) {
        //Init
        std::string oid;
        std::string modeStr;
        std::vector<std::string> commandList;
        int timeout = 0;
        EventMode mode;
        //Ask user
        std::cout << "OID: ";
        std::cin >> oid;
        if (oid == "QUIT") {
          std::cout << "Scheduling saved! Bye bye!" << std::endl;
          break;
        }
        std::cout << "Event mode [GET/SET/AUTO/INIT]: ";
        std::cin >> modeStr;
        if (modeStr == "QUIT") {
          std::cout << "Scheduling saved! Bye bye!" << std::endl;
          break;
        } else if (modeStr == "GET") {
          mode = EventMode::GET;
        } else if (modeStr == "SET") {
          mode = EventMode::SET;
        } else if (modeStr == "AUTO") {
          mode = EventMode::AUTO;
          //@! Ask timeout for Scheduled events
          std::cout << "Set timeout for scheduled event: ";
          std::cin >> timeout;
        } else if (modeStr == "INIT") {
          mode = EventMode::INIT;
        } else {
          std::cout << "Invalid Event mode" << std::endl;
          continue;
        }

        std::cout << "Set command list to execute; press ENTER (without typing anything else) to commit scheduling" << std::endl;
        while (1) {
          std::string command;
          std::cout << ">> ";
          std::getline(std::cin, command);
          if (command == "") {
            break;
          }
          //Add command to commandList
          commandList.push_back(command);
        }

        //Commit changes
        std::string errorString = nullptr;
        if (mibScheduler->parseScheduling(oid, mode, commandList, &errorString, timeout)) {
          std::cout << "Scheduling entry saved successfully" << std::endl;
        } else {
          std::cout << "Scheduling entry refused: " << errorString << std::endl;
        }
      }
    }

    delete mibtab;
    delete mibScheduler;
  } else if (cmdLineOpts.command == Command::DUMP_SCHEDULE) {
    //TODO: implement this
  } else if (cmdLineOpts.command == Command::RESET) {
    //TODO: implement this
  } else {
    logger::log(COMPONENT, LOG_ERROR, "Unknown command");
    exitcode = 255;
  }

  //std exit
  return exitcode;
}
