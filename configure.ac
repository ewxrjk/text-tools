#
# Copyright (C) 2011, 2014, 2015, 2017 Richard Kettlewell
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

AC_PREREQ([2.61])
AC_INIT([rjk-text-tools],[0.1],[rjk@terraraq.org.uk])
AC_CONFIG_AUX_DIR([config.aux])
AM_INIT_AUTOMAKE([foreign])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])
AC_CONFIG_SRCDIR([src/Textfile.hh])
AC_CONFIG_HEADER([config.h])
AC_PROG_CXX
AC_LANG([C++])
AC_CHECK_HEADERS([langinfo.h])
AC_CHECK_FUNCS([nl_langinfo])
AC_CACHE_CHECK([for Pango/Cairomm CFLAGS],[rjk_cv_cairomm_cflags],[
  rjk_cv_cairomm_cflags=`pkg-config --silence-errors --cflags cairomm-1.0 pangomm-1.4`
])
AC_CACHE_CHECK([for Pango/Cairomm LIBS],[rjk_cv_cairomm_libs],[
  rjk_cv_cairomm_libs=`pkg-config --silence-errors --libs cairomm-1.0 pangomm-1.4`
])
if test -z "$rjk_cv_cairomm_libs"; then
 AC_MSG_ERROR([could not find Cairo])
fi
AC_SUBST([CAIROMM_CFLAGS],[$rjk_cv_cairomm_cflags])
AC_SUBST([CAIROMM_LIBS],[$rjk_cv_cairomm_libs])
if test "x$GXX" = xyes; then
  CXXFLAGS="$CXXFLAGS -Wall -W -Werror -Wpointer-arith -Wwrite-strings"
  CXXFLAGS="$CXXFLAGS -std=c++11"
  cxxcoverage="-fprofile-arcs -ftest-coverage"
fi
AC_ARG_ENABLE([coverage],[AS_HELP_STRING([--enable-coverage],
                                         [compile with coverage information])])
if test "$enable_coverage" = yes; then
  CXXFLAGS="${CXXFLAGS} ${cxxcoverage}"
  LIBS="${LIBS} -lgcov"
fi
CXXFLAGS="${CXXFLAGS} ${CXXFLAGS_EXTRA}"
AC_OUTPUT([Makefile src/Makefile doc/Makefile])
