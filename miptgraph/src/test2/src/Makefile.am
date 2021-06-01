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

# the C++ mipt graph lib
miptlib1 = ../../Layout/.libs/libmiptgraphlayout.a
miptlib2 = ../../Graph/.libs/libmiptgraphgraph.a

#
bin_PROGRAMS = miptgml

#
miptgml_SOURCES = \
	main.c \
	mem.c \
	uniqstring.c \
	uniqnode.c \
	gml.c \
	splay-tree.c

#
miptgml_H = \
	main.h \
	mem.h \
	uniqstring.h \
	uniqnode.h \
	gml.h \
	splay-tree.h

#
miptgml_CFLAGS = \
	@CFLAGS@ @PACKAGE_CFLAGS@ @WARNING_CFLAGS@ @GTK_CFLAGS@

#
miptgml_tolink = @GTK_LIBS@

#
miptgml$(EXEEXT): $(miptgml_SOURCES)
	rm -v -f ./*.o
	rm -v -f ./miptgml
	$(CC) -c $(miptgml_CFLAGS) $(miptgml_SOURCES) -I. -I.. -I../..
	$(CXX) *.o $(miptgml_tolink) $(miptlib1) $(miptlib2) -o miptgml

#
indent:
	./lindent.sh $(miptgml_SOURCES) $(miptgml_H)

# peg parser
parser:
	rm -v -f ./packcc
	rm -v -f ./gml.c
	rm -v -f ./gml.h
	$(CC) -std=gnu89 -Wall -Wextra -Wno-unused-parameter -Wno-overlength-strings -pedantic -O2 -DNDEBUG packcc.c -o packcc
	./packcc gml.peg

#
clean:
	rm -v -f *.o
	rm -v -f miptgml
	rm -v -f *~
	rm -v -f ./packcc

#
cleaner:
	make clean-generic

#
distclean:
	make clean-generic

#
clean-generic:
	rm -v -f *.bc
	rm -v -f ./massif.out.*
	rm -v -f ./a.out
	rm -v -f *~
	rm -v -f ./*.so.1
	rm -v -f ./*.o
	rm -v -f ./*.a
	rm -v -f ./*.i
	rm -v -f ./*.s
	rm -v -f ./O
	rm -v -f ./OO
	rm -v -f ./O1
	rm -v -f ./O2
	rm -v -f ./O3
	rm -v -f ./ID
	rm -v -f ./dsmake.output
	rm -v -f ./dsmake.warnings
	rm -v -f ./dsmake.errors
	rm -v -f *.dot
	rm -v -f *.*r.*
	rm -v -f *.*t.*
	rm -v -f *.bc
	rm -v -f *.plist
	rm -v -f *.ps
	rm -v -f ./*.exe
	rm -v -f jsonlint
	rm -v -f ./*.gk*
	rm -v -f ./*.out
	rm -v -f ./*.res
	rm -v -f -r -d .deps
	rm -v -f -r -d .libs
	rm -v -f *.o
	rm -v -f *.lo
	rm -v -f *.la
	rm -v -f Makefile
	rm -v -f Makefile.in
	rm -v -f miptgml
	rm -v -f ./packcc

# /* end */
