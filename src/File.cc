//
// Copyright (C) 2011, 2015 Richard Kettlewell
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
#include "File.hh"
#include <cerrno>
#include <cstring>
#include <cstdarg>

File::~File() {
  if(fp && fp != stdin && fp != stdout && fp != stderr)
    fclose(fp);
}

void File::open(const std::string &path_, const std::string &mode_) {
  if(fp)
    throw std::logic_error("File::open but already open");
  if(path_ == "-") {
    if(mode_.at(0) == 'r') {
      fp = stdin;
      path = "stdin";
    } else {
      fp = stdout;
      path = "stdout";
    }
  } else {
    fp = fopen(path_.c_str(), mode_.c_str());
    if(!fp)
      throw std::runtime_error("opening " + path_ + ": " + strerror(errno));
    if(fwide(fp, 0) != 0)
      throw std::runtime_error("your C library is broken");
    path = path_;
  }
}

int File::get() {
  if(!fp)
    throw std::logic_error("File::get when closed");
  int c = getc(fp);
  if(c == EOF && ferror(fp))
    throw std::runtime_error("reading " + path + ": " + strerror(errno));
  return c;
}

wint_t File::getw() {
  if(!fp)
    throw std::logic_error("File::getw when closed");
  wint_t c = getwc(fp);
  if(c == WEOF && ferror(fp))
    throw std::runtime_error("reading " + path + ": " + strerror(errno));
  return c;
}

void File::close() {
  if(!fp)
    throw std::logic_error("File::close when closed");
  FILE *fp_ = fp;
  fp = NULL;
  if(fclose(fp_) < 0)
    throw std::runtime_error("closing " + path + ": " + strerror(errno));
}

int File::printf(const char *fmt, ...) {
  if(!fp)
    throw std::logic_error("File::printf when closed");
  va_list ap;
  va_start(ap, fmt);
  int rc = vfprintf(fp, fmt, ap);
  va_end(ap);
  if(rc < 0)
    throw std::runtime_error("writing " + path + ": " + strerror(errno));
  return rc;
}

void File::put(int c) {
  if(!fp)
    throw std::logic_error("File::put when closed");
  if(putc(c, fp) < 0)
    throw std::runtime_error("writing " + path + ": " + strerror(errno));
}

void File::putw(int wc) {
  if(!fp)
    throw std::logic_error("File::putw when closed");
  if(putwc(wc, fp) == WEOF)
    throw std::runtime_error("writing " + path + ": " + strerror(errno));
}

void File::flush() {
  if(!fp)
    throw std::logic_error("File::flush when closed");
  if(fflush(fp) < 0)
    throw std::runtime_error("flushing " + path + ": " + strerror(errno));
}
