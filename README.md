text-tools
==========

[![Build Status](https://travis-ci.org/ewxrjk/text-tools.svg?branch=master)](https://travis-ci.org/ewxrjk/text-tools)

This is a small collection of text processing tools, capable of
converting plain text to HTML, Postscript, PDF and PNG format.
The use of overprinting to represent bold and underline, for instance
as traditionally found in man(1) output, is recognized.

Installation
------------

    ./autogen.sh   # only if configure doesn't exist yet
    ./configure
    make
    sudo make install

The following commands will be installed:

    texttohtml
    texttopng
    texttops
    texttopdf

They all have man pages.

Copyright
---------

Copyright © 2011, 2014, 2015 Richard Kettlewell

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

