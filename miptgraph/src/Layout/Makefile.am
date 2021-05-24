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

noinst_LTLIBRARIES = libmiptgraphlayout.la

# List all source files for this library, including headers
libmiptgraphlayout_la_SOURCES = \
	LEdge.cpp \
	LGraph.cpp \
	LNode.cpp \
	Ordering.cpp \
	LEdge.h \
	LGraph.h \
	LNode.h \
	Ordering.h \
	Layout.h

# This is for libtool
AM_LDFLAGS = $(LT_LDFLAGS)

# Here list all include flags, relative to this "srcdir" directory.
AM_CPPFLAGS = $(CFLAGS)

# Here list all the header files that are required by a user of the library,
includemgdir = $(includedir)/miptgraph
includemg_HEADERS = \
	LEdge.h \
	LGraph.h \
	LNode.h \
	Ordering.h \
	Layout.h

indent:
	clang-format -style=WebKit -i *.cpp

cleaner:
	rm -v -f -r -d .deps
	rm -v -f -r -d .libs
	rm -v -f *.o
	rm -v -f *.lo
	rm -v -f *.la
	rm -v -f Makefile
	rm -v -f Makefile.in
	rm -v -f *.scallgraph.gv
