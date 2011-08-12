#
# Copyright (C) 2011 Richard Kettlewell
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

AM_CXXFLAGS=${CAIROMM_CFLAGS}

bin_PROGRAMS=texttohtml texttopng texttops
texttohtml_SOURCES=File.cc HtmlOutput.cc narrowToWide.cc Textfile.cc	\
TextfileProcessor.cc texttohtml.cc Characters.hh File.hh HtmlOutput.hh	\
Textfile.hh Utils.hh
texttopng_SOURCES=File.cc CairoOutput.cc Textfile.cc			\
TextfileProcessor.cc texttopng.cc Characters.hh File.hh CairoOutput.hh	\
Textfile.hh Utils.hh wideToUtf8.cc
texttopng_LDFLAGS=${CAIROMM_LIBS}
texttops_SOURCES=File.cc CairoOutput.cc Textfile.cc			\
TextfileProcessor.cc texttops.cc Characters.hh File.hh CairoOutput.hh	\
Textfile.hh Utils.hh wideToUtf8.cc
texttops_LDFLAGS=${CAIROMM_LIBS}
