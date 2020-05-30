//
// Copyright (C) 2014 Richard Kettlewell
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#include <config.h>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include "Utils.hh"

int main(void) {

  assert(stringToDouble("0") == 0.0);
  assert(stringToDouble("10") == 10);
  assert(stringToDouble("-1") == -1);

  try {
    stringToDouble("");
    assert(!"unexpectedly succeeded");
  } catch(std::runtime_error &e) {
    assert(std::string(e.what()) == "converting '': invalid numeric syntax");
  }

  try {
    stringToDouble(" ");
    assert(!"unexpectedly succeeded");
  } catch(std::runtime_error &e) {
    assert(std::string(e.what()) == "converting ' ': invalid numeric syntax");
  }

  try {
    stringToDouble("0x");
    assert(!"unexpectedly succeeded");
  } catch(std::runtime_error &e) {
    assert(std::string(e.what()) == "converting '0x': invalid numeric syntax");
  }

  try {
    stringToDouble("1e309");
    assert(!"unexpectedly succeeded");
  } catch(std::runtime_error &e) {
    assert(e.what() == std::string("converting '1e309': ") + strerror(ERANGE));
  }

  return 0;
}
