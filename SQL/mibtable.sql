/**
 *  Murmure - Net-SNMP MIB Versatile Extender
 *  Developed by Christian Visintin
 * 
 * 	Copyright (C) 2018 Christian Visintin
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

-- Murmure MIB table Version 1.0
-- Database for storing MIB's OID and scheduled events
-- Written and designed by Christian Visintin

CREATE TABLE oids (
  oid VARCHAR(64) PRIMARY KEY NOT NULL,
  name VARCHAR(255),
  datatype VARCHAR(32) NOT NULL,
  value VARCHAR(128),
  accessmode INTEGER NOT NULL
);

CREATE TABLE scheduled_events (
  event_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  mode VARCHAR(1) NOT NULL,
  timeout INTEGER,
  oid VARCHAR(64) NOT NULL,
  FOREIGN KEY(oid) REFERENCES oids(oid)
);

CREATE TABLE events_commands (
  command_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  command VARCHAR(256) NOT NULL,
  execution_order INTEGER NOT NULL,
  event_id INTEGER NOT NULL,
  FOREIGN KEY(event_id) REFERENCES scheduled_events(event_id)
);
