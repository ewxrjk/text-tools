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
#include "Textfile.hh"
#include <cerrno>
#include <cstring>
#include <cassert>
#include <clocale>
#include <iostream>
#include <sstream>
#include <cstdio>

class TestProcessor: public TextfileProcessor {
  std::wostream &m_os;

public:
  TestProcessor(std::wostream &os): m_os(os) {}

  void begin() {
    TextfileProcessor::begin();
    m_os << "begin\n";
  }

  void bold(bool state) {
    TextfileProcessor::bold(state);
    m_os << "bold " << state << '\n';
  }

  void underline(bool state) {
    TextfileProcessor::underline(state);
    m_os << "underline " << state << '\n';
  }

  void text(const std::wstring &s) {
    m_os << "text " << s << '\n';
  }

  void end() {
    TextfileProcessor::end();
    m_os << "end\n";
  }

  void finished() {
    TextfileProcessor::finished();
    m_os << "finished\n";
  }
};

static void write(const std::string &path, const std::wstring &contents) {
  File f(path, "w");
  for(size_t n = 0; n < contents.size(); ++n)
    f.putw(contents.at(n));
}

int main(void) {
  if(!setlocale(LC_CTYPE, "")) {
    perror("locale");
    return 1;
  }

  remove("test.input");

  Textfile t;

  {
    write("test.input", L"plain text\n");
    t.open("test.input");
    std::wstringstream ss;
    TestProcessor tp(ss);
    t.process(tp);
    std::wstring s = ss.str();
    assert(s
           == L"begin\n"
              "text p\n"
              "text l\n"
              "text a\n"
              "text i\n"
              "text n\n"
              "text  \n"
              "text t\n"
              "text e\n"
              "text x\n"
              "text t\n"
              "text \n\n"
              "end\n");
  }

  {
    write("test.input", L"\ttab\n");
    t.open("test.input");
    std::wstringstream ss;
    TestProcessor tp(ss);
    t.process(tp);
    std::wstring s = ss.str();
    assert(s
           == L"begin\n"
              "text  \n"
              "text  \n"
              "text  \n"
              "text  \n"
              "text  \n"
              "text  \n"
              "text  \n"
              "text  \n"
              "text t\n"
              "text a\n"
              "text b\n"
              "text \n\n"
              "end\n");
  }

  {
    write("test.input", L"xxx\ttab\n");
    t.open("test.input");
    std::wstringstream ss;
    TestProcessor tp(ss);
    t.process(tp);
    std::wstring s = ss.str();
    assert(s
           == L"begin\n"
              "text x\n"
              "text x\n"
              "text x\n"
              "text  \n"
              "text  \n"
              "text  \n"
              "text  \n"
              "text  \n"
              "text t\n"
              "text a\n"
              "text b\n"
              "text \n\n"
              "end\n");
  }

  {
    write("test.input", L"underl\b_i\b_n\b_e\b_\n");
    t.open("test.input");
    std::wstringstream ss;
    TestProcessor tp(ss);
    t.process(tp);
    std::wstring s = ss.str();
    assert(s
           == L"begin\n"
              "text u\n"
              "text n\n"
              "text d\n"
              "text e\n"
              "text r\n"
              "underline 1\n"
              "text l\n"
              "text i\n"
              "text n\n"
              "text e\n"
              "underline 0\n"
              "text \n\n"
              "end\n");
  }

  {
    write("test.input", L"under_\bl_\bi_\bn_\be\n");
    t.open("test.input");
    std::wstringstream ss;
    TestProcessor tp(ss);
    t.process(tp);
    std::wstring s = ss.str();
    assert(s
           == L"begin\n"
              "text u\n"
              "text n\n"
              "text d\n"
              "text e\n"
              "text r\n"
              "underline 1\n"
              "text l\n"
              "text i\n"
              "text n\n"
              "text e\n"
              "underline 0\n"
              "text \n\n"
              "end\n");
  }

  {
    write("test.input", L"bold t\bte\bex\bxt\bt\n");
    t.open("test.input");
    std::wstringstream ss;
    TestProcessor tp(ss);
    t.process(tp);
    std::wstring s = ss.str();
    assert(s
           == L"begin\n"
              "text b\n"
              "text o\n"
              "text l\n"
              "text d\n"
              "text  \n"
              "bold 1\n"
              "text t\n"
              "text e\n"
              "text x\n"
              "text t\n"
              "bold 0\n"
              "text \n\n"
              "end\n");
  }

  {
    write("test.input", L"bold u\bu\b_n\bn\b_d\bd\b_e\be\b_r\br\b_ "
                        L"l\b_\bli\b_\bin\b_\bne\b_\be\n");
    t.open("test.input");
    std::wstringstream ss;
    TestProcessor tp(ss);
    t.process(tp);
    std::wstring s = ss.str();
    assert(s
           == L"begin\n"
              "text b\n"
              "text o\n"
              "text l\n"
              "text d\n"
              "text  \n"
              "bold 1\n"
              "underline 1\n"
              "text u\n"
              "text n\n"
              "text d\n"
              "text e\n"
              "text r\n"
              "bold 0\n"
              "underline 0\n"
              "text  \n"
              "bold 1\n"
              "underline 1\n"
              "text l\n"
              "text i\n"
              "text n\n"
              "text e\n"
              "bold 0\n"
              "underline 0\n"
              "text \n\n"
              "end\n");
  }

  {
    write("test.input", L"s+\bolaris");
    t.open("test.input");
    std::wstringstream ss;
    TestProcessor tp(ss);
    t.process(tp);
    std::wstring s = ss.str();
    assert(s
           == L"begin\n"
              "text s\n"
              "bold 1\n"
              "text o\n"
              "bold 0\n"
              "text l\n"
              "text a\n"
              "text r\n"
              "text i\n"
              "text s\n"
              "end\n");
  }

  {
    write("test.input", L"_\b_");
    t.open("test.input");
    std::wstringstream ss;
    TestProcessor tp(ss);
    t.process(tp);
    std::wstring s = ss.str();
    assert(s
           == L"begin\n"
              "underline 1\n"
              "text _\n"
              "underline 0\n"
              "end\n");
  }

  {
    write("test.input", L".\b");
    t.open("test.input");
    std::wstringstream ss;
    TestProcessor tp(ss);
    t.process(tp);
    std::wstring s = ss.str();
    assert(s
           == L"begin\n"
              "text .\n"
              "text \b\n"
              "end\n");
  }

  assert(remove("test.input") == 0);

  return 0;
}
