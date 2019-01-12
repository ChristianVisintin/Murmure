# Contributing

When contributing to this repository, please first discuss the change you wish to make via issue, email, or any other method with the owners of this repository before making a change.

Please note we have a code of conduct, please follow it in all your interactions with the project.

## Pull Request Process

1. Ensure any install or build dependencies are removed before the end of the layer when doing a  build.
2. Update the README.md with details of changes to the interface, this includes new line in the changelog, new modules if added, new command line options if added, etc.
3. Increase the version numbers in any examples files and the README.md to the new version that this Pull Request would represent. The versioning scheme we use is [SemVer](http://semver.org/).
4. You may merge the Pull Request in once you have the sign-off of two other developers, or if you  do not have permission to do that, you may request the second reviewer to merge it for you.

### Pull Request Process for modules

Murmure pull requests basically involve module integration in the master branch.
This procedure describes how to contribute in module integration and in module implementation for the Murmure project. Before starting to work on a module be sure to have a full comprehension of what modules are reading the [Readme Module Chapter](https://github.com/ChristianVisintin/Murmure/blob/master/README.md#Modules)

#### Requirements

* The SNMP type you will implemenet the module for, must be standard and documented in the IETF standards
* Python3
* Knowledge of C++11

#### Module implementation

Once you've decided the SNMP SYNTAX you're going to implement, the first thing to do is writing the header file of your module. To do this move into the project folder under ```/include/murmure/modules/```. Here, supposing you're going to create your module, which implements the fake type called "dummy" create the file dummy.hpp

Each module must implement the [Module interface](https://github.com/ChristianVisintin/Murmure/blob/master/include/murmure/modules/module.hpp) so your header file will have the following structure:  

```cpp
/**
 *  Murmure - Net-SNMP MIB Versatile Extender
 *  Developed by $YOURNAME
 *
 * Copyright (C) $CURRENTYEAR $YOURNAME
 *
 * This file is part of "Murmure"
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

#ifndef DUMMY_HPP
#define DUMMY_HPP

#include <murmure/modules/module.hpp>

/**
 * @name: DUMMY
 * @ref: <https://www.ietf.org/rfc/rfcxxx.txt>
**/

namespace murmure {

class Dummy : public Module {
public:
  Dummy();
  ~Dummy();
  bool setValue(std::string value);
  bool setValue(std::string oid, std::string value);
  std::string getPrintableValue();
  std::string getPrimitiveType();
};

} // namespace murmure

#endif
```

As you can see the header file is pretty simple, you just have to do are the following actions:

* replace "dummy" with the type you're going to implement
* update @ref with the correct IETF document
* Update yourname in the license section with yours
* Update currentyear in the license section with the current year

Once the header as been filled, you have to implement all these function, to do this move your location to /src/murmure/modules/ and create the file dummy.cpp
The module implementation will have the following structure:

```cpp
/**
 *  Murmure - Net-SNMP MIB Versatile Extender
 *  Developed by $YOURNAME
 *
 * Copyright (C) $CURRENTYEAR $YOURNAME
 *
 * This file is part of "Murmure"
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

#include <murmure/modules/dummy.hpp>
#include <murmure/primitives/???.hpp>

using namespace murmure;

/**
 * @function Dummy
 * @description Dummy class constructor
 * NOTE: set primitive to nullptr; set primitiveType to the primitive we'll use
**/

Dummy::Dummy() {
  primitive = nullptr;
  primitiveType = PRIMITIVE_???;
}

/**
 * @function ~Dummy
 * @description Dummy class destructor
**/

Dummy::~Dummy() {
  if (primitive != nullptr) {
    ???<???>* primitivePtr = reinterpret_cast<???<???>*>(primitive);
    delete primitivePtr;
  }
  primitive = nullptr;
}

/**
 * @function setValue
 * @description initialize primitive and set its value
 * @param std::string
 * @returns bool
 * NOTE: must be called in initialization only
**/

bool Dummy::setValue(std::string value) {
  if (primitive == nullptr) {
    primitive = new ???<???>(value);
  }
  return true;
}

/**
 * @function setValue
 * @description update primitive value associated to this module
 * @param std::string
 * @param std::string
 * @returns bool
**/

bool Dummy::setValue(std::string oid, std::string value) {
  //Check if primitive is initialized
  //NOTE: transform value if your module needs to do that
  if (primitive == nullptr) {
    return false;
  }
  ???<???>* primitivePtr = reinterpret_cast<???<???>*>(primitive);
  return primitivePtr->setValue(oid, value);
}

/**
 * @function getPrintableValue
 * @description get printable value from its primitive
 * @returns std::string
**/

std::string Dummy::getPrintableValue() {
  //Check if primitive is initialized
  //NOTE: transform value if your module needs to do that
  if (primitive == nullptr) {
    return "";
  }
  ???<???>* primitivePtr = reinterpret_cast<???<???>*>(primitive);
  return primitivePtr->getPrintableValue();
}

/**
 * @function getPrimitiveType
 * @description get primitive type name associated to this module
 * @returns std::string
**/

std::string Dummy::getPrimitiveType() {
  return primitiveType;
}

```

Here it comes the hardest part (not really hard though).
All you have to do is:

* include a primitive header. Yep, all modules must interface themselves with a Primitive type, which handles the communication with the database and provide you a printable value to return. This is necessary because net-snmp only accepts a few types (which in Murmure are called indeed Primitives).
* Replace ??? with primitive class and the template the primitive uses (e.g if we implement String primitive then we'll instance new String with template std::string)
* Implement any data manipulation in setValue and getPrintableValue if required by your module
* Change YOURNAME with your name in the license section
* Change CURRENTYEAR with the current year in the license section

#### Test your module

In order to test your module rebuild the project including the module in the configuration and create a test MIB to check if it works properly. Remember that all the modules must satisfy the following requirements:

* Must be memory safe: they must free all the memory they allocate
* Must be modular and completely independent with all other Murmure components. It can't instance any other project class.
* Must implement the Module interface
* Must report the name in the header
* Must report the IETF reference in the header

Once you've checked everything is satisfied you can make your pull request.

Thank you for any contribution!  
Christian Visintin

## Code of Conduct

### Our Pledge

In the interest of fostering an open and welcoming environment, we as
contributors and maintainers pledge to making participation in our project and
our community a harassment-free experience for everyone, regardless of age, body
size, disability, ethnicity, gender identity and expression, level of experience,
nationality, personal appearance, race, religion, or sexual identity and
orientation.

### Our Standards

Examples of behavior that contributes to creating a positive environment
include:

* Using welcoming and inclusive language
* Being respectful of differing viewpoints and experiences
* Gracefully accepting constructive criticism
* Focusing on what is best for the community
* Showing empathy towards other community members

Examples of unacceptable behavior by participants include:

* The use of sexualized language or imagery and unwelcome sexual attention or advances
* Trolling, insulting/derogatory comments, and personal or political attacks
* Public or private harassment
* Publishing others' private information, such as a physical or electronic address, without explicit permission
* Other conduct which could reasonably be considered inappropriate in a professional setting

### Our Responsibilities

Project maintainers are responsible for clarifying the standards of acceptable
behavior and are expected to take appropriate and fair corrective action in
response to any instances of unacceptable behavior.

Project maintainers have the right and responsibility to remove, edit, or
reject comments, commits, code, wiki edits, issues, and other contributions
that are not aligned to this Code of Conduct, or to ban temporarily or
permanently any contributor for other behaviors that they deem inappropriate,
threatening, offensive, or harmful.

### Scope

This Code of Conduct applies both within project spaces and in public spaces
when an individual is representing the project or its community. Examples of
representing a project or community include using an official project e-mail
address, posting via an official social media account, or acting as an appointed
representative at an online or offline event. Representation of a project may be
further defined and clarified by project maintainers.

### Enforcement

Instances of abusive, harassing, or otherwise unacceptable behavior may be
reported by contacting the project team at [INSERT EMAIL ADDRESS]. All
complaints will be reviewed and investigated and will result in a response that
is deemed necessary and appropriate to the circumstances. The project team is
obligated to maintain confidentiality with regard to the reporter of an incident.
Further details of specific enforcement policies may be posted separately.

Project maintainers who do not follow or enforce the Code of Conduct in good
faith may face temporary or permanent repercussions as determined by other
members of the project's leadership.

### Attribution

This Code of Conduct is adapted from the [Contributor Covenant][homepage], version 1.4,
available at [http://contributor-covenant.org/version/1/4][version]

[homepage]: http://contributor-covenant.org
[version]: http://contributor-covenant.org/version/1/4/
