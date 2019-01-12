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

#define MURMURE_VERSION "1.0-rc"

#define USAGE \
  "\
Usage:\n\
\t-D\t\t\t\t\tStart Murmure as daemon (use for NET-SNMP pass_persist)\n\
\t-g <OID>\t\t\t\tissue 'get' on specified OID\n\
\t-n <OID>\t\t\t\tissue 'get next' on specified OID\n\
\t-s <OID> <type> <value>\t\t\tissue 'set' on specified OID\n\
\t-M --parse-mib <rootOID> <MIBfile>\tParse and configure Murmure for selected MIB\n\
\t-S --schedule [schedule file]\t\tConfigure Murmure schedulation.\n\
\t--dump-scheduling [outfile]\t\tDump scheduling.\n\
\t--reset\t\t\t\t\tReset entire mib and event tables\n\
\t-C --change <OID> <value>\t\tSet value for OID manually to value\n\
\t-h --help\t\t\t\tShow this page\n\
"

//TODO: command to change values manually
//TODO: implement set export value

#include <algorithm>
#include <iostream>
#include <sstream>

#include <murmure/mibtable.hpp>
#include <utils/getopts.hpp>
#include <utils/logger.hpp>

#include <mibparser/mibparser.hpp>
#include <mibscheduler/scheduler.hpp>

//Logger
#ifndef MURMURE_LOGFILE
#define MURMURE_LOGFILE "/var/log/murmure.log"
#endif

#ifndef LOGLEVEL
#define LOGLEVEL LOG_INFO
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
  Oid* reqOid = mibtab->getOidByOid(requestedOid);
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
  std::string nextOid = requestedOid;
  bool oidFound = false;
  do {
    nextOid = mibtab->getNextOid(nextOid);
    if (nextOid == "") {
      //Output no-such-name
      std::cout << "no-such-name" << std::endl;
      break;
    }
    Oid* assocOid = mibtab->getOidByOid(nextOid);
    //Get on next oid
    if (assocOid == nullptr) {
      std::stringstream ss;
      ss << "OID " << requestedOid << " does not exist";
      logger::log(COMPONENT, LOG_WARN, ss.str());
      //Output no-such-name
      std::cout << "no-such-name" << std::endl;
      return;
    }

    //Check access mode
    if (assocOid->getAccessMode() == AccessMode::NOT_ACCESSIBLE) {
      oidFound = false;
      continue;
    }

    oidFound = true;
    //Exec GET commands
    mibScheduler->fetchAndExec(requestedOid, EventMode::GET);

    //Else output OID, type, value
    std::cout << assocOid->getOid() << std::endl;
    std::cout << assocOid->getType() << std::endl;
    std::cout << assocOid->getPrintableValue() << std::endl;
  } while (!oidFound);
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
  Oid* reqOid = mibtab->getOidByOid(requestedOid);
  if (reqOid == nullptr) {
    //Get parent Oid
    std::string parentOidStr;
    size_t lastDotPos = requestedOid.find_last_of(".");
    if (lastDotPos != std::string::npos) {
      parentOidStr = requestedOid.substr(0, lastDotPos);
    } else {
      //@! Is not a valid OID
      std::stringstream ss;
      ss << "OID " << requestedOid << " does not exist";
      logger::log(COMPONENT, LOG_WARN, ss.str());
      //Output no-such-name
      std::cout << "no-such-name" << std::endl;
      return;
    }
    //Get grandParent OID
    Oid* grandParentOid = mibtab->getOidByOid(mibtab->getPreviousOid(parentOidStr));
    if (grandParentOid != nullptr) {
      //Check if it is table
      if (grandParentOid->getPrimitiveType() == PRIMITIVE_SEQUENCE) {
        //@! It is table, check if READ-CREATE
        //Get parentOid object
        Oid* parentOid = mibtab->getOidByOid(parentOidStr);
        //Check access mode
        if (parentOid->getAccessMode() != AccessMode::READCREATE && parentOid->getAccessMode() != AccessMode::READWRITE) {
          std::stringstream ss;
          ss << "OID " << parentOid << " is not at least READCREATE";
          logger::log(COMPONENT, LOG_WARN, ss.str());
          //Output read-only
          std::cout << "read-only" << std::endl;
          return;
        }
        //Access mode is OK
        Oid* childOid = new Oid(requestedOid, parentOid->getType(), value, 3, parentOid->getName());
        //Add new OID to mibtable
        if (mibtab->addOid(childOid)) {
          //@! Table element added Successfully
          //if added successfully output OID, type, value
          std::cout << childOid->getOid() << std::endl;
          std::cout << childOid->getType() << std::endl;
          std::cout << childOid->getPrintableValue() << std::endl;
          //Exec SET commands for parent OID
          mibScheduler->fetchAndExec(parentOidStr, EventMode::SET);
          return;
        } else {
          //Commit failed
          std::stringstream ss;
          ss << "Unable to set value for OID " << requestedOid;
          logger::log(COMPONENT, LOG_ERROR, ss.str());
          //Output read-only
          std::cout << "commit-failed" << std::endl;
          return;
        }
      } else {
        //@! OID exists but it is not table
        std::stringstream ss;
        ss << "OID " << requestedOid << " does not exist";
        logger::log(COMPONENT, LOG_WARN, ss.str());
        //Output no-such-name
        std::cout << "no-such-name" << std::endl;
        return;
      }
    } else {
      //@! Grandparent OID does not exist
      std::stringstream ss;
      ss << "OID " << requestedOid << " does not exist";
      logger::log(COMPONENT, LOG_WARN, ss.str());
      //Output no-such-name
      std::cout << "no-such-name" << std::endl;
      return;
    }
  }

  //@! Regular flow; no table; OID exists
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
  std::string expectedType = reqOid->getPrimitiveType();
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

  //Exec SET commands
  mibScheduler->fetchAndExec(requestedOid, EventMode::SET);

  //Else output OID, type, value
  std::cout << reqOid->getOid() << std::endl;
  std::cout << reqOid->getType() << std::endl;
  std::cout << reqOid->getPrintableValue() << std::endl;
  return;
}

