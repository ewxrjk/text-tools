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
#include "Utils.hh"

int main(void) {
  std::string s;

  s = wideToUtf8(L"");
  assert(s == "");

  s = wideToUtf8(L"whatever");
  assert(s == "whatever");

  /* 6.4.3: "A universal character name shall not specify a character whose
   * short identifier is less than 00A0 other than 0024 ($), 0040 (@), or 0060
   * (‘), nor one in the range D800 through DFFF inclusive."
   */
  static const wchar_t c99_is_moronic[] = {L'u', L'n', L'i', L'c',   L'o', L'd',
                                           L'e', L':', L' ', 0x0080, 0};
  s = wideToUtf8(c99_is_moronic);
  assert(s == "unicode: \xC2\x80");

  s = wideToUtf8(L"unicode: \u07ff");
  assert(s == "unicode: \xDF\xBF");

  s = wideToUtf8(L"unicode: \u0800");
  assert(s == "unicode: \xE0\xA0\x80");

  s = wideToUtf8(L"unicode: \uffff");
  assert(s == "unicode: \xEF\xBF\xBF");

  s = wideToUtf8(L"unicode: \U00010000");
  assert(s == "unicode: \xF0\x90\x80\x80");

  s = wideToUtf8(L"unicode: \U0010FFFF");
  assert(s == "unicode: \xF4\x8F\xBF\xBF");

  return 0;
}
