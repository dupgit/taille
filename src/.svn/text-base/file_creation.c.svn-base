/* -*- indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */
/*
   file_creation.c
   Stresses the filesystem by creating a huge number of files in a
   single directory

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

/**
 *  file_creation test is not intended to measure the amount of time
 *  that a file creation takes.  It is indeed intended to crash file
 *  filesystem response time by creating a very huge number of files
 *  in a single directory  and thus view how many file creations are 
 *  needed to achieve this
 */

#include "gstressfs.h"

static gboolean create_one_empty_file(test_t *the_test, guint number, guint nthread);
static gboolean create_empty_files(test_t *the_test, guint number, guint nthread);
static file_creation_t *new_file_creation_t(guint howmany, guint64 buf_size, guint64 file_size);

/**
 *  Creates one empty file
 */
static gboolean create_one_empty_file(test_t *the_test, guint number, guint nthread)
{
	FILE *fp = NULL;
	gchar *filename = NULL;
	gchar *time_str = NULL;
	pid_t the_pid;
	GTimeVal time;

	the_pid = getpid();
	g_get_current_time(&time);
	time_str = g_strdup_printf("fc%d_%04d_%05d_%ld_%ld", the_pid, nthread, number, time.tv_sec, time.tv_usec);
	filename = g_build_filename(the_test->exec_dirname, time_str, NULL);
	g_free(time_str);

	fp = g_fopen(filename, "wb");

	if (fp != NULL)
		{ 
			fclose(fp);
			g_free(filename);
			the_test->percent = 1.0;
			return TRUE;
		}
	else
		{
			g_free(filename);
			the_test->percent = 0.0;
			return FALSE;
		}
}


/**
 *  Writes buffer to the file fp until the file's size
 *  reaches file_size
 */
static gboolean write_in_the_file(FILE *fp, gchar *buffer, guint64 buf_size, guint64 file_size)
{
	size_t nelem = 0;
	gboolean ok = TRUE;
   
	while (ok == TRUE && file_size > 0)
		{
			nelem = fwrite(buffer, 1, buf_size, fp);
			
			if (nelem != buf_size)
				{
					ok = FALSE;
				}

			file_size -= buf_size;

			if (file_size < buf_size)
				{
					buf_size = file_size;
				}
		}

	return ok;
}


/**
 *  Creates one file filled with zeros
 *  TODO : update percent while filling the file
 */
static gboolean create_one_zero_filled_file(test_t *the_test, guint number, guint nthread)
{
	FILE *fp = NULL;
	gchar *filename = NULL;
	gchar *time_str = NULL;
	pid_t the_pid;
	GTimeVal time;
	gchar *buffer = NULL;
	file_creation_t *intern_struct = NULL;  /* internal structure for the test */
	gboolean ok = FALSE;

	intern_struct = (file_creation_t *) the_test->data;

	buffer = intern_struct->buffer;

	the_pid = getpid();
	g_get_current_time(&time);
	time_str = g_strdup_printf("fc%d_%04d_%05d_%ld_%ld", the_pid, nthread, number, time.tv_sec, time.tv_usec);
	filename = g_build_filename(the_test->exec_dirname, time_str, NULL);
	g_free(time_str);

	fp = g_fopen(filename, "wb");

	if (fp != NULL)
		{ 
			ok = write_in_the_file(fp, buffer, intern_struct->buf_size, intern_struct->file_size);
			fclose(fp);

			g_free(filename);
		
			the_test->percent = 1.0; /* This may not be true */
		   
			return ok;
		}
	else
		{
			g_free(filename);
		
			the_test->percent = 0.0;

			return FALSE;
		}
}


/**
 *  Creates many empty files
 */
static gboolean create_empty_files(test_t *the_test, guint number, guint nthread)
{
	guint i = 0;
	gboolean ok = TRUE;
	file_creation_t *file_creation = NULL;

	file_creation = (file_creation_t *) the_test->data;

	i = 0;
	while (ok == TRUE && i<file_creation->howmany)
		{
			ok = ok && create_one_empty_file(the_test, i, nthread);
			i++;
			if (ok == TRUE)
				{
					the_test->percent = (gfloat) (i) / (gfloat) (file_creation->howmany);
				}
			else
				{
					i--;
					the_test->percent = (gfloat) (i) / (gfloat) (file_creation->howmany);
				}
		}

	return ok;
}


