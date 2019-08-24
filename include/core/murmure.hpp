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

#ifndef MURMURE_HPP
#define MURMURE_HPP


#define MURMURE_VERSION "1.1.0"

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

#include <core/mibtable.hpp>
#include <utils/getopts.hpp>
#include <utils/logger.hpp>
#include <utils/databasefacade.hpp>

#include <mibparser/mibparser.hpp>
#include <mibscheduler/scheduler.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

//Logger
#ifndef LOGFILE
#define DEFAULT_MURMURE_LOGFILE "/var/log/murmure.log"
#else
#define DEFAULT_MURMURE_LOGFILE QUOTE(LOGFILE)
#endif

#ifndef LOGLEVEL
#define DEFAULT_MURMURE_LOGLEVEL LOG_INFO
#else
#define DEFAULT_MURMURE_LOGLEVEL LOGLEVEL
#endif

//SQL file for db build
#ifndef SQLFILE
#define DATABASE_SQLFILE "/usr/local/share/SQL/mibtable.sql"
#else
#define DATABASE_SQLFILE QUOTE(SQLFILE)
#endif

#ifndef DBPATH
#define DEFAULT_DATABASEPATH "/usr/local/share/mib.db"
#else
#define DEFAULT_DATABASEPATH QUOTE(DBPATH)
#endif

#define COMPONENT "Core"



#endif
