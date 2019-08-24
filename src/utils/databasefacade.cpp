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

#include <utils/databasefacade.hpp>

#include <sqlite3.h>

using namespace database;

std::string databasePath;
sqlite3* db;
sqlite3_stmt* statement;
bool isOpen = false; //Is database open?

void database::init(const std::string& dbPath) {
  databasePath = dbPath;
}

/**
 * @function open
 * @description Open database
 * @param std::string&: error string pointer
 * @returns bool: True if open succeeded
**/

bool open(std::string& error) {

  if (!isOpen) {
    if (sqlite3_open(databasePath.c_str(), &db) == SQLITE_OK) {
      isOpen = true;
      return true;
    } else {
      error = std::string(sqlite3_errmsg(db));
      return false;
    }
  } else {
    error = "Database already open";
    return true;
  }
}

/**
 * @function close
 * @description Close database
 * @param std::string&: error string pointer
 * @returns bool: True if closed successfully; if database wasn't open, true will be returned anyway
**/

bool close(std::string& error) {

  //If database is open, try to close it
  if (isOpen) {
    if (sqlite3_close(db) != SQLITE_OK) {
      //Close failed
      error = std::string(sqlite3_errmsg(db));
      return false;
    }
  } else {
    error = "Database is already closed";
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
 * @param std::string&: error string pointer
 * @returns bool: True if succeeded
**/

bool database::exec(std::string query, std::string& error) {

  //Open database
  if (!open(error)) {
    return false;
  }
  char* errMsg = 0;
  bool rc;
  //Exec query
  if (sqlite3_exec(db, query.c_str(), NULL, 0, &errMsg) == SQLITE_OK) {
    sqlite3_free(errMsg);
    rc = true;
  } else {
    error = std::string(errMsg);
    sqlite3_free(errMsg);
    rc = false;
  }
  if (!close(error)) {
    return false;
  }
  return rc;
}

/**
 * @function select
 * @description Select from database
 * @param std::string query: query to exec
 * @param std::string&: pointer to error string
 * @param std::vector<std::vector<std::string>>*: pointer vector of rows of columns (strings); nullptr is returned if failed
 * @returns bool: True if select succeeded
**/

bool database::select(std::vector<std::vector<std::string>>* result, std::string query, std::string& error) {
  //Open database
  if (!open(error)) {
    return false;
  }
  //Empty result vector
  result->clear();

  size_t vectorSize = 0;
  if (sqlite3_prepare_v2(db, query.c_str(), query.size(), &statement, NULL) != SQLITE_OK) {
    error = std::string(sqlite3_errmsg(db));
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
      error = std::string(sqlite3_errmsg(db));
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
  if (!close(error)) {
    return false;
  }
  return res;
}
