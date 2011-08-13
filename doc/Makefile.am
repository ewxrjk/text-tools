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

dist_man_MANS=texttohtml.1 texttopng.1
man_MANS=texttops.1 texttopdf.1
EXTRA_DIST=texttops.1.in

all: texttops.1 texttopdf.1

texttops.1: texttops.1.in
	sed < $^ > $@ \
		-e 's/%ps%/ps/g;' \
		-e 's/%postscript%/Postscript/g'

texttopdf.1: texttops.1.in
	sed < $^ > $@ \
		-e 's/%ps%/pdf/g;' \
		-e 's/%postscript%/PDF/g'

clean-local:
	rm -f texttops.1 texttopdf.1
