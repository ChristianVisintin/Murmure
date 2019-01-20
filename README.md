# Murmure

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![HitCount](http://hits.dwyl.io/ChristianVisintin/Murmure.svg)](http://hits.dwyl.io/ChristianVisintin/Murmure) [![Stars](https://img.shields.io/github/stars/ChristianVisintin/Murmure.svg)](https://github.com/ChristianVisintin/Murmure) [![Issues](https://img.shields.io/github/issues/ChristianVisintin/Murmure.svg)](https://github.com/ChristianVisintin/Murmure) [![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/ChristianVisintin/Murmure/issues) [![MemoryLeaks](https://img.shields.io/badge/Memory%20Leaks-None-brightgreen.svg)](https://img.shields.io/badge/Memory%20Leaks-None-brightgreen.svg)

**Net-SNMP** MIB Versatile Extender  

Developed by *Christian Visintin*  

Version 1.0.0

---

## What is Murmure

Murmure is a Net-SNMP MIB Versatile *(and automatic)* Extender.
As we know, it is possible to extend Net-SNMP to handle extra MIBs, but there isn't any
application that does this for you automatically out there.
With Murmure it is possible to automatically extend Net-SNMP without writing a single line of code.  

The only thing you need is a MIB file and Murmure will do the job for you!
Murmure also lets you to schedule events to execute when a SET or a GET is issued on a certain OID and also to schedule temporized events for any OID you want, you just need to configure it with a configuration file.

---

## How does it work

Murmure parses a MIB file and then builds a database with all the OIDs read. Then it can be executed as a **daemon** for *pass_persist* configuration or as oneshot executable for *pass* configuration. When it receives a request on a certain OID it will read the associated data from the database and will return or update values as requested.
It is possible to configure scheduled events on any OID, these events will be triggered based on their type, which can be 'GET/SET/AUTO/INIT', but we'll see this in event section.
Murmure can handle every type allowed in SNMP, but if it is not a primitive (we'll see later what a primitive is) a module to handle it is required.

---

## Build

Build is accomplished using GNU autotools.

### Requirements

* C++11 compiler
* GNU Autotools
* Python3
* libsqlite3
* sqlite3
* libpthread

#### Optional Requirements

* inquirer - to facilitate module selection ```pip install inquirer```

### Build Procedure

```sh
set -e
python3 build/moduleSelector.py
./autogen.sh
./configure
make
make install
```

### Configure Options

* DBPATH: Murmure database path
* LOGFILE: Murmure logfile
* LOGLEVEL: Log level used for logging
  * 1: FATAL
  * 2: ERROR
  * 3: WARN
  * 4: INFO
  * 5: DEBUG
* SQLFILE: Murmure SQL file

---

## Command Line Options

* ```-D``` Start Murmure as daemon (use for NET-SNMP pass_persist)
* ```-g <oid>``` issue 'get' on specified OID
* ```-s <oid> <type> <value>``` issue 'set' on specified OID with new value
* ```-n <oid>``` issue 'getnext' on specified OID
* ```-M <rootOID> <mibfile>``` parse specified MIB file; MIB root OID must be specified
* ```-S [schedule file]``` schedule Murmure for this MIB; if file is not passed command line will be used for scheduling
* ```--dump-scheduling [outfile]``` Dump scheduling to a file if passed; if not is dumped to stdout
* ```--reset``` Reset entire MIB and schedule tables
* ```-C <oid> <value>``` Change manually the value associated to OID

---

## Configuration

Once Murmure has been built, in order to configure Murmure properly these steps have to be followed:

### MIB Parsing

The first thing to do is parse a MIB file, which creates the mib database.
To do it is enough to issue this command:

```sh
murmure -M <rootOid> <mibfile>
```

Remember that all commands must be executed with root permission  
If everything worked fine this message should be displayed "MIB parsed successfully"  

If not check if these requirements are satisfied:

* Database file exists and can be edited by your user
* MIB file is in UNIX text format (use dos2unix to convert from DOS text format)
* All SYNTAX in MIB files are primitives or are supported by Murmure installed modules (check Data Types chapter)

**All values parsed will be set with a default value which is "0".** you can then create your scripts which manually changes the values using **murmure --change command line option.**

### Scheduling

Once a MIB file has been parsed, it is possible to schedule events.
Events are shell commands/programs which are executed in a specific order and are associated to a specific OID and to a certain trigger.  

Triggers are a circumstances which activates the event, they are:

* GET
* SET
* AUTO
* INIT

#### GET Events

GET events are executed when a GET request is issued on the OID associated to the event.
Get events are executed before the object's value is read from the database.

#### SET Events

SET events are executed when a SET request is issued on the OID associated to the event.
If **$SNMP_VALUE** is present in the event's command it will be replaced by the value set to the Object.
The set events are executed after the new value has been read to the database.

#### INIT Events

INIT events are executed at the start of Murmure when in Daemon mode and are executed only once.

#### AUTO Events

AUTO events are a special type of event, called **scheduled event**. Scheduled events have, in addition, a timeout associated and are executed when their timeout expires. When the timeout expires the timer is obviously reset.

### Net-SNMP Configuration

#### Daemon mode

```txt
...
pass_persist <rootOid> /path/to/murmure/murmure -D
...
```

#### Oneshot mode

```txt
...
pass <rootOid> /path/to/murmure/murmure
...
```

---

## Data Types

Every object has its Syntax which specifies its data type, but net-snmp does not support all of them, in specific net-snmp extension can return only a few types of data. These data types in murmure are called "primitives".  
In specific primitive types are:

* COUNTER
* GAUGE
* INTEGER
* IPADDRESS
* OBJECT
* OCTET
* SEQUENCE
* STRING
* TIMETICKS

Primitives are implemented in murmure in murmure/primitives/ and all of them implements the Primitive interface.
If we had only primitives and you tried to parse a MIB which implements other types such as DisplayString or DateAndTime an error would raise, saying that Murmure could not resolve that type.  
How can other types (such as DisplayString or DateAndTime) be implemented?  
Simply using Modules.

### Modules

Modules are the functionality which allows you to use non-primitive types with net-snmp. A module is just a data type which uses and extends a Primitive to create an interface between net-snmp and the Primitive class.
An example to explain modules is DisplayString <https://www.ietf.org/rfc/rfc1213.txt>.
If Murmure hadn't modules, when a MIB which uses DisplayString gets parsed, it would return an error saying DisplayString is not a known data type. However, using modules this can be achieved easily.
DisplayString, for example, can be just imagined as a STRING Primitive, so it will instance a String primitive in it setting and getting the value communicating with the same primitive.
Another case is DateAndTime, which is a special Octet primitive, which must have a particular syntax. So before setting the value will check that its length is 8 or 11 bytes etc.

Since SNMP has many data types, during the build process it is possible to specify which modules you want into your Murmure build. If you want to contribute adding new Modules, check the contributions guide.

#### Supported Modules

These are the modules currently supported by Murmure

* BITS
* Counter32
* Counter64
* DateAndTime
* DisplayString
* Gauge32
* Integer32
* Opaque
* RowStatus
* Unsigned32

---

## Known Issues

* MIB parsing fails but everything seems right: probably it's because your MIB is in dos format, please try to issue ```dos2unix``` command on your MIB file

---

## Contributions [![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/ChristianVisintin/Murmure/issues)

Contributions are welcomed, please follow [Murmure's contributions guide](https://github.com/ChristianVisintin/Murmure/blob/master/CONTRIBUTING.md)

---

## Branches

* **Master**: always stable
* **Dev**: master under development
* others: independent features under development

## Changelog

### Murmure 1.0.0 (20/01/2019)

* Added modules support
* Added the following Modules:
  * BITS
  * Counter32
  * Counter64
  * DateAndTime
  * DisplayString
  * Gauge32
  * Integer32
  * Opaque
  * RowStatus
  * Unsigned32
* Added destructor for primitive interface
* Added autotools build
* Fixed Scheduling issue

### Murmure 1.0.0-rc (12/01/2019)

* First version

---

## License

Murmure is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. Murmure is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with Murmure.  
If not, see <http://www.gnu.org/licenses/>.
