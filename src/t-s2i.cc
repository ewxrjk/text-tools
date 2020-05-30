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

  assert(stringToInt("0") == 0.0);
  assert(stringToInt("10") == 10);
  assert(stringToInt("-1") == -1);

  try {
    stringToInt("");
    assert(!"unexpectedly succeeded");
  } catch(std::runtime_error &e) {
    assert(e.what() == "converting '': " + std::string(strerror(EINVAL)));
  }

  try {
    stringToInt(" ");
    assert(!"unexpectedly succeeded");
  } catch(std::runtime_error &e) {
    assert(e.what() == "converting ' ': " + std::string(strerror(EINVAL)));
  }

  try {
    stringToInt("0z");
    assert(!"unexpectedly succeeded");
  } catch(std::runtime_error &e) {
    assert(e.what() == "converting '0z': " + std::string(strerror(EINVAL)));
  }

  try {
    stringToInt("0x100");
    assert(!"unexpectedly succeeded");
  } catch(std::runtime_error &e) {
    assert(e.what() == "converting '0x100': " + std::string(strerror(EINVAL)));
  }

  try {
    stringToInt("4294967296");
    assert(!"unexpectedly succeeded");
  } catch(std::runtime_error &e) {
    assert(e.what()
           == std::string("converting '4294967296': ") + strerror(ERANGE));
  }

  try {
    stringToInt("9223372036854775808");
    assert(!"unexpectedly succeeded");
  } catch(std::runtime_error &e) {
    assert(e.what()
           == std::string("converting '9223372036854775808': ")
                  + strerror(ERANGE));
  }

  return 0;
}
