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
  { "list-fonts", no_argument, NULL, 'l' },
  { "border", required_argument, NULL, 'b' },
  { "page-numbering", no_argument, NULL, 'p' },
  { "title", required_argument, NULL, 'e' },
  { "help", no_argument, NULL, 'h' },
  { "version", no_argument, NULL, 'V' },
  { NULL, 0, NULL, 0 },
};

static void help() {
  printf("Usage:\n"
         "  texttops [OPTIONS] [--] [INPUT...]\n"
         "\n"
         "Options:\n"
         "  -T, --type TYPE             Set output type (pdf or ps)\n"
         "  -t, --tab COLUMNS           Set tab size (default=8)\n"
         "  -w, --width POINTS          Set output file width (default=595)\n"
         "  -H, --height POINTS         Set output file height (default=841)\n"
         "  -f, --font-size SIZE        Set font size (default=8)\n"
         "  -F, --font FONT             Set font (default=Courier New)\n"
         "  -b, --border POINTS         Set border (default=36)\n"
         "  -p, --page-numbering        Enable page numbering\n"
         "  -e, --title TITLE           Add title string\n"
         "  -l, --list-fonts            List fonts\n"
         "  -h, --help                  Display usage message\n"
         "  -V, --version               Display version string\n");
}

static void version() {
  printf("%s\n", VERSION);
}

static Cairo::RefPtr<Cairo::Surface> surface;
static Cairo::RefPtr<Cairo::Context> context;

static void newpage(CairoOutput *) {
  context->show_page();
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
    double width = 595, height = 841;   // A4
    double fontsize = 8.0;
    double border = 36.0;
    const char *type;
    if(strstr(argv[0], "pdf")) type = "pdf";
    else type = "ps";
    const char *font = "Courier New";
    const char *title = NULL;
    bool pageNumbering = false;
    if(!setlocale(LC_CTYPE, ""))
      throw std::runtime_error(std::string("setlocale: ")
                               + strerror(errno));
    Pango::init();
    int opt;
    while((opt = getopt_long(argc, argv, "+hVt:w:H:f:F:T:lb:pe:", options, NULL)) >= 0) {
      switch(opt) {
      case 'T': type = optarg; break;
      case 't': tabstop = atoi(optarg); break;
      case 'w': width = atof(optarg); break; // TODO units
      case 'H': height = atof(optarg); break; // TODO units
      case 'f': fontsize = atof(optarg); break;
      case 'F': font = optarg; break;
      case 'l': CairoOutput::listFonts(); return 0;
      case 'b': border = atof(optarg); break;
      case 'p': pageNumbering = true; break;
      case 'e': title = optarg; break;
      case 'h': help(); return 0;
      case 'V': version(); return 0;
      default: return 1;
      }
    }
    // TODO arg sanity checking
    if(!strcmp(type, "pdf"))
      surface = Cairo::PdfSurface::create_for_stream(sigc::ptr_fun(writer), width, height);
    else
      surface = Cairo::PsSurface::create_for_stream(sigc::ptr_fun(writer), width, height);
    Pango::FontDescription fontdesc;
    fontdesc.set_family(font);
    fontdesc.set_size(PANGO_SCALE * fontsize);
    context = Cairo::Context::create(surface);
    CairoOutput o(context, fontdesc, newpage);
    o.setPageNumbering(pageNumbering);
    if(title) o.setTitle(title);
    o.setBorder(border);
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
