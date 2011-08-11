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
#include "Textfile.hh"

void TextfileProcessor::begin() {}

void TextfileProcessor::character(wchar_t c, bool isBold, bool isUnderlined) {
  if(isBold != boldState)
    bold(boldState = isBold);
  if(isUnderlined != underlineState)
    underline(underlineState = isUnderlined);
  std::wstring s; s += c;
  text(s);
}

void TextfileProcessor::bold(bool) {}
void TextfileProcessor::underline(bool) {}

void TextfileProcessor::end() { 
  if(boldState)
    bold(boldState = false);
  if(underlineState)
    underline(underlineState = false);
}
