AUTOMAKE_OPTIONS = foreign
SUBDIRS = src SQL
AM_LDFLAGS = -lsqlite3 -lpthread

clean-local:
	mv src/Makefile.am.bak src/Makefile.am
	mv src/murmure/modulefacade.orig.cpp src/murmure/modulefacade.cpp

