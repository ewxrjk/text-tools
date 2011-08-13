//
// Copyright (C) 2011 Richard Kettlewell
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
#include "Utils.hh"
#include <cstdlib>
#include <vector>
#include <cerrno>
#include <cstring>
#include <stdexcept>

double stringToDouble(const std::string &s) {
  const char *sc = s.c_str();
  char *e;
  errno = 0;
  double n = strtod(sc, &e);
  if(errno)
    throw std::runtime_error("converting '" + s + "': " + strerror(errno));
  if(e == sc || *e)
    throw std::runtime_error("converting '" + s + "': invalid numeric syntax");
  return n;
}
