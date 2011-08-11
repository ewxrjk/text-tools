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
#ifndef CAIROOUTPUT_HH
#define CAIROOUTPUT_HH
/** @file src/CairoOutput.hh
 * @brief Textfile processor that writes to a Cairo surface
 */
#include "Textfile.hh"
#include <cairomm/cairomm.h>

/** @brief Textfile processor that writes to a Cairo surface */
class CairoOutput: public TextfileProcessor {
public:
  /** @brief Construct a CairoOutput
   * @param context Rendering context
   * @param page Function called when a complete page has been rendered
   *
   * @p context should already have a suitable font configured.
   *
   * @bug You can't choose the text and background color.
   */
  CairoOutput(Cairo::RefPtr<Cairo::Context> context,
              Cairo::RefPtr<Cairo::FontFace> normalFont_,
              Cairo::RefPtr<Cairo::FontFace> boldFont_,
              Cairo::RefPtr<Cairo::FontFace> italicFont_,
              Cairo::RefPtr<Cairo::FontFace> boldItalicFont_,
              void (*page)(CairoOutput *));

  /** @brief Destroy a CairoOutput */
  ~CairoOutput();

  void bold(bool state);
  void underline(bool state);
  void text(const std::wstring &s);
  void finished();

private:
  Cairo::RefPtr<Cairo::Context> context;
  Cairo::RefPtr<Cairo::FontFace> normalFont;
  Cairo::RefPtr<Cairo::FontFace> boldFont;
  Cairo::RefPtr<Cairo::FontFace> italicFont;
  Cairo::RefPtr<Cairo::FontFace> boldItalicFont;
  void (*page)(CairoOutput *);
  bool boldState, underlineState;
  int row, column, rows, columns;
};

#endif /* CAIROOUTPUT_HH */
