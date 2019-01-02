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

#include <utils/databasefacade.hpp>

#include <sqlite3.h>

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

bool database::open(std::string* error) {

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

bool database::close(std::string* error) {

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

  isOpen = false;
  db = NULL;
  statement = NULL;

  return true;
}

/**
 * @function exec
 * @description Exec a change in the database (e.g. update, insert, create, delete)
 * @param std::string query to exec
 * @param std::string*: error string pointer
 * @returns bool: True if succeeded
**/

bool database::exec(std::string query, std::string* error) {

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

bool database::select(std::vector<std::vector<std::string>>* result, std::string query, std::string* error) {

  if (!isOpen) {
    *error = "Database is not open";
    return false;
  }

  //Empty result vector
  result->clear();

  size_t vectorSize = 0;
  if (sqlite3_prepare_v2(db, query.c_str(), query.size(), &statement, NULL) != SQLITE_OK) {
    *error = std::string(sqlite3_errmsg(db));
    sqlite3_finalize(statement);
    return false;
  }
  //Iterate over rows
  int stepCode;
  bool res = true;
  do {
    stepCode = sqlite3_step(statement);
    if (stepCode != SQLITE_ROW && stepCode != SQLITE_DONE) {
      //Is error
      *error = std::string(sqlite3_errmsg(db));
      res = false;
    } else if (stepCode == SQLITE_ROW) {
      //Is SQLITE_ROW
      const int columnCount = sqlite3_column_count(statement);
      std::vector<std::string> row;
      row.reserve(columnCount);
      for (int i = 0; i < columnCount; i++) {
        //Push columns to row vector
        std::string column = std::string(reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(statement, i))));
        row.push_back(column);
      }
      //Push row to result vector
      result->resize(vectorSize++);
      result->push_back(row);
    }
  } while (stepCode == SQLITE_ROW);
  //SQLITE_DONE

  sqlite3_reset(statement);
  sqlite3_finalize(statement);
  return res;
}