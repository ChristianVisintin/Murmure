LIBS = 
INCLUDE = ../include/
AM_CXXFLAGS = -Wall -std=c++11 -I ${INCLUDE}
AM_LDFLAGS = -lsqlite3 -lpthread

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = murmure
murmure_SOURCES = murmure.cpp mibparser/mibparser.cpp mibscheduler/event.cpp mibscheduler/scheduledevent.cpp mibscheduler/scheduler.cpp core/primitives/counter.cpp core/primitives/gauge.cpp core/primitives/integer.cpp core/primitives/ipaddress.cpp core/primitives/objectid.cpp core/primitives/octet.cpp core/primitives/sequence.cpp core/primitives/string.cpp core/primitives/timeticks.cpp core/mibtable.cpp core/modulefacade.cpp core/oid.cpp utils/databasefacade.cpp utils/getopts.cpp utils/logger.cpp utils/strutils.cpp
murmure_LDADD = ${AM_LDFLAGS}
