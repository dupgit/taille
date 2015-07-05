SUBDIRS = 

DEFS = 	-DPLUGINS_DIR=\"$(plugins_dir)\" 	\
	-DDATA_DIR=\"$(datadir)\" 		\
	-I$(top_srcdir)/include 		\
	-I$(srcdir)/ -I$(TOP_DIR) 		\
	-I$(SRC_DIR)/include			\
	$(GLIB2_CFLAGS) $(CFLAGS)

AM_CPPFLAGS = 

bin_PROGRAMS = taille

taille_LDFLAGS = $(LDFLAGS) -export-dynamic -no-undefined 
taille_LDADD = $(GLIB2_LIBS) 

taille_SOURCES = 			\
		taille.c			\
		include/taille.h	
