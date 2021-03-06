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
#include <cstdio>

std::string wideToUtf8(const std::wstring &s) {
  std::string u;
  for(std::wstring::size_type n = 0; n < s.size(); ++n) {
    const unsigned c = s.at(n);
    if(c <= 0x7F)
      u += c;
    else if(c <= 0x07ff) {
      u += (char)(0xC0 + (c >> 6));
      u += (char)(0x80 + (c & 0x3F));
    } else if(c <= 0xffff) {
      u += (char)(0xE0 + (c >> 12));
      u += (char)(0x80 + ((c >> 6) & 0x3F));
      u += (char)(0x80 + (c & 0x3F));
    } else {
      u += (char)(0xF0 + (c >> 18));
      u += (char)(0x80 + ((c >> 12) & 0x3F));
      u += (char)(0x80 + ((c >> 6) & 0x3F));
      u += (char)(0x80 + (c & 0x3F));
    }
  }
  return u;
}
