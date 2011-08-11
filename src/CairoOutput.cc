#include <config.h>
#include "CairoOutput.hh"
#include "Utils.hh"

CairoOutput::CairoOutput(Cairo::RefPtr<Cairo::Context> context_,
                         Cairo::RefPtr<Cairo::FontFace> normalFont_,
                         Cairo::RefPtr<Cairo::FontFace> boldFont_,
                         Cairo::RefPtr<Cairo::FontFace> italicFont_,
                         Cairo::RefPtr<Cairo::FontFace> boldItalicFont_,
                         void (*page_)(CairoOutput *)):
  context(context_),
  normalFont(normalFont_),
  boldFont(boldFont_),
  italicFont(italicFont_),
  boldItalicFont(boldItalicFont_),
  page(page_),
  boldState(false), underlineState(false),
  row(0), column(0) {
  context->set_font_face(normalFont);
  Cairo::FontExtents fe;
  context->get_font_extents(fe);
  double left, top, right, bottom;
  context->get_clip_extents(left, top, right, bottom);
  rows = (bottom - top) / fe.height;
  columns = (right - left) / fe.max_x_advance;
}

CairoOutput::~CairoOutput() {
}

void CairoOutput::bold(bool state) { boldState = state; }
void CairoOutput::underline(bool state) { underlineState = state; }

void CairoOutput::text(const std::wstring &s) {
  context->set_font_face(normalFont);
  Cairo::FontExtents fe;
  context->get_font_extents(fe);
  double left, top, right, bottom;
  context->get_clip_extents(left, top, right, bottom);
  std::wstring::size_type n = 0;
  context->set_font_face(boldState ? underlineState ? boldItalicFont
                                                    : boldFont
                                   : underlineState ? italicFont
                                                      : normalFont);
  while(n < s.size()) {
    if(row == 0 && column == 0) {
      context->set_source_rgb(1.0, 1.0, 1.0); // TODO configurable
      context->paint();
      context->set_source_rgb(0.0, 0.0, 0.0); // TODO configurable
      // TODO page numbering
    }
    // Newlines
    if(s[n] == '\n') {
      ++row;
      column = 0;
      if(row == rows) {
        page(this);
        row = column = 0;
      }
      ++n;
      continue;
    }
    context->move_to(left + column * fe.max_x_advance,
                     top + row * fe.height + fe.ascent);
    std::string u = wideToUtf8(s.substr(n, 1));
    context->show_text(u);
    ++column;
    ++n;
  }
}

void CairoOutput::finished() { 
  if(row || column) {
    boldState = underlineState = false;
    row = column = 0;
    page(this);
  }
}

// TODO bold, underline