/**
 * Creates a new file_creation structure that will be 
 * integrated within the test_t structure
 */
static file_creation_t *new_file_creation_t(guint howmany, guint64 buf_size, guint64 file_size)
{
	file_creation_t *file_creation = NULL;

	file_creation = (file_creation_t *) g_malloc0(sizeof(file_creation_t));

	file_creation->howmany = howmany;
	file_creation->buf_size = buf_size;
	file_creation->file_size = file_size;

	file_creation->buffer = (gchar *) g_malloc0(sizeof(gchar)*file_creation->buf_size);


	return file_creation;
}

/**
 *  Function that increase the wanted file_size by a factor of 2
 *  each time it is called
 *  In those vary functions YOU MUST fill in the conditional dirname
 *  that will be used to create a corresponding directory 
 *  (the_test->cond_dirname) and you must also indicate tests conditions
 *  in a human understandable sentence in the_test->test_conditions
 */
static gboolean vary_file_size(test_t *the_test, gboolean init)
{
	file_creation_t *file_creation = (file_creation_t *) the_test->data;
	
	if (init == TRUE)
		{
			file_creation->file_size = DEFAULT_FILE_SIZE;
			if (the_test->cond_dirname != NULL)
				{
					g_free(the_test->cond_dirname);
				}
			the_test->cond_dirname = g_strdup_printf("%Ld_bytes", file_creation->file_size);

			if (the_test->test_conditions != NULL)
				{
					g_free(the_test->test_conditions);
				}
			the_test->test_conditions = g_strdup_printf("Creating files with %Ld bytes size", file_creation->file_size);
 
			return TRUE;
		}
	else
		{
			if (file_creation->file_size <= MAX_FILE_SIZE)
				{
					file_creation->file_size = 2 * file_creation->file_size;
					if (the_test->cond_dirname != NULL)
						{
							g_free(the_test->cond_dirname);
						}
					the_test->cond_dirname = g_strdup_printf("%Ld_bytes", file_creation->file_size);

					if (the_test->test_conditions != NULL)
						{
							g_free(the_test->test_conditions);
						}
					the_test->test_conditions = g_strdup_printf("Creating files with %Ld bytes size", file_creation->file_size);
					return TRUE;
				}
			else
				{
					return FALSE;
				}
		}
}


/**
 *  Initialisation function
 *  TODO : a de_init function that may be called 
 *  at the end of the program to free some memory !
 */
GList *init_test_creation_file(GList *test_list)
{
  test_t *the_test = NULL;
  file_creation_t *file_creation = NULL;
  GList *vary_func_list = NULL;

  /* test data internal initialisation */
  file_creation = new_file_creation_t(FILES_HOW_MANY, DEFAULT_BUF_SIZE, DEFAULT_FILE_SIZE);

  /* test initialisation -- May be all guint values should be user definable in a tab preference ? */
  the_test = new_test_t_struct(_("Files"), _("Empty File Creation"), "file_creation_atomic", NULL, (TestExecFunc) create_one_empty_file, NULL, NULL, NULL, (gpointer) file_creation, MAX_TIMES_TO_RUN, FC_MAX_NB_THREADS);
   test_list = add_test_to_test_list(test_list, the_test);

  /* You can add as many test as you want, but with different names and parameters */
   the_test = new_test_t_struct(_("Files"), _("Empty Files Creation (not really atomic)"), "file_creation_not_atomic", NULL, (TestExecFunc) create_empty_files, NULL, NULL, NULL, (gpointer) file_creation, MAX_TIMES_TO_RUN, FC_MAX_NB_THREADS);
  test_list = add_test_to_test_list(test_list, the_test);
   
  /**
   * You can use a special function that will be called after each execution and that may change
   * parameters. This function will be called until it returns FALSE. You must add this function
   * to a list (see above) thus you may add more than one function.
   */
  vary_func_list = add_vary_function_to_list(vary_func_list, (TestVaryFunc) vary_file_size);
  the_test = new_test_t_struct(_("Files"), _("Zero Filled File Creation"), "zero_filled_file_creation", NULL, (TestExecFunc) create_one_zero_filled_file, NULL, vary_func_list, NULL, (gpointer) file_creation, MAX_TIMES_TO_RUN, FC_MAX_NB_THREADS);
  test_list = add_test_to_test_list(test_list, the_test);
  

  return test_list;
}
