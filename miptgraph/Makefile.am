#
# /*
#  *  This program is free software: you can redistribute it and/or modify
#  *  it under the terms of the GNU General Public License as published by
#  *  the Free Software Foundation, either version 3 of the License, or
#  *  (at your option) any later version.
#  *
#  *  This program is distributed in the hope that it will be useful,
#  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  *  GNU General Public License for more details.
#  *
#  *  You should have received a copy of the GNU General Public License
#  *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#  *
#  * SPDX-License-Identifier: GPL-3.0+
#  * License-Filename: LICENSE
#  *
#  */
#

#
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

#
AM_MAKEFLAGS = --no-print-directory

#
AUTOMAKE_OPTIONS = foreign subdir-objects

# dirs with makefile.am
SUBDIRS = . src/Graph src/Layout

# indent sources
indent:
	cd src/Graph && make indent
	cd src/Layout && make indent
	cd src/test1 && make indent

#
cleaner:
	cd src && make cleaner
	cd src/Graph && make cleaner
	cd src/Layout && make cleaner
	cd src/test1 && make cleaner
	rm -v -f -r -d html
	rm -v -f -r -d m4
	rm -v -f -r -d autom4te.cache
	rm -v -f aclocal.m4
	rm -v -f ar-lib
	rm -v -f compile
	rm -v -f config.guess
	rm -v -f config.h.in
	rm -v -f config.h.in~
	rm -v -f config.log
	rm -v -f config.status
	rm -v -f config.sub
	rm -v -f configure
	rm -v -f depcomp
	rm -v -f dsmake.*
	rm -v -f install-sh
	rm -v -f libtool
	rm -v -f ltmain.sh
	rm -v -f Makefile
	rm -v -f Makefile.in
	rm -v -f missing
	rm -v -f stamp-h1
	rm -v -f O1
	rm -v -f O2

# end.
