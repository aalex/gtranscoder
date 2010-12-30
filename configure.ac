#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(GTRANSCODER, 0.1.1, alexandre@quessy.net)
AC_DEFINE([PACKAGE_URL],["http://www.toonloop.com"])
AC_CONFIG_HEADER([src/config.h])
AC_CONFIG_MACRO_DIR([m4]) # check for m4 macros in directory m4
AM_INIT_AUTOMAKE()

# Check for C compiler
AC_PROG_CC
AC_HEADER_STDC

AC_PROG_LIBTOOL
PKG_PROG_PKG_CONFIG

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

# take care of localization
AH_TEMPLATE([GETTEXT_PACKAGE], [Package name for gettext])
GETTEXT_PACKAGE=gtranscoder	# note that this is a command
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE")
AC_SUBST(GETTEXT_PACKAGE)
ALL_LINGUAS="" # fr de es
AM_GLIB_GNU_GETTEXT
AC_PROG_INTLTOOL

# Checks for header files.
# AC_CHECK_HEADERS([arpa/inet.h stdlib.h string.h sys/ioctl.h unistd.h])
# 
# # Checks for typedefs, structures, and compiler characteristics.
# AC_HEADER_STDBOOL
# AC_C_CONST
# AC_PROG_GCC_TRADITIONAL
# AC_CHECK_FUNCS([inet_ntoa memset socket])

# export compiler/linker options
AC_SUBST(CFLAGS)
AC_SUBST(CFLAGS, " -O2 -Wall -Wno-unused -Werror -Wextra --std=gnu99 -Wfatal-errors") 
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

# look for GTK
# PKG_CHECK_MODULES([GTK], [gtk+-2.0 >= 2.12.9 glib-2.0 >= 2.16.6 gdk-2.0])
# AC_SUBST([GTK_LIBS])
# AC_SUBST([GTK_CFLAGS])

# look for GNOME
GNOME_MODULES="libgnomeui-2.0 >= 2.0.0"
PKG_CHECK_MODULES(GNOME, $GNOME_MODULES)
AC_SUBST(GNOME_CFLAGS)
AC_SUBST(GNOME_LIBS)


AC_CONFIG_FILES([
    Makefile
    src/Makefile
    pixmaps/Makefile
    po/Makefile.in
    ])
AC_OUTPUT

