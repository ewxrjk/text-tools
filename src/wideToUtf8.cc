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

std::string wideToUtf8(const std::wstring &s) {
  std::string u;
  for(std::wstring::size_type n = 0; n < s.size(); ++n) {
    const unsigned c = s.at(n);
    if(c <= 0x7F) 
      u += c;
    else if(c <= 0x0800) {
      u += 0xC0 + (c >> 6);
      u += 0x80 + (c & 0x3F);
    } else if(c <= 0x10000) {
      u += 0xE0 + (c >> 18);
      u += 0x80 + ((c >> 6) & 0x3F);
      u += 0x80 + (c & 0x3F);
    } else {
      u += 0xE0 + (c >> 24);
      u += 0x80 + ((c >> 18) & 0x3F);
      u += 0x80 + ((c >> 6) & 0x3F);
      u += 0x80 + (c & 0x3F);
    }
  }
  return u;
}
