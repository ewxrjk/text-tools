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
#include "HtmlOutput.hh"
#include "Characters.hh"

void HtmlOutput::bold(bool state) {
  if(state) {
    bold_index = index;
    output.printf("<b>");
  } else {
    if(underline_index > bold_index) {
      underline(false);
      output.printf("</b>");
      bold_index = 0;
      underline(true);
    } else {
      output.printf("</b>");
      bold_index = 0;
    }
  }
  ++index;
}

void HtmlOutput::underline(bool state) {
  if(state) {
    underline_index = index;
    output.printf(underlineAsItalic ? "<i>" : "<u>");
  } else {
    if(bold_index > underline_index) {
      bold(false);
      output.printf(underlineAsItalic ? "</i>" : "</u>");
      underline_index = 0;
      bold(true);
    } else {
      output.printf(underlineAsItalic ? "</i>" : "</u>");
      underline_index = 0;
    }
  }
  ++index;
}

void HtmlOutput::text(const std::wstring &s) {
  for(std::wstring::size_type n = 0; n < s.size(); ++n) {
    wchar_t c = s.at(n);
    if((c >= SPACE && c <= TILDE
        && c != LESS_THAN_SIGN
        && c != AMPERSAND)
       || c == LINE_FEED)
      output.put((int)c);
    else
      output.printf("&#%ld;", (long)c);
  }
}
