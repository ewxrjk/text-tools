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
#include "CairoOutput.hh"
#include "Utils.hh"
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <getopt.h>
#include <pangomm/init.h>

static const struct option options[] = {
  { "tab", required_argument, NULL, 't' },
  { "width", required_argument, NULL, 'w' },
  { "height", required_argument, NULL, 'H' },
  { "font-size", required_argument, NULL, 'f' },
  { "font", required_argument, NULL, 'F' },
  { "output", required_argument, NULL, 'o' },
  { "list-fonts", no_argument, NULL, 'l' },
  { "border", required_argument, NULL, 'b' },
  { "page-numbering", no_argument, NULL, 'p' },
  { "title", required_argument, NULL, 'T' },
  { "italic", no_argument, NULL, 'i' },
  { "help", no_argument, NULL, 'h' },
  { "version", no_argument, NULL, 'V' },
  { NULL, 0, NULL, 0 },
};

static void help() {
  printf("Usage:\n"
         "  texttopng [OPTIONS] [--] [INPUT...]\n"
         "  texttopng --list-fonts\n"
         "\n"
         "Text options:\n"
         "  -t, --tab COLUMNS           Set tab size (default=8)\n"
         "  -f, --font-size SIZE        Set font size\n"
         "  -F, --font FONT             Set font (default=Courier New)\n"
         "  -i, --italic                Render underlined text in italic\n"
         "Layout options:\n"
         "  -w, --width PIXELS          Set output file width\n"
         "  -H, --height PIXELS         Set output file height\n"
         "  -b, --border PIXELS         Set border\n"
         "  -T, --title TITLE           Top-of-page title string\n"
         "  -p, --page-numbering        Enable page numbering\n"
         "Other options:\n"
         "  -l, --list-fonts            List fonts\n"
         "  -o, --output BASE           Set output base (default=output)\n"
         "  -h, --help                  Display usage message\n"
         "  -V, --version               Display version string\n");
}

static void version() {
  printf("%s\n", VERSION);
}

static Cairo::RefPtr<Cairo::Surface> surface;
static int count;
static const char *output = "output";

static void newpage(CairoOutput *) {
  char *buffer;
  if(asprintf(&buffer, "%s%d.png", output, count++) < 0)
    throw std::runtime_error(std::string("asprintf: ") + strerror(errno));
  surface->write_to_png(buffer);
  free(buffer);
}

int main(int argc, char **argv) {
  try {
    int tabstop = 8;
    int width = -1, height = -1;
    double fontsize = 10.0;
    double border = 0;
    const char *font = "Courier New";
    const char *title = NULL;
    bool pageNumbering = false;
    bool italic = false;
    if(!setlocale(LC_CTYPE, ""))
      throw std::runtime_error(std::string("setlocale: ")
                               + strerror(errno));
    Pango::init();
    int opt;
    while((opt = getopt_long(argc, argv, "+hVt:w:H:f:F:o:lb:pT:i", options, NULL)) >= 0) {
      switch(opt) {
      case 't': tabstop = stringToInt(optarg); break;
      case 'w': width = stringToInt(optarg); break;
      case 'H': height = stringToInt(optarg); break;
      case 'f': fontsize = stringToDouble(optarg); break;
      case 'F': font = optarg; break;
      case 'o': output = optarg; break;
      case 'l': CairoOutput::listFonts(); return 0;
      case 'b': border = stringToDouble(optarg); break;
      case 'p': pageNumbering = true; break;
      case 'T': title = optarg; break;
      case 'i': italic = true; break;
      case 'h': help(); return 0;
      case 'V': version(); return 0;
      default: return 1;
      }
    }
    if(tabstop <= 0)
      throw std::runtime_error("invalid top stop size");
    if(width == 0 || height == 0)
      throw std::runtime_error("invalid page dimensions");
    if(fontsize <= 0)
      throw std::runtime_error("invalid font size");
    if(border < 0)
      throw std::runtime_error("invalid border size");
    Pango::FontDescription fontdesc;
    fontdesc.set_family(font);
    fontdesc.set_size(PANGO_SCALE * fontsize);
    if(width < 0 || height < 0) {
      double mw, mh;
      CairoOutput::getEmSize(Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32,
                                                         1, 1),
                             fontdesc, mw, mh);
      if(width < 0) width = 80 * mw + 2 * border;
      if(height < 0) height = 66 * mh + 2 * border;
    }
    surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 
                                          width, height);
    Cairo::RefPtr<Cairo::Context> context = Cairo::Context::create(surface);
    CairoOutput o(context, fontdesc, newpage);
    o.setPageNumbering(pageNumbering);
    if(title) o.setTitle(title);
    o.setBorder(border);
    o.setItalic(italic);
    Textfile t;
    t.setTabStop(tabstop);
    if(optind >= argc) {
      t.open("-");
      t.process(o);
    } else {
      while(optind < argc) {
        t.open(argv[optind++]);
        t.process(o);
      }
    }
    o.finished();
    return 0;
  } catch(std::runtime_error &e) {
    fprintf(stderr, "ERROR: %s\n", e.what());
    return 1;
  }
}
