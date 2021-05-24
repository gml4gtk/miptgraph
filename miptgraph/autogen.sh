#!/bin/sh
# (or use sh -x to see what is going on)
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
#  *
#  */
#
# sparse llvm c compiler CC=sparsec
# llvm-clang compiler CC=clang
#
# If environment variable DRYRUN is set, no configuring will be done -
# (e.g. in bash)  DRYRUN=1 ./autogen.sh
# will not do any configuring but will emit the programs that would be run.
#
# This is basically:
# aclocal -I m4
# autoheader
# autoconf
# automake --copy --add-missing --force-missing --include-deps --foreign
#
# or: autoreconf -fvim
#
# libtool version 2.2, 2.4 tested http://ftp.gnu.org/gnu/libtool

#
rm -f -r -d m4
rm -f -r -d autom4te.cache
rm -f ./configure
rm -f ./Makefile
rm -f ./Makefile.in
mkdir m4
aclocal
autoheader
libtoolize --force --copy
automake --add-missing --copy --foreign
autoconf
./configure
make clean

# end.
