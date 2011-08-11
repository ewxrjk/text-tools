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

static const struct option options[] = {
  { "tab", required_argument, NULL, 't' },
  { "width", required_argument, NULL, 'w' },
  { "height", required_argument, NULL, 'H' },
  { "font-size", required_argument, NULL, 'f' },
  { "font", required_argument, NULL, 'F' },
  { "output", required_argument, NULL, 'o' },
  { "help", no_argument, NULL, 'h' },
  { "version", no_argument, NULL, 'V' },
  { NULL, 0, NULL, 0 },
};

static void help() {
  printf("Usage:\n"
         "  texttopng [OPTIONS] [--] [INPUT...]\n"
         "\n"
         "Options:\n"
         "  -t, --tab COLUMNS           Set tab size (default=8)\n"
         "  -w, --width PIXELS          Set output file width (default=480)\n"
         "  -H, --height PIXELS         Set output file height (default=792)\n"
         "  -f, --font-size SIZE        Set font size (default=10)\n"
         "  -F, --font FONT             Set font (default=monospace)\n"
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
    int width = 6 * 80, height = 12 * 66;
    double fontsize = 10.0;
    const char *font = "monospace";
    if(!setlocale(LC_CTYPE, ""))
      throw std::runtime_error(std::string("setlocale: ")
                               + strerror(errno));
    int opt;
    while((opt = getopt_long(argc, argv, "+hVt:w:H:f:F:o:", options, NULL)) >= 0) {
      switch(opt) {
      case 't': tabstop = atoi(optarg); break;
      case 'w': width = atoi(optarg); break;
      case 'H': height = atoi(optarg); break;
      case 'f': fontsize = atof(optarg); break;
      case 'F': font = optarg; break;
      case 'o': output = optarg; break;
      case 'h': help(); return 0;
      case 'V': version(); return 0;
      default: return 1;
      }
    }
    // TODO arg sanity checking
    surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 
                                          width, height);
    Cairo::RefPtr<Cairo::Context> context = Cairo::Context::create(surface);
    context->set_font_size(fontsize);
    context->select_font_face(font,
                              Cairo::FONT_SLANT_NORMAL,
                              Cairo::FONT_WEIGHT_NORMAL);
    Cairo::RefPtr<Cairo::FontFace> normalFont = context->get_font_face();
    context->select_font_face(font,
                              Cairo::FONT_SLANT_NORMAL,
                              Cairo::FONT_WEIGHT_BOLD);
    Cairo::RefPtr<Cairo::FontFace> boldFont = context->get_font_face();
    context->select_font_face(font,
                              Cairo::FONT_SLANT_ITALIC,
                              Cairo::FONT_WEIGHT_NORMAL);
    Cairo::RefPtr<Cairo::FontFace> italicFont = context->get_font_face();
    context->select_font_face(font,
                              Cairo::FONT_SLANT_ITALIC,
                              Cairo::FONT_WEIGHT_BOLD);
    Cairo::RefPtr<Cairo::FontFace> boldItalicFont = context->get_font_face();
    CairoOutput o(context,
                  normalFont, boldFont, italicFont, boldItalicFont,
                  newpage);
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
