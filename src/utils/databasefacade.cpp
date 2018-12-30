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

#include <sqlite3.h>
#include <utils/databasefacade.hpp>

using namespace database;

sqlite3* db;
sqlite3_stmt* statement;
bool isOpen = false; //Is database open?

/**
 * @function open
 * @description Open database
 * @param std::string*: error string pointer
 * @returns bool: True if open succeeded
**/

bool open(std::string* error) {

  if (!isOpen) {
    if (sqlite3_open(DATABASEPATH, &db) == SQLITE_OK) {
      isOpen = true;
      return true;
    } else {
      *error = std::string(sqlite3_errmsg(db));
      return false;
    }
  } else {
    *error = "Database already open";
    return true;
  }
}

/**
 * @function close
 * @description Close database
 * @param std::string*: error string pointer
 * @returns bool: True if closed successfully; if database wasn't open, true will be returned anyway
**/

bool close(std::string* error) {

  //If database is open, try to close it
  if (isOpen) {
    if (sqlite3_close(db) != SQLITE_OK) {
      //Close failed
      *error = std::string(sqlite3_errmsg(db));
      return false;
    }
  } else {
    *error = "Database is already closed";
  }

  return true;
}

/**
 * @function exec
 * @description Exec a change in the database (e.g. update, insert, create, delete)
 * @param std::string query to exec
 * @param std::string*: error string pointer
 * @returns bool: True if succeeded
**/

bool exec(std::string query, std::string* error) {

  if (!isOpen) {
    *error = "Database is not open";
    return false;
  }

  char* errMsg = 0;
  //Exec query
  if (sqlite3_exec(db, query.c_str(), NULL, 0, &errMsg) == SQLITE_OK) {
    sqlite3_free(errMsg);
    return true;
  } else {
    *error = std::string(errMsg);
    sqlite3_free(errMsg);
    return false;
  }
}

/**
 * @function select
 * @description Select from database
 * @param std::string query: query to exec
 * @param std::string*: pointer to error string
 * @param std::vector<std::vector<std::string>>*: pointer vector of rows of columns (strings); nullptr is returned if failed
 * @returns bool: True if select succeeded
**/

bool select(std::vector<std::vector<std::string>>* result, std::string query, std::string* error) {

  if (!isOpen) {
    *error = "Database is not open";
    return false;
  }

  //Empty result vector
  result->clear();

  size_t vectorSize = 0;
  sqlite3_prepare(db, query.c_str(), -1, &statement, NULL);
  if (sqlite3_step(statement) != SQLITE_DONE) {
    *error = std::string(sqlite3_errmsg(db));
    sqlite3_finalize(statement);
    return false;
  }
  while (sqlite3_column_text(statement, 0)) {
    const int columnCount = sqlite3_column_count(statement);
    std::vector<std::string> row;
    for (int i = 0; i < columnCount; i++) {
      row.reserve(columnCount);
      //Push columns to row vector
      row.push_back(std::string(reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(statement, i)))));
    }
    //Push row to result vector
    result->resize(++vectorSize);
    result->push_back(row);
    //Step forward
    sqlite3_step(statement);
  }

  sqlite3_finalize(statement);
  return true;
}