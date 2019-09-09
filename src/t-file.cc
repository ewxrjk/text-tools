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
#include "File.hh"
#include "Utils.hh"
#include <cerrno>
#include <cstring>
#include <cassert>
#if HAVE_LANGINFO_H
# include <langinfo.h>
#endif
#include <clocale>
#include <iostream>

int main(void) {

  if(!setlocale(LC_CTYPE, "")) {
    perror("locale");
    return 1;
  }
  std::string pound;
#if HAVE_NL_LANGINFO
  const std::string codeset = nl_langinfo(CODESET);
  if(codeset == "UTF-8") {
    pound = "\xC2\xA3";
  } else if(codeset == "ISO-8859-1") {
    pound = "\xA3";
  } else {
    std::cerr << "PROBLEM: unrecognized codeset " << codeset << '\n';
    return 77;
  }
#else
  std::cerr << "PROBLEM: no nl_langinfo\n";
  return 77;
#endif

  remove("test.tmp");

  try {
    File f("test.tmp", "r");
    assert(!"unexpectedly succeeded");
  } catch(std::runtime_error &e) {
    assert(e.what() == "opening test.tmp: " + std::string(strerror(ENOENT)));
  }

  /* Write out the file */
  File f("test.tmp", "w");
  f.printf("%s\n", "test line");
  f.printf("%s", pound.c_str());
  f.put('\n');

  try {
    f.open("whatever", "r");
    assert(!"unexpectedly succeeded");
  } catch(std::logic_error &e) {
    assert(e.what() == std::string("File::open but already open"));
  }

  f.close();

  try {
    f.printf("xxx");
    assert(!"unexpectedly succeeded");
  } catch(std::logic_error &e) {
    assert(e.what() == std::string("File::printf when closed"));
  }

  try {
    f.put('!');
    assert(!"unexpectedly succeeded");
  } catch(std::logic_error &e) {
    assert(e.what() == std::string("File::put when closed"));
  }

  try {
    f.putw(L'!');
    assert(!"unexpectedly succeeded");
  } catch(std::logic_error &e) {
    assert(e.what() == std::string("File::putw when closed"));
  }

  try {
    f.flush();
    assert(!"unexpectedly succeeded");
  } catch(std::logic_error &e) {
    assert(e.what() == std::string("File::flush when closed"));
  }

  try {
    f.close();
    assert(!"unexpectedly succeeded");
  } catch(std::logic_error &e) {
    assert(e.what() == std::string("File::close when closed"));
  }

  try {
    f.getw();
    assert(!"unexpectedly succeeded");
  } catch(std::logic_error &e) {
    assert(e.what() == std::string("File::getw when closed"));
  }

  try {
    f.get();
    assert(!"unexpectedly succeeded");
  } catch(std::logic_error &e) {
    assert(e.what() == std::string("File::get when closed"));
  }

  /* Read it back in byte by byte */
  {
    f.open("test.tmp", "r");
    std::string t;
    int ch;
    while((ch = f.get()) != EOF)
      t += (char)ch;
    assert(t == "test line\n" + pound + "\n");
    f.close();
  }

  /* Read it back as wide characters */
  {
    f.open("test.tmp", "r");
    std::wstring w;
    wint_t wch;
    while((wch = f.getw()) != WEOF)
      w += wch;
    assert(w == L"test line\n\u00A3\n");
    f.close();
  }

  assert(remove("test.tmp") == 0);

  return 0;
}
