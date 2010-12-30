## Process this file with automake to produce Makefile.in
INCLUDES = -DPACKAGE_LOCALEDIR=\""$(datadir)/locale/"\" \
            -DG_LOG_DOMAIN=\"Gtranscoder\" \
			-DDATADIR=\"$(datadir)\" \
			-DLIBDIR=\"$(libdir)\" \
			-DSYSCONFDIR=\"$(sysconfdir)\" \
			-DPREFIX=\"$(prefix)\" \
            -I$(top_srcdir) \
            $(GTK_CFLAGS)

LIBS = $(GTK_LIBS)

bin_PROGRAMS = gtranscoder
gtranscoder_SOURCES = \
    main.c \
    application.c \
    application.h \
	gtranscoder-error.c \
	gtranscoder-error.h \
    transcoding.c \
    transcoding.h

