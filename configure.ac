#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(GTRANSCODER, 0.1.1, alexandre@quessy.net)
AC_DEFINE([PACKAGE_URL],["http://www.toonloop.com"])
AC_CONFIG_HEADER([src/config.h])
AC_PROG_LIBTOOL
AM_INIT_AUTOMAKE()

# Check for C compiler
AC_PROG_CC

# GNU help2man creates man pages from --help output; in many cases, this
# is sufficient, and obviates the need to maintain man pages separately.
# However, this means invoking executables, which we generally cannot do
# when cross-compiling, so we test to avoid that (the variable
# "cross_compiling" is set by AC_PROG_CC).
if test $cross_compiling = no; then
    AM_MISSING_PROG(HELP2MAN, help2man)
else
    HELP2MAN=:
fi

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h stdlib.h string.h sys/ioctl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
PKG_PROG_PKG_CONFIG

AC_PROG_GCC_TRADITIONAL
AC_CHECK_FUNCS([inet_ntoa memset socket])

PKG_CHECK_MODULES([GTK], [gtk+-2.0 >= 2.12.9 glib-2.0 >= 2.16.6 gdk-2.0])
AC_SUBST([GTK_LIBS])
AC_SUBST([GTK_CFLAGS])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
    ])
AC_OUTPUT

