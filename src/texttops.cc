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
#include "Optional.hh"
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
  { "list-fonts", no_argument, NULL, 'l' },
  { "border", required_argument, NULL, 'b' },
  { "page-numbering", no_argument, NULL, 'p' },
  { "title", required_argument, NULL, 'T' },
  { "italic", no_argument, NULL, 'i' },
  { "type", required_argument, NULL, 'y' },
  { "output", required_argument, NULL, 'o' },
  { "help", no_argument, NULL, 'h' },
  { "version", no_argument, NULL, 'V' },
  { NULL, 0, NULL, 0 },
};

static void help() {
  printf("Usage:\n"
         "  texttops [OPTIONS] [--] [INPUT...]\n"
         "  texttops --list-fonts\n"
         "\n"
         "Text options:\n"
         "  -t, --tab COLUMNS           Set tab size\n"
         "  -f, --font-size SIZE        Set font size\n"
         "  -F, --font FONT             Set font\n"
         "  -i, --italic                Render underlined text in italic\n"
         "Layout options:\n"
         "  -w, --width POINTS          Set output file width\n"
         "  -H, --height POINTS         Set output file height\n"
         "  -b, --border POINTS         Set border\n"
         "  -T, --title TITLE           Top-of-page title string\n"
         "  -p, --page-numbering        Enable page numbering\n"
         "     Title and page numbering require a nontrivial border\n"
         "Output options:\n"
         "  -y, --type TYPE             Set output type (pdf, ps, png)\n"
         "  -o, --output OUTPUT         Output filename\n"
         "     PNG output goes to OUTPUT0.png, OUTPUT1.png, ...; default=output\n"
         "     PS/PDF goes to stdout by default.\n"
         "Other options:\n"
         "  -l, --list-fonts            List available fonts\n"
         "  -h, --help                  Display usage message\n"
         "  -V, --version               Display version string\n");
}

static void version() {
  printf("%s\n", VERSION);
}

static Cairo::RefPtr<Cairo::Surface> surface;
static Cairo::RefPtr<Cairo::Context> context;
static int count;
static const char *output;

static void newpage_ps(CairoOutput *) {
  context->show_page();
}

static void newpage_png(CairoOutput *) {
  char *buffer;
  if(asprintf(&buffer, "%s%d.png", output, count++) < 0)
    throw std::runtime_error(std::string("asprintf: ") + strerror(errno));
  surface->write_to_png(buffer);
  free(buffer);
}

Cairo::ErrorStatus writer(const unsigned char *data, unsigned int length) {
  fwrite(data, 1, length, stdout);
  if(ferror(stdout))
    throw std::runtime_error(std::string("writing to stdout: ")
                             + strerror(errno));
  return CAIRO_STATUS_SUCCESS;
}

int main(int argc, char **argv) {
  try {
    int tabstop = 8;
    Optional<double> width, height;
    Optional<double> fontsize;
    Optional<double> border;
    std::string type;
    if(strstr(argv[0], "pdf")) type = "pdf";
    else if(strstr(argv[0], "png")) type = "png";
    else type = "ps";
    const char *font = "Courier New";
    const char *title = NULL;
    bool pageNumbering = false;
    bool italic = false;
    if(!setlocale(LC_CTYPE, ""))
      throw std::runtime_error(std::string("setlocale: ")
                               + strerror(errno));
    Pango::init();
    int opt;
    while((opt = getopt_long(argc, argv, "+hVt:w:H:f:F:T:lb:py:io:", options, NULL)) >= 0) {
      switch(opt) {
      case 'y': type = optarg; break;
      case 't': tabstop = stringToInt(optarg); break;
      case 'w': width = stringToDouble(optarg); break; // TODO units
      case 'H': height = stringToDouble(optarg); break; // TODO units
      case 'f': fontsize = stringToDouble(optarg); break;
      case 'F': font = optarg; break;
      case 'l': CairoOutput::listFonts(); return 0;
      case 'b': border = stringToDouble(optarg); break;
      case 'p': pageNumbering = true; break;
      case 'T': title = optarg; break;
      case 'i': italic = true; break;
      case 'o': output = optarg; break;
      case 'h': help(); return 0;
      case 'V': version(); return 0;
      default: return 1;
      }
    }
    if(tabstop <= 0)
      throw std::runtime_error("invalid top stop size");
    if(!fontsize.exists()) fontsize = 8.0;
    if(type == "png") {
      if(!output) output = "output";
      if(!border.exists()) border = 0;
    } else {
      if(!border.exists()) border = 36.0;
    }
    if(border <= 0.0)
      throw std::runtime_error("invalid border size");
    if(fontsize <= 0.0)
      throw std::runtime_error("invalid font size");
    Pango::FontDescription fontdesc;
    fontdesc.set_family(font);
    fontdesc.set_size(PANGO_SCALE * fontsize);
    if(!width.exists() || !height.exists()) {
      if(type == "png") {
        double mw, mh;
        CairoOutput::getEmSize(Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32,
                                                           1, 1),
                               fontdesc, mw, mh);
        if(!width.exists()) width = 80 * mw + 2 * border;
        if(!height.exists()) height = 66 * mh + 2 * border;
      } else {
        if(!width.exists()) width = 595;
        if(!height.exists()) height = 841;
      }
    }
    if(width <= 0 || height <= 0)
      throw std::runtime_error("invalid page dimensions");
    void (*newpage)(CairoOutput *);
    if(type == "png") {
      newpage = newpage_png;
    } else {
      newpage = newpage_ps;
      if(output)
        if(!freopen(output, "wb", stdout))
          throw std::runtime_error(std::string("opening ") + output + ": " + strerror(errno));
    }
    if(type == "pdf")
      surface = Cairo::PdfSurface::create_for_stream(sigc::ptr_fun(writer), width, height);
    else if(type == "ps")
      surface = Cairo::PsSurface::create_for_stream(sigc::ptr_fun(writer), width, height);
    else if(type == "png")
      surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32,
                                            width, height);
    else
      throw std::runtime_error("unrecognized output type");
    context = Cairo::Context::create(surface);
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
    surface->finish();
    if(fclose(stdout) < 0)
      throw std::runtime_error(std::string("closing stdout: ")
                               + strerror(errno));
    return 0;
  } catch(std::runtime_error &e) {
    fprintf(stderr, "ERROR: %s\n", e.what());
    return 1;
  }
}
