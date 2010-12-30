ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src pixmaps po
EXTRA_DIST = config.rpath NEWS gtranscoder.desktop.in \
  intltool-extract.in \
  intltool-merge.in \
  intltool-update.in

desktopdir = $(datadir)/applications
desktop_in_files = gtranscoder.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

DISTCLEANFILES = intltool-extract \
                 intltool-merge \
                 intltool-update

clean-local:
	rm -f gtranscoder.desktop
	rm -f po/.intltool-merge-cache

