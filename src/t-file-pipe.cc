//
// Copyright (C) 2014 Richard Kettlewell
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
#include <cassert>
#include <csignal>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>

int main(void) {
  int p[2];
  if(pipe(p) < 0) {
    perror("pipe");
    return 1;
  }
  if(dup2(p[0], 0) < 0 || dup2(p[1], 1) < 0) {
    perror("dup2");
    return 1;
  }
  if(close(p[0]) < 0 || close(p[1]) < 0) {
    perror("close");
    return 1;
  }
  {
    int ch;
    File in("-", "r");
    File out("-", "w");

    out.put('!');
    out.flush();
    ch = in.get();
    assert(ch == '!');
  }
  struct stat s;
  assert(fstat(0, &s) == 0);
  assert(fstat(1, &s) == 0);
  if(signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
    perror("signal");
    return 1;
  }
  File out("-", "w");
  if(close(0) < 0) {
    perror("close");
    return 1;
  }
  try {
    out.put('x');
    out.flush();
    assert(!"unexpectedly succeeded");
  } catch(std::runtime_error &e) {
    assert(e.what() == "flushing stdout: " + std::string(strsignal(SIGPIPE)));
  }

  return 0;
}
