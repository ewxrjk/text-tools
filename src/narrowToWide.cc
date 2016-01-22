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
#include <cstring>
#include <cerrno>
#include <stdexcept>

std::wstring narrowToWide(const std::string &s) {
  size_t n = mbstowcs(NULL, s.c_str(), 0);
  if(n == (size_t)-1)
    throw std::runtime_error(std::string("mbstowcs: ") + strerror(errno));
  std::vector<wchar_t> buffer(n+1);
  mbstowcs(&buffer[0], s.c_str(), n);
  return std::wstring(&buffer[0], n);
}
