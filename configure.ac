AC_PREREQ([2.69])
AC_INIT([murmure], [1.1.0], [https://github.com/ChristianVisintin/Murmure])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_SRCDIR([include/core/mibtable.hpp])
AC_LANG(C++)
AC_REQUIRE_CPP


AC_PREFIX_DEFAULT(["/usr/local"])
# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
#Libsqlite3
AC_SEARCH_LIBS([sqlite3_open], [sqlite3], [], [
  AC_MSG_ERROR([unable to find sqlite3_open()])
])
#Libpthread
AC_SEARCH_LIBS([pthread_join], [pthread], [], [
  AC_MSG_ERROR([unable to find pthread()])
])

# Checks for header files.
AC_CHECK_HEADERS([sqlite3.h thread])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_CHECK_FUNCS([select setenv strtol system])

#Check for Python
AC_CHECK_PROG(PYTHON, python3, yes)
if test "${PYTHON}" != "yes"; then
  AC_MSG_ERROR([No python3 in $(PATH), consider doing apt install python3])
fi

#Check for sqlite3
AC_CHECK_PROG(SQLITE3, sqlite3, yes)
if test "${SQLITE3}" != "yes"; then
  AC_MSG_ERROR([No sqlite3 in $(PATH), consider doing apt install sqlite3])
fi

#defines

AC_ARG_VAR([DBPATH], [Murmure database location])
AC_ARG_VAR([LOGFILE], [Murmure logfile location])
AC_ARG_VAR([LOGLEVEL], [Murmure Log level (1:FATAL-5:DEBUG)])
AC_ARG_VAR([SQLFILE], [Murmure SQL file for build])

CPPFLAGS=

#DB Path
if test "${DBPATH}" != ""; then
  CPPFLAGS="${CPPFLAGS} -D DBPATH=${DBPATH}"
fi 

#LogFile
if test "${LOGFILE}" != ""; then
  CPPFLAGS="${CPPFLAGS} -D LOGFILE=${LOGFILE}"
fi 

#LogLevel
if test "${LOGLEVEL}" != ""; then
  CPPFLAGS="${CPPFLAGS} -D LOGLEVEL=${LOGLEVEL}"
fi

#SQL
if test "${SQLFILE}" != ""; then
  CPPFLAGS="${CPPFLAGS} -D SQLFILE=${SQLFILE}"
elif test "${PREFIX}" != ""; then
  CPPFLAGS="${CPPFLAGS} -D SQLFILE=${PREFIX}/SQL/mibtable.sql"
fi

AC_CONFIG_FILES([Makefile src/Makefile SQL/Makefile])
AC_OUTPUT
