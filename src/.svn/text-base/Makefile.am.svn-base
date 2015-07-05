SUBDIRS = 

DEFS = 	-DPLUGINS_DIR=\"$(plugins_dir)\" 	\
	-DDATA_DIR=\"$(datadir)\" 		\
	-I$(top_srcdir)/include 		\
	-I$(srcdir)/ -I$(TOP_DIR) 		\
	-I$(SRC_DIR)/include			\
	$(GTK2_CFLAGS) $(GLIB2_CFLAGS) $(CFLAGS)\
	$(LIBGLADE_CFLAGS) $(GTHREADS2_CFLAGS)	\
	$(LIBDUPTOOLS_CFLAGS)

AM_CPPFLAGS = -DPROGRAM_LOCALE_DIR=\"${PROGRAM_LOCALE_DIR}\"

bin_PROGRAMS = gstressfs
gstressfs_LDFLAGS = -export-dynamic -no-undefined 
gstressfs_LDADD = $(GLIB2_LIBS) $(GTK2_LIBS) $(LIBGLADE_LIBS)      \
	       	   $(GTHREADS2_LIBS) $(LIBDUPTOOLS_LIBS)

gstressfs_SOURCES = 			\
		gstressfs.c		\
		file_creation.c		\
		include/gstressfs.h	\
		include/file_creation.h

gladefiles = gstressfs.glade
gladedir = $(datadir)/@PACKAGE@
glade_DATA = $(gladefiles)
EXTRA_DIST = $(gladefiles)