int main(int argc, char* argv[]) {

  //Exitcode declaration
  int exitcode = 0;

  //Initialize logger variables
  logger::logfile = "";
  logger::logLevel = LOGLEVEL;
  logger::toStdout = true;

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
  std::string optError;
  if (!getOpts(&cmdLineOpts, argc, argv, &optError)) {
    std::cout << optError << std::endl;
    std::cout << "Murmure " << MURMURE_VERSION << " - Developed by Christian Visintin" << std::endl;
    std::cout << "<https://github.com/ChristianVisintin/Murmure> (C) 2018-2019" << std::endl;
    std::cout << USAGE << std::endl;
    return 255;
  }

  //Options are valid

  //Init logger
  logger::logfile = std::string(MURMURE_LOGFILE);
  logger::toStdout = true;

  if (cmdLineOpts.command == Command::HELP) {
    std::cout << "Murmure " << MURMURE_VERSION << " - Developed by Christian Visintin" << std::endl;
    std::cout << "<https://github.com/ChristianVisintin/Murmure> (C) 2018-2019" << std::endl;
    std::cout << USAGE << std::endl;
  } else if (cmdLineOpts.command == Command::DAEMON) {
    //Daemon Mode
    //Set silent mode
    logger::toStdout = false;
    //Instance new mibtable
    Mibtable* mibtab = new Mibtable();
    //Load mibtable
    if (!mibtab->loadMibTable()) {
      logger::log(COMPONENT, LOG_FATAL, "MIB table loading failed; execution aborted");
      delete mibtab;
      return 1;
    }
    logger::log(COMPONENT, LOG_INFO, "MIB table loaded successfully");
    //Instance scheduler
    Scheduler* mibScheduler = new Scheduler(mibtab);
    //Load scheduler events
    if (!mibScheduler->loadEvents()) {
      logger::log(COMPONENT, LOG_FATAL, "Could not load scheduler events; execution aborted");
      delete mibtab;
      delete mibScheduler;
      return 2;
    }
    logger::log(COMPONENT, LOG_INFO, "Scheduler loaded successfully");
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
        std::transform(datatype.begin(), datatype.end(), datatype.begin(), ::toupper);
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
    //Set silent mode
    logger::toStdout = false;
    //Instance new mibtable
    Mibtable* mibtab = new Mibtable();
    //Load mibtable
    if (!mibtab->loadMibTable()) {
      logger::log(COMPONENT, LOG_FATAL, "MIB table loading failed; execution aborted");
      delete mibtab;
      return 1;
    }
    logger::log(COMPONENT, LOG_INFO, "MIB table loaded successfully");
    //Instance scheduler
    Scheduler* mibScheduler = new Scheduler(mibtab);
    //Load scheduler events
    if (!mibScheduler->loadEvents()) {
      logger::log(COMPONENT, LOG_FATAL, "Could not load scheduler events; execution aborted");
      delete mibtab;
      delete mibScheduler;
      return 2;
    }
    logger::log(COMPONENT, LOG_INFO, "Scheduler loaded successfully");
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
    //Set silent mode
    logger::toStdout = false;
    //Instance new mibtable
    Mibtable* mibtab = new Mibtable();
    //Load mibtable
    if (!mibtab->loadMibTable()) {
      logger::log(COMPONENT, LOG_FATAL, "MIB table loading failed; execution aborted");
      delete mibtab;
      return 1;
    }
    logger::log(COMPONENT, LOG_INFO, "MIB table loaded successfully");
    //Instance scheduler
    Scheduler* mibScheduler = new Scheduler(mibtab);
    //Load scheduler events
    if (!mibScheduler->loadEvents()) {
      logger::log(COMPONENT, LOG_FATAL, "Could not load scheduler events; execution aborted");
      delete mibtab;
      delete mibScheduler;
      return 2;
    }
    logger::log(COMPONENT, LOG_INFO, "Scheduler loaded successfully");
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
    //Set silent mode
    logger::toStdout = false;
    //Instance new mibtable
    Mibtable* mibtab = new Mibtable();
    //Load mibtable
    if (!mibtab->loadMibTable()) {
      logger::log(COMPONENT, LOG_FATAL, "MIB table loading failed; execution aborted");
      delete mibtab;
      return 1;
    }
    logger::log(COMPONENT, LOG_INFO, "MIB table loaded successfully");
    //Instance scheduler
    Scheduler* mibScheduler = new Scheduler(mibtab);
    //Load scheduler events
    if (!mibScheduler->loadEvents()) {
      logger::log(COMPONENT, LOG_FATAL, "Could not load scheduler events; execution aborted");
      delete mibtab;
      delete mibScheduler;
      return 2;
    }
    logger::log(COMPONENT, LOG_INFO, "Scheduler loaded successfully");
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
    std::transform(datatype.begin(), datatype.end(), datatype.begin(), ::toupper);
    std::stringstream setStream;
    setStream << "Received SET for OID " << requestedOid << "; Type: " << datatype << "; Value: " << value;
    snmp_set(mibtab, mibScheduler, requestedOid, datatype, value);
    delete mibtab;       //Free mibtab
    delete mibScheduler; //Free scheduler
  } else if (cmdLineOpts.command == Command::PARSE_MIB) {
    std::string rootOid = cmdLineOpts.args.at(0);
    std::string mibFile = cmdLineOpts.args.at(1);
    Mibparser* mibParser = new Mibparser();
    if (mibParser->parseMibFile(rootOid, mibFile)) {
      logger::log(COMPONENT, LOG_INFO, "MIB parsed successfully");
    } else {
      logger::log(COMPONENT, LOG_FATAL, "MIB parsing failed");
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

        std::cin.ignore();
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
        std::string errorString;
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
    std::string dumpFile = "";
    //Check if dump file has been provided
    if (cmdLineOpts.args.size() > 0) {
      //Parse file
      dumpFile = cmdLineOpts.args.at(0);
    }
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
    //Load scheduler events
    if (!mibScheduler->loadEvents()) {
      logger::log(COMPONENT, LOG_FATAL, "Could not load scheduler events; execution aborted");
      delete mibtab;
      delete mibScheduler;
      return 2;
    }

    //Dump scheduling
    if (!mibScheduler->dumpScheduling(dumpFile)) {
      logger::log(COMPONENT, LOG_FATAL, "Scheduling dump failed");
      exitcode = 1;
    }

    exitcode = 0;
    delete mibtab;
    delete mibScheduler;

  } else if (cmdLineOpts.command == Command::RESET) {
    //Instance new scheduler, no mib table needed
    Scheduler* mibScheduler = new Scheduler();
    //Instance new mibtable
    Mibtable* mibtab = new Mibtable();
    //Load mibtable
    if (!mibtab->loadMibTable()) {
      logger::log(COMPONENT, LOG_FATAL, "MIB table loading failed; execution aborted");
      delete mibtab;
      return 1;
    }
    //Clear all events
    if (!mibScheduler->clearEvents()) {
      logger::log(COMPONENT, LOG_FATAL, "Scheduling reset failed");
      return 1;
    }
    //Clear mibtable
    if (!mibtab->clearMibtable()) {
      logger::log(COMPONENT, LOG_FATAL, "Mibtable reset failed");
      return 1;
    }
    delete mibScheduler;
    delete mibtab;
    exitcode = 0;

  } else {
    logger::log(COMPONENT, LOG_ERROR, "Unknown command");
    exitcode = 255;
  }

  //std exit
  return exitcode;
}
