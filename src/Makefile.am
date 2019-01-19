LIBS = 
INCLUDE = ../include/
AM_CXXFLAGS = -Wall -std=c++11 -I ${INCLUDE}
AM_LDFLAGS = -lsqlite3 -lpthread

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = Murmure
Murmure_SOURCES = murmure.cpp mibparser/mibparser.cpp mibscheduler/event.cpp mibscheduler/scheduledevent.cpp mibscheduler/scheduler.cpp murmure/primitives/counter.cpp murmure/primitives/gauge.cpp murmure/primitives/integer.cpp murmure/primitives/ipaddress.cpp murmure/primitives/objectid.cpp murmure/primitives/octet.cpp murmure/primitives/sequence.cpp murmure/primitives/string.cpp murmure/primitives/timeticks.cpp murmure/mibtable.cpp murmure/modulefacade.cpp murmure/oid.cpp utils/databasefacade.cpp utils/getopts.cpp utils/logger.cpp utils/strutils.cpp
Murmure_LDADD = ${AM_LDFLAGS}
