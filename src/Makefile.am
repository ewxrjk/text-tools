#
# Copyright (C) 2011, 2014 Richard Kettlewell
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

bin_PROGRAMS=texttohtml texttops
noinst_PROGRAMS=t-n2w t-s2d t-s2i t-w2u t-file t-textfile
TESTS=t-n2w t-s2d t-s2i t-w2u t-file t-textfile

texttohtml_SOURCES=File.cc HtmlOutput.cc narrowToWide.cc Textfile.cc	\
TextfileProcessor.cc texttohtml.cc Characters.hh File.hh HtmlOutput.hh	\
Textfile.hh Utils.hh stringToInt.cc Optional.hh
texttops_SOURCES=File.cc CairoOutput.cc Textfile.cc			\
TextfileProcessor.cc texttops.cc Characters.hh File.hh CairoOutput.hh	\
Textfile.hh Utils.hh wideToUtf8.cc stringToInt.cc stringToDouble.cc	\
Optional.hh
texttops_LDADD=${CAIROMM_LIBS}

t_n2w_SOURCES=narrowToWide.cc t-n2w.cc
t_s2d_SOURCES=stringToDouble.cc t-s2d.cc
t_s2i_SOURCES=stringToInt.cc t-s2i.cc
t_w2u_SOURCES=wideToUtf8.cc t-w2u.cc
t_file_SOURCES=File.cc t-file.cc
t_textfile_SOURCES=Textfile.cc TextfileProcessor.cc File.cc t-textfile.cc

install-exec-hook:
	rm -f $(DESTDIR)$(bindir)/texttopdf$(EXEEXT)
	ln -s texttops$(EXEEXT) \
		$(DESTDIR)$(bindir)/texttopdf$(EXEEXT)
	rm -f $(DESTDIR)$(bindir)/texttopng$(EXEEXT)
	ln -s texttops$(EXEEXT) \
		$(DESTDIR)$(bindir)/texttopng$(EXEEXT)

uninstall-local:
	rm -f $(DESTDIR)$(bindir)/texttopdf$(EXEEXT)
	rm -f $(DESTDIR)$(bindir)/texttopng$(EXEEXT)

clean-local:
	rm -f *.gcno
	rm -f *.gcda
	rm -f *.gcov
