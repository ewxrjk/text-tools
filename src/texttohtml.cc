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
#include "HtmlOutput.hh"
#include "Utils.hh"
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <getopt.h>

static const struct option options[] = {
  { "title", required_argument, NULL, 'T' },
  { "tab", required_argument, NULL, 't' },
  { "italic", no_argument, NULL, 'i' },
  { "help", no_argument, NULL, 'h' },
  { "version", no_argument, NULL, 'V' },
  { NULL, 0, NULL, 0 },
};

static void help() {
  printf("Usage:\n"
         "  texttohtml [OPTIONS] [--] [INPUT...]\n"
         "\n"
         "Options:\n"
         "  -t, --tab COLUMNS           Set tab size (default=8)\n"
         "  -T, --title TITLE           HTML <title> element\n"
         "  -i, --italic                Render underlined text in italic\n"
         "  -h, --help                  Display usage message\n"
         "  -V, --version               Display version string\n");
}

static void version() {
  printf("%s\n", VERSION);
}

int main(int argc, char **argv) {
  try {
    const char *title = NULL;
    int tabstop = 8;
    bool italic = false;
    if(!setlocale(LC_CTYPE, ""))
      throw std::runtime_error(std::string("setlocale: ")
                               + strerror(errno));
    int opt;
    while((opt = getopt_long(argc, argv, "+hVT:t:i", options, NULL)) >= 0) {
      switch(opt) {
      case 'T': title = optarg; break;
      case 't': tabstop = stringToInt(optarg); break;
      case 'i': italic = true; break;
      case 'h': help(); return 0;
      case 'V': version(); return 0;
      default: return 1;
      }
    }
    if(tabstop <= 0)
      throw std::runtime_error("invalid top stop size");
    HtmlOutput h;
    Textfile t;
    t.setTabStop(tabstop);
    h.setItalic(italic);
    h.output.open("-", "w");
    h.output.printf("<html>\n"
                    "<head>\n");
    if(title) {
      h.output.printf("<title>");
      h.text(narrowToWide(title));
      h.output.printf("</title>\n");
    }
    h.output.printf("</head>\n"
                    "<body>\n"
                    "<pre>");
    if(optind >= argc) {
      t.open("-");
      t.process(h);
    } else {
      while(optind < argc) {
        t.open(argv[optind++]);
        t.process(h);
      }
    }
    h.finished();
    h.output.printf("</pre>\n"
                    "</body>\n"
                    "</html>\n");
    if(fclose(stdout) < 0)
      throw std::runtime_error(std::string("closing stdout: ")
                               + strerror(errno));
    return 0;
  } catch(std::runtime_error &e) {
    fprintf(stderr, "ERROR: %s\n", e.what());
    return 1;
  }
}
