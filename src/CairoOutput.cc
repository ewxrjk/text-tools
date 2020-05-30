#include <config.h>
#include "CairoOutput.hh"
#include "Utils.hh"
#include "Characters.hh"
#include <pangomm/cairofontmap.h>

CairoOutput::CairoOutput(Cairo::RefPtr<Cairo::Context> context_,
                         const Pango::FontDescription &font_,
                         void (*page_)(CairoOutput *)):
    context(context_),
    layout(Pango::Layout::create(context)), font(font_), page(page_), border(0),
    pageNumbering(false), underlineAsItalic(false), boldState(false),
    underlineState(false), x(0), y(0), pageNumber(0) {
  double left, top, right, bottom;
  context->get_clip_extents(left, top, right, bottom);
  width = right - left;
  height = bottom - top;
}

CairoOutput::~CairoOutput() {}

void CairoOutput::newPage() {
  x = y = 0;
  context->set_source_rgb(1.0, 1.0, 1.0); // TODO configurable
  context->paint();
  context->set_source_rgb(0.0, 0.0, 0.0); // TODO configurable
  ++pageNumber;
  if(title.size()) {
    Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(context);
    layout->set_font_description(font);
    layout->set_text(title);
    const Pango::Rectangle extent = layout->get_pixel_logical_extents();
    double left, top, right, bottom;
    context->get_clip_extents(left, top, right, bottom);
    context->move_to((left + right) / 2 - extent.get_width() / 2,
                     top + border / 2 - extent.get_height() / 2);
    layout->show_in_cairo_context(context);
  }
  if(pageNumbering) {
    char buffer[20];
    snprintf(buffer, sizeof buffer, "Page %d", pageNumber);
    Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(context);
    layout->set_font_description(font);
    layout->set_text(buffer);
    const Pango::Rectangle extent = layout->get_pixel_logical_extents();
    double left, top, right, bottom;
    context->get_clip_extents(left, top, right, bottom);
    context->move_to((left + right) / 2 - extent.get_width() / 2,
                     bottom - border / 2 - extent.get_height() / 2);
    layout->show_in_cairo_context(context);
  }
}

void CairoOutput::bold(bool state) {
  boldState = state;
}
void CairoOutput::underline(bool state) {
  underlineState = state;
}

void CairoOutput::text(const std::wstring &s) {
  std::wstring::size_type n = 0;
  while(n < s.size()) {
    if(s[n] != LINE_FEED) {
      if(boldState)
        line += L"<b>";
      if(underlineState)
        line += underlineAsItalic ? L"<i>" : L"<u>";
      switch(s[n]) {
      default: line += s[n]; break;
      case L'<':
      case L'&': {
        wchar_t buffer[16];
        swprintf(buffer, sizeof buffer / sizeof *buffer, L"&#%u;", s[n]);
        line += buffer;
        break;
      }
      }
      if(underlineState)
        line += underlineAsItalic ? L"</i>" : L"</u>";
      if(boldState)
        line += L"</b>";
    } else
      renderLine();
    ++n;
    // TODO bold, italic
  }
}

void CairoOutput::renderLine() {
  if(!x && !y)
    newPage();
  // Create a Pango layout for the line
  Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(context);
  layout->set_font_description(font);
  layout->set_markup(wideToUtf8(line));
  const Pango::Rectangle extent = layout->get_pixel_logical_extents();
  // If the line overflows the layout, push out a page go onto the next
  if(y + extent.get_height() > height - 2 * border) {
    page(this);
    newPage();
  }
  // Render the line
  context->move_to(x + border, y + border);
  layout->show_in_cairo_context(context);
  y += extent.get_height();
  line.clear();
}

void CairoOutput::finished() {
  if(line.size())
    renderLine();
  if(x || y) {
    page(this);
    boldState = underlineState = false;
    pageNumber = 0;
  }
}

void CairoOutput::listFonts() {
  std::vector<Glib::RefPtr<Pango::FontFamily>> families =
      Glib::wrap(pango_cairo_font_map_get_default())->list_families();
  for(size_t n = 0; n < families.size(); ++n)
    if(families[n]->is_monospace())
      printf("%s\n", families[n]->get_name().c_str());
}

void CairoOutput::getEmSize(Cairo::RefPtr<Cairo::Surface> surface,
                            const Pango::FontDescription &fontdesc, double &w,
                            double &h) {
  Cairo::RefPtr<Cairo::Context> context = Cairo::Context::create(surface);
  Glib::RefPtr<Pango::Layout> layout = Pango::Layout::create(context);
  layout->set_font_description(fontdesc);
  layout->set_text("M");
  const Pango::Rectangle extent = layout->get_pixel_logical_extents();
  w = extent.get_width();
  h = extent.get_height();
}
