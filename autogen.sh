#!/bin/sh
# could be replaced with autoreconf -fivI m4 (verbose, force rebuild of ltmain, .in files, etc.)
libtoolize --force
aclocal 
autoheader
automake -a -f --add-missing
autoconf -f
#./configure $@
