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
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# Macro: AC_INIT (package, version, [bug-report], [tarname], [url])
# minor version number is increased at important feature change
# Process with autogen.sh or manual:
# aclocal
# autoheader
# autoconf
# libtoolize --force --automake
# automake -a --gnu --include-deps
# or do this all using autoreconf -fvim
#
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([miptgraph], [2.0], [mooigraph@gmail.com], [miptgraph.tar.gz], [https://notabug.org/mooigraph/miptgraph])
AC_CONFIG_SRCDIR([src/Layout/LEdge.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

AM_INIT_AUTOMAKE([-Wall])

AM_SILENT_RULES([no])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# add here compile flags
#CFLAGS="$CFLAGS -fplugin=$HOME/scallgraph_plugin.so.0.0.0"
CFLAGS="$CFLAGS -Wall -Wextra"

AC_SUBST(CFLAGS)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AM_PROG_AR

AC_PROG_LIBTOOL
AM_PROG_LIBTOOL
LT_INIT

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# compilation date in config.h
CONFIG_DATE=`date +"%a %x %Y"`

AC_SUBST(CONFIG_DATE)

AC_DEFINE_UNQUOTED(COMPILE_DATE, ["$CONFIG_DATE"], [Year, month and day this program is compiled.])

# short system name in config.h
CONFIG_UNAME=`uname -s`

AC_SUBST(CONFIG_UNAME)

AC_DEFINE_UNQUOTED(COMPILE_UNAME, ["$CONFIG_UNAME"], [System where this is compiled.])



AC_COPYRIGHT([
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
])


AC_CONFIG_FILES([Makefile src/Makefile src/Layout/Makefile src/test1/Makefile])

AC_OUTPUT

AC_MSG_NOTICE([

Configuration:
  Source code location	${srcdir}
  Build code location	${builddir}
  Destination prefix	${prefix}
  CXX-Compiler		${CXX}
  Config warning flags	"${WARNING_CFLAGS}"
  Config CFLAGS		"${CFLAGS}"
  Config LIBS		"${LIBS}"
  Config PACKAGE_CFLAGS	"$PACKAGE_CFLAGS"
])

