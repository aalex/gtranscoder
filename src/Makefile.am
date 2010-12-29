## Process this file with automake to produce Makefile.in
INCLUDES = -DGNOMELOCALEDIR=\""$(datadir)/locale/"\" \
           -DG_LOG_DOMAIN=\"Gtranscoder\" \
           -I$(top_srcdir) \
           $(GNOME_CFLAGS)

LIBS = $(GNOME_LIBS)

bin_PROGRAMS = gtranscoder
gtranscoder_SOURCES = main.c

