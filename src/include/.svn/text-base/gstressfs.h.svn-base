/* -*- indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */
/*
   digere.h
   Projet digestion

   (C) Copyright 2008 Olivier Delhomme
   e-mail : olivierdelhomme@gmail.com
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.           
*/
#ifndef _GSTRESSFS_H_
#define _GSTRESSFS_H_

#include <glade/glade.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <glib/gstdio.h>
#include <glib/gi18n.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>
#include <dup/duptools.h>

#include "config.h"


#define ProgName "GStressFS"
#define ProgVersion "0.0.1"
#define ProgDate "06.02.2008"
#define ProgAuthor "Olivier Delhomme <olivierdelhomme@gmail.com>"


#define INIT_NB_TEST 2   /* Thoses two values should be defined by the test itself */
#define NB_MULTIPLIER 2  /* and eventualy by the user in a specific preference tab */
#define INIT_NB_THREADS 2


/**
 *  Main structure
 */
typedef struct
{
	GladeXML *xml;        /* Glade XML interface definition    */
	GList *location_list; /* Localisation list                 */
	log_t *log;           /* Log system                        */
	gchar *dirname;       /* Where all the tests might be done */
	GList *test_list;     /* lists of tests                    */
} main_struct_t;


/**
 *  A structure to keep track of the execution times
 */
typedef struct
{
	guint number;          /* number of execution of the atomic test                                   */
	guint nthreads;        /* number of threads at the same time (total execution = nthreads * number) */
	gdouble time;          /* total time it took (from the program's point of view)                    */
	struct rusage *rusage; /* usage statistics from getrusage                                          */
} exec_time_t;

typedef gboolean (* TestExecFunc) (gpointer, guint, guint); /* Test execution function type          */
typedef void (* TestInitFunc) (gpointer);                   /* Test initialisation function type     */
typedef gboolean (* TestVaryFunc) (gpointer, gboolean);     /* Test varying function definition type */
/* the vary function will be called until it returns FALSE */
/* TRUE is passed when an initialisation is requiered (begin and end of the tests) */

/**
 *  Test structure
 */
typedef struct
{
	gchar *groupname;       /* test group name                                                     */
	gchar *name;            /* test name                                                           */
	gchar *dirname;         /* base directory name (should not change after init)                  */
	gchar *exec_dirname;    /* directory name where the test should executes itself                */
	gchar *cond_dirname;    /* Must be updated by the varying function upon test conditions        */
	gchar *test_conditions; /* Must be updated by the varying func to indicate the test conditions */
	TestExecFunc exec;      /* execution function for the specified test                           */
	TestInitFunc init;      /* init function for the specified test                                */
	GList *vary_func_list;  /* list of functions to be used to vary some parameters                */
	GList *exec_time_list;  /* list of execution times                                             */
	gpointer data;          /* internal data of the test                                           */
	guint max_nb;           /* max number of atomic test per threads                               */        
	guint max_threads;      /* max number of threads                                               */
	gfloat percent;         /* values from 0.0 to 1.0 represents, in percent,                      */
						    /* the state of the current running test                               */
	                        /* this value has to be set by the test itself                         */
} test_t;


/**
 * Structure for threads
 */
typedef struct
{
	test_t *one_test;             /* test_t structure        */
	main_struct_t *main_struct;   /* main_struct_t structure */
	guint number;                 /* number times it runs    */
	guint nthread;                /* number of thread        */
} threads_t;


/**
 *  Includes
 */
#include "file_creation.h"


/**
 *  Functions
 */
extern test_t *new_test_t_struct(gchar *groupname, gchar *name, gchar *dirname, gchar *exec_dirname, TestExecFunc exec, TestInitFunc init, GList *vary_func_list, GList *exec_time_list, gpointer data, guint max_nb, guint max_threads);
extern void free_test_t_struct(test_t *one_test);
extern GList *add_test_to_test_list(GList *test_list, test_t *one_test);
extern GList *add_vary_function_to_list(GList *vary_func_list, TestVaryFunc func);

#endif /* _GSTRESSFS_H_ */
