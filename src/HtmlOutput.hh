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
#ifndef HTMLOUTPUT_H
#define HTMLOUTPUT_H
/** @file src/HtmlOutput.hh
 * @brief Textfile processor implementation that generates HTML
 */

#include "File.hh"
#include "Textfile.hh"

/** @brief Textfile processor implementation that generates HTML
 */
class HtmlOutput: public TextfileProcessor {
public:
  /** @brief Output file
   *
   * This must be opened by the caller.
   */
  File output;

  /** @brief Construct an HTML output text processor */
  inline HtmlOutput():
      index(1), bold_index(0), underline_index(0), underlineAsItalic(false) {}

  void bold(bool state);
  void underline(bool state);

  void text(const std::wstring &s);

  /** @brief Set italic mode
   * @param it If @c true, underlined text will be rendered in italic */
  inline void setItalic(bool it) {
    underlineAsItalic = it;
  }

private:
  size_t index;
  size_t bold_index, underline_index;
  bool underlineAsItalic;
};

#endif /* HTMLOUTPUT_H */
