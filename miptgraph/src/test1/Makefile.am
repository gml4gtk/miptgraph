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

# make test programs depending on lib
all: libs
	make test1
	make test2
	make test3
	make test4

# generate output files
output:
	./test1 >test1-output.txt
	./test2 >test2-output.txt
	./test3 >test3-output.txt
	./test4 >test4-output.txt

# generate svg image data
test1:
	$(CXX) test1.cpp ../Layout/.libs/libmiptgraphlayout.a ../Graph/.libs/libmiptgraphgraph.a -o test1

# run two graph layout at once
test2:
	$(CXX) test2.cpp ../Layout/.libs/libmiptgraphlayout.a ../Graph/.libs/libmiptgraphgraph.a -o test2

# graph with a cycle
test3:
	$(CXX) test3.cpp ../Layout/.libs/libmiptgraphlayout.a ../Graph/.libs/libmiptgraphgraph.a -o test3

# graph with no edges
test4:
	$(CXX) test4.cpp ../Layout/.libs/libmiptgraphlayout.a ../Graph/.libs/libmiptgraphgraph.a -o test4

libs: graphlib layoutlib

graphlib:
	cd ../Graph && make

layoutlib:
	cd ../Layout && make

indent:
	clang-format -style=WebKit -i *.cpp *.h

clean:
	rm -v -f test1
	rm -v -f test2
	rm -v -f test3
	rm -v -f test4
	rm -v -f *.output

cleaner:
	make clean
	rm -v -f -r -d .deps
	rm -v -f -r -d .libs
	rm -v -f *.o
	rm -v -f *.lo
	rm -v -f *.la
	rm -v -f Makefile
	rm -v -f Makefile.in

# end.
