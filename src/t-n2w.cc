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
#include <clocale>
#include <iostream>
#include <stdexcept>
#if HAVE_LANGINFO_H
#include <langinfo.h>
#endif
#include "Utils.hh"

int main(void) {
  std::wstring w;

  if(!setlocale(LC_CTYPE, "")) {
    perror("locale");
    return 1;
  }

  w = narrowToWide("");
  assert(w == L"");

  w = narrowToWide("whatever");
  assert(w == L"whatever");

#if HAVE_NL_LANGINFO
  const std::string codeset = nl_langinfo(CODESET);
  if(codeset == "UTF-8") {
    w = narrowToWide("unicode: \xC2\xA3");
    assert(w == L"unicode: \u00a3");
    try {
      narrowToWide("bad unicode: \xC2");
      assert(!"unexpectedly succeeded");
    } catch(std::runtime_error &e) {
    }
    try {
      narrowToWide("bad unicode: \xC2whatever");
      assert(!"unexpectedly succeeded");
    } catch(std::runtime_error &e) {
    }
  } else if(codeset == "ISO-8859-1") {
    w = narrowToWide("latin-1: \xA3");
    assert(w == L"latin-1: \u00a3");
  } else {
    std::cerr << "PROBLEM: unrecognized codeset " << codeset << '\n';
    return 77;
  }
#else
  std::cerr << "PROBLEM: no nl_langinfo\n";
  return 77;
#endif

  return 0;
}
