AUTOMAKE_OPTIONS = foreign
SUBDIRS = src SQL
AM_LDFLAGS = -lsqlite3 -lpthread

clean-local:
	if [ -e "src/Makefile.am.bak" ]; then mv src/Makefile.am.bak src/Makefile.am; fi
	if [ -e "src/murmure/modulefacade.orig.cpp" ]; then mv src/murmure/modulefacade.orig.cpp src/murmure/modulefacade.cpp; fi

