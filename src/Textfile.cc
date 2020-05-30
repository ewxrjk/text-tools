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
#include "File.hh"
#include "Characters.hh"
#include <stdexcept>

Textfile::Textfile(): tabstop(8) {}

Textfile::~Textfile() {}

void Textfile::open(const std::string &path) {
  contents.clear();
  File f(path, "r");
  wint_t c;
  while((c = f.getw()) != WEOF)
    contents.push_back(c);
}

void Textfile::process(TextfileProcessor &processor) {
  processor.begin();
  size_t column = 0;
  std::wstring::size_type n = 0;
  const std::wstring::size_type limit = contents.size();
  while(n < limit) {
    // First deal with simple text (i.e. no tabs, newlines or markup)
    if(contents.at(n) != CHARACTER_TABULATION && contents.at(n) != LINE_FEED
       && !(n + 1 < limit && contents.at(n + 1) == BACKSPACE)) {
      processor.character(contents.at(n));
      ++column;
      ++n;
      continue;
    }
    // Tabs
    if(contents.at(n) == CHARACTER_TABULATION) {
      processor.character(L' ');
      ++column;
      while(column % tabstop) {
        processor.character(L' ');
        ++column;
      }
      ++n;
      continue;
    }
    // Newlines
    if(contents.at(n) == LINE_FEED) {
      processor.character(LINE_FEED);
      column = 0;
      ++n;
      continue;
    }
    // Bold & underline
    if(n + 2 < limit && contents.at(n + 1) == BACKSPACE) {
      wchar_t c = contents.at(n++);
      bool b = false, u = false;
      while(n + 1 < limit && contents.at(n) == BACKSPACE) {
        ++n;
        if(c == '+' && contents.at(n) == 'o')
          c = 'o'; // hack for solaris
        if(c == '_') {
          c = contents.at(n);
          u = true;
        } else {
          if(contents.at(n) == c)
            b = true;
          else if(contents.at(n) == '_' && c != '_')
            u = true;
        }
        ++n;
      }
      processor.character(c, b, u);
      ++column;
      continue;
    }
    // Junk
    processor.character(contents.at(n));
    ++column;
    ++n;
  }
  processor.end();
}
