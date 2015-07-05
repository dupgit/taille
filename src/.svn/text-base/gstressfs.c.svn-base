/* -*- indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */
/*
   gstressfs.c
   Stress your file system !

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

#include "gstressfs.h"

static void log_all_reconnized_test(main_struct_t *main_struct);
static GList *init_test_list(main_struct_t *main_struct);
static gchar *select_a_folder(GtkWidget *parent);

static void remove_test_directory(main_struct_t *main_struct, test_t *one_test);
static gboolean build_and_create_directory_name(main_struct_t *main_struct, test_t *one_test, gchar *main_dirname, guint ntimes, guint nthread);

static gboolean run_one_test(main_struct_t *main_struct, test_t *one_test, guint number, guint nthread);
static gboolean run_one_test_multiple_times(main_struct_t *main_struct, test_t *one_test, guint number);

static threads_t *new_thread_t(main_struct_t *main_struct, test_t *one_test, guint number, guint nthread);
static void free_thread_t(threads_t *thread);

static void threaded_run(gpointer user_data);
static GList *spawn_all_threads(GList *thread_list, main_struct_t *main_struct, test_t *one_test, guint number, guint nthread);
static gboolean wait_for_all_threads(GList *thread_list);
static gboolean run_one_test_threaded_increasing(main_struct_t *main_struct, test_t *one_test);

static test_t *find_the_test_that_called_me(main_struct_t *main_struct, GtkWidget *widget);

static void menu_run_one_test(GtkWidget *widget, gpointer data);
static void menu_run_many_tests(GtkWidget *widget, gpointer data);
static void menu_run_threaded_tests(GtkWidget *widget, gpointer data);
static void menu_quit(GtkWidget *widget, gpointer data);
static void menu_run_all(GtkWidget *widget, gpointer data);
static void menu_open_folder(GtkWidget *widget, gpointer data);

static gboolean delete_main_window_event(GtkWidget *widget, GdkEvent  *event, gpointer data );
static void destroy_main_window(GtkWidget *widget, gpointer data);
static void connect_menu_signals(main_struct_t *main_struct);
static void connect_signals(main_struct_t *main_struct);
static gboolean load_xml_interface(main_struct_t *main_struct);
static void init_international_languages(void);
static main_struct_t *init_main_struct(void);
static void init_interface(main_struct_t *main_struct);
static void fin_du_programme(main_struct_t *main_struct);
static void init_log_window(main_struct_t *main_struct);

static exec_time_t *new_exec_time_t(guint number, guint nbthreads, gdouble time, struct rusage *rusage);

/**
 *  Logs all the available tests.
 */
static void log_all_reconnized_test(main_struct_t *main_struct)
{
	test_t *one_test = NULL;
	GList *test_list = main_struct->test_list;

	ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("List of tests :"));
	while (test_list != NULL)
		{
			one_test = (test_t *) test_list->data;
			ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, "\t%s (%s)", one_test->name, one_test->dirname);
			test_list = g_list_next(test_list);
		}
}


/**
 *  Populates a new test_t structure with the right values
 *  groupname : a group_name where tests might be grouped together
 *  name : the test name
 *  dirname : the subdirectory name where the tests migth be conducted
 *  exec : the exec function (might be NULL but the test will not run)
 *  init : the init function (might be NULL)
 *  exec_time_list : GList of exec_time_t that keep track of execution times
 *  data : the internal datas (if any - might be NULL) for the test itself
 */
test_t *new_test_t_struct(gchar *groupname, gchar *name, gchar *dirname, gchar *exec_dirname, TestExecFunc exec, TestInitFunc init, GList *vary_func_list, GList *exec_time_list, gpointer data, guint max_nb, guint max_threads)
{
	test_t *the_test = NULL;

	if (exec != NULL)
		{

			the_test = (test_t *) g_malloc0(sizeof(test_t));
	
			if (groupname == NULL)
				{
					the_test->groupname = g_strdup("Default Group");
				}
			else
				{
					the_test->groupname = g_strdup(groupname);
				}

			if (name == NULL)
				{
					the_test->name = g_strdup("Default Name");
				}
			else
				{
					the_test->name = g_strdup(name);
				}

			if (dirname == NULL)
				{
					the_test->dirname = g_strdup("dirname");
				}
			else
				{
					the_test->dirname = g_strdup(dirname);
				}

			if (exec_dirname == NULL)
				{
					the_test->exec_dirname = g_strdup("exec_dirname");
				}
			else
				{
					the_test->exec_dirname = g_strdup(exec_dirname);
				}

			the_test->exec = exec;
			the_test->init = init;
			the_test->vary_func_list = vary_func_list;
			the_test->exec_time_list = exec_time_list;
			the_test->data = data;
			the_test->max_nb = max_nb;
			the_test->max_threads = max_threads;
			the_test->percent = 0.0;  /* at init time we do think that nothing happened within the test itself */

			return the_test;
		}
	else
		{
			return NULL;
		}
}


/**
 *  Add a varying function to the varying function list
 */
extern GList *add_vary_function_to_list(GList *vary_func_list, TestVaryFunc func)
{

	return g_list_append(vary_func_list, (gpointer) func);
}


/**
 *  Free memory a bit
 */
void free_test_t_struct(test_t *one_test)
{

	if (one_test != NULL)
		{

			g_free(one_test->name);
			g_free(one_test->dirname);
			g_free(one_test);
		}
}


/**
 *  Adds a test to the test list
 */
GList *add_test_to_test_list(GList *test_list, test_t *one_test)
{
	return g_list_append(test_list, (gpointer) one_test);
}


/**
 * inits the test list with all available tests
 */
static GList *init_test_list(main_struct_t *main_struct)
{
	GList *test_list = main_struct->test_list;

	test_list = init_test_creation_file(test_list);
	
	return test_list;
}


/**
 *  Runs one test 
 *  "number" represents the number of runtimes for the specified test 
 *  and should change at each call
 */
static gboolean run_one_test(main_struct_t *main_struct, test_t *one_test, guint number, guint nthread)
{
	if (one_test->exec != NULL)
		{
			return one_test->exec(one_test, number, nthread);
		}
	else
		{
			return FALSE;
		}
}


/**
 *  Function that will prompt the user to select a
 *  folder
 */
static gchar *select_a_folder(GtkWidget *parent)
{
  GtkFileChooser *fcd = NULL;
  gchar *filename = NULL; 

  /* Folder selection */
  fcd = GTK_FILE_CHOOSER(gtk_file_chooser_dialog_new(_("Open a folder..."),
			 GTK_WINDOW(parent),
			 GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, 
			 GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			 GTK_STOCK_OPEN, GTK_RESPONSE_OK, 
			 NULL));

  /* window properties : modal, without multiple selections */
  gtk_window_set_modal(GTK_WINDOW(fcd), TRUE);
  gtk_file_chooser_set_select_multiple(fcd, FALSE);
   
  switch(gtk_dialog_run(GTK_DIALOG(fcd)))
    {
    case GTK_RESPONSE_OK:
      /* Path retrieval */
      filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fcd));
      break;
    default:
      filename = NULL;
      break;
    }

  gtk_widget_destroy(GTK_WIDGET(fcd));

  return filename;
}



/** 
 *  Quit Menu
 */
static void menu_quit(GtkWidget *widget, gpointer data)
{
  main_struct_t *main_struct = (main_struct_t *) data;

  fin_du_programme(main_struct);		
}


/**
 *  builds a new absolute dirname from main_dirname and test_dirname
 */
static gboolean build_and_create_directory_name(main_struct_t *main_struct, test_t *one_test, gchar *main_dirname, guint ntimes, guint nthread)
{
	gchar *new_name = NULL;
	gchar *temp = NULL;
	gint result = 0;
	gboolean ok = TRUE;

	temp = g_strdup_printf("%03d-%06d", nthread, ntimes);

	if (one_test->cond_dirname != NULL)
		{
			new_name = g_build_path(G_DIR_SEPARATOR_S, main_dirname, one_test->dirname, temp, one_test->cond_dirname, NULL);
		}
	else
		{
			new_name = g_build_path(G_DIR_SEPARATOR_S, main_dirname, one_test->dirname, temp, NULL);
		}

	g_free(temp);

	result = g_mkdir_with_parents(new_name, S_IRWXU | S_IRWXG);

	if (result != 0)
		{
			ldt_log_message(main_struct->log, G_LOG_LEVEL_WARNING, _("Error while creating %s directory"), new_name);
			ok = FALSE;
		}
	   
	g_free(one_test->exec_dirname);

	one_test->exec_dirname = new_name;

	return ok;
}


/**
 *  Removes the dirname created for the test
 */
static void remove_test_directory(main_struct_t *main_struct, test_t *one_test)
{ 
	/* this function (g_rmdir) only removes empty directories !! */
	/*
	gint result = 0;

	result = g_rmdir(one_test->exec_dirname); 
	
	if (result != 0)
		{
			log_message(main_struct->log, G_LOG_LEVEL_WARNING, _("Error while removing %s directory"), one_test->exec_dirname);
		}
	*/
}

/**
 *  Runs "one_test" test "number" times
 */
static gboolean run_one_test_multiple_times(main_struct_t *main_struct, test_t *one_test, guint number)
{
	guint i = 0 ;
	gboolean ok = TRUE;

	while (i<number && ok == TRUE)
		{
			ok = run_one_test(main_struct, one_test, i, 1);
			i++;
		}

	return ok;
}

/**
 *  Creates a new exec_time_t structure filled with the values
 *  provided
 */
static exec_time_t *new_exec_time_t(guint number, guint nbthreads, gdouble time, struct rusage *rusage)
{
	exec_time_t *exec_time = NULL;
	
	exec_time = (exec_time_t *) g_malloc0(sizeof(exec_time_t));

	exec_time->number = number;
	exec_time->nthreads = nbthreads;
	exec_time->time = time;
	exec_time->rusage = rusage;

	return exec_time;
}


/**
 *  Runs one test, multiple times, increasing the number of times at each
 *  running session
 */
static gboolean run_one_test_increasing(main_struct_t *main_struct, test_t *one_test)
{
	guint i = INIT_NB_TEST; /* Should we make this a test defined value ? */
	guint sum = 0;
	gdouble time = 0;
	gulong microseconds = 0;
	GTimer *timer = NULL;
	gboolean ok = TRUE;
	exec_time_t *exec_time = NULL;
	struct rusage *rusage = NULL;
 
	timer = g_timer_new();

	while (i < one_test->max_nb && ok == TRUE)
		{

			/* directory making */
			if (build_and_create_directory_name(main_struct, one_test, main_struct->dirname, i, 1) == TRUE)
				{

					/* The test is started (and will run i times) */
					g_timer_start(timer);
					ok = run_one_test_multiple_times(main_struct, one_test, i);
					g_timer_stop(timer);

					/* time is calculated and logged */
					time = g_timer_elapsed(timer, &microseconds);
					rusage = (struct rusage *) g_malloc0(sizeof(struct rusage));
					getrusage(RUSAGE_CHILDREN, rusage);
					exec_time = new_exec_time_t(i, 1, time, rusage);
					one_test->exec_time_list = g_list_append(one_test->exec_time_list, exec_time);
					ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("%s ran %d times and took %f seconds"), one_test->name, i, time);

					/* Directory cleaning */
					remove_test_directory(main_struct, one_test);

					i = i * NB_MULTIPLIER;
				}
			else
				{
					ok = FALSE;
				}
		}

	if (ok == FALSE)
		{
			ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("%s crashed at %f %% when running multiple times at %f %%!"), one_test->name, one_test->percent*100, (gfloat)sum / (gfloat) one_test->max_nb);
		}

	return ok;
}


/**
 *   Retrieves a new structure for thread  creation
 */
static threads_t *new_thread_t(main_struct_t *main_struct, test_t *one_test, guint number, guint nthread)
{
	threads_t *thread = NULL;

	thread = (threads_t *) g_malloc0(sizeof(threads_t));

	thread->main_struct = main_struct; /* To be used read only in the threads */
	/* as we might also write in one_test's test_t structure, we'll duplicate it */
	thread->one_test = new_test_t_struct(one_test->groupname, one_test->name, one_test->dirname, one_test->exec_dirname, one_test->exec, one_test->init, one_test->vary_func_list, one_test->exec_time_list, one_test->data, one_test->max_nb, one_test->max_threads);       
	thread->number = number;
	thread->nthread = nthread;
	
	return thread;
}


/**
 *  Frees the thread_t struct
 */
static void free_thread_t(threads_t *thread)
{
	free_test_t_struct(thread->one_test);
	g_free(thread);
}


/**
 *  Gfunc for the thread called by g_thread_create
 */
static void threaded_run(gpointer user_data)
{
	threads_t *thread = (threads_t *) user_data;
	main_struct_t *main_struct = thread->main_struct;
	test_t *one_test = thread->one_test;
	guint number = thread->number;
	guint nthread = thread->nthread;
	gboolean ok = FALSE;
	
	/* ok = run_one_test(main_struct, one_test, number, nthread); */
	ok = run_one_test_multiple_times(main_struct, one_test, number);

	if (ok == FALSE)
		{
			ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("%s crashed : thread n° %d at %f %% !"), one_test->name, nthread, one_test->percent*100);
		}

	free_thread_t(thread); /* we won't use that datas anymore */
}


/**
 *  spawn all threads
 */
static GList *spawn_all_threads(GList *thread_list, main_struct_t *main_struct, test_t *one_test, guint number, guint nthread)
{
	GThread *one_thread = NULL;
	GError **error = NULL;
	gboolean ok = TRUE;
	guint i = 0;
	threads_t *thread = NULL;
	
	while (ok == TRUE && i < nthread)
		{
			thread = new_thread_t(main_struct, one_test, number, i+1); 
			one_thread = g_thread_create((GThreadFunc) threaded_run, thread, TRUE, error);

			if (one_thread != NULL)
				{
					thread_list = g_list_append(thread_list, (gpointer) one_thread);
					i++;
				}
			else
				{
					ok = FALSE;
				}
		}

	return thread_list;
}

/**
 *  Waits for the threads to terminates
 */
static gboolean wait_for_all_threads(GList *thread_list)
{
	GThread *one_thread = NULL;
	GList *head = thread_list;

	while (thread_list != NULL)
		{
			one_thread = (GThread *) thread_list->data;
			g_thread_join(one_thread);
			thread_list = g_list_next(thread_list);
		}

	g_list_free(head);  

	return TRUE; /* TODO : we should use the return value of func to do something usefull here */
}


/**
 *  Runs one test in two or more threads
 */
static gboolean run_one_test_threaded_increasing(main_struct_t *main_struct, test_t *one_test)
{
	guint i = INIT_NB_THREADS; /* Should we make this a test defined value ? */
	gboolean ok = TRUE;
	GList *thread_list;
	gdouble time = 0;
	gulong microseconds = 0;
	GTimer *timer = NULL;
	exec_time_t *exec_time = NULL;
	struct rusage *rusage = NULL;

	timer = g_timer_new();

	while (i <= one_test->max_threads && ok == TRUE)
		{
			/* directory making */
		    if (build_and_create_directory_name(main_struct, one_test, main_struct->dirname, 1, i) == TRUE)
				{
					/* The test is started (and will run 1 time in i threads) */
					g_timer_start(timer); 	
					thread_list = spawn_all_threads(NULL, main_struct, one_test, 1, i);
					ok = wait_for_all_threads(thread_list);
					g_timer_stop(timer); 
			
					/* time is calculated and logged */
					time = g_timer_elapsed(timer, &microseconds);
					rusage = (struct rusage *) g_malloc0(sizeof(struct rusage));
					getrusage(RUSAGE_CHILDREN, rusage);
					exec_time = new_exec_time_t(1, i, time, rusage);
					one_test->exec_time_list = g_list_append(one_test->exec_time_list, exec_time);

					/* Directory cleaning */
					remove_test_directory(main_struct, one_test);

					ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("%s ran in %d threads and took %f seconds"), one_test->name, i, time);

					i++;
				}
			else
				{
					ok = FALSE;
				}
		}

	return ok;
}

/**
 *  Runs one test many times in two or more threads
 */
static gboolean run_one_test_many_threaded_increasing(main_struct_t *main_struct, test_t *one_test)
{
	guint i = INIT_NB_THREADS; /* Should we make this a test defined value ? */
	gboolean ok = TRUE;
	GList *thread_list;
	gdouble time = 0;
	gulong microseconds = 0;
	GTimer *timer = NULL;
	exec_time_t *exec_time = NULL;
	struct rusage *rusage = NULL;

	timer = g_timer_new();

	while (i <= one_test->max_threads && ok == TRUE)
		{
			/* directory making */
		    if (build_and_create_directory_name(main_struct, one_test, main_struct->dirname, 1, i) == TRUE)
				{
			
					/* The test is started (and will run one_test->max_nb times in i threads) */
					g_timer_start(timer); 	
					thread_list = spawn_all_threads(NULL, main_struct, one_test, one_test->max_nb, i);
					ok = wait_for_all_threads(thread_list);
					g_timer_stop(timer); 
			
					/* time is calculated and logged */
					time = g_timer_elapsed(timer, &microseconds);
					rusage = (struct rusage *) g_malloc0(sizeof(struct rusage));
					getrusage(RUSAGE_CHILDREN, rusage);
					exec_time = new_exec_time_t(1, i, time, rusage);
					one_test->exec_time_list = g_list_append(one_test->exec_time_list, exec_time);

					/* Directory cleaning */
					remove_test_directory(main_struct, one_test);

					ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("%s ran in %d threads and took %f seconds"), one_test->name, i, time);

					i++;
				}
			else
				{
					ok = FALSE;
				}
		}

	return ok;
}


/**
 *  Runs one test and call the appropriate vary functions
 */
static gboolean run_one_test_varying(main_struct_t *main_struct, test_t *one_test)
{

	GList *list = one_test->vary_func_list;
	TestVaryFunc vary_func;
	gboolean ok = TRUE;
	gboolean test_ok = TRUE;

	while (list != NULL && test_ok == TRUE)
		{
			vary_func = (TestVaryFunc) list->data;
			
			ok = vary_func(one_test, TRUE);
			
			while (ok == TRUE && test_ok == TRUE)
				{
					if (one_test->test_conditions != NULL)
						{
							ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, one_test->test_conditions, one_test->name);
						}
					else
						{
							ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Unknown test conditions"), one_test->name);
						}

					test_ok = run_one_test_many_threaded_increasing(main_struct, one_test);

					/*
					  ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Waiting 10s for the system to calm down !"), one_test->name);
					  g_usleep(10*G_USEC_PER_SEC);
					*/

					ok = vary_func(one_test, FALSE);
				
				}

			ok = vary_func(one_test, TRUE);
		}

	return test_ok;
}



/** 
 *  Menu Run All. Selects a folder where to run if none has
 *  been previously choosed
 */
static void menu_run_all(GtkWidget *widget, gpointer data)
{
  main_struct_t *main_struct = (main_struct_t *) data;
  GtkWidget *main_window = glade_xml_get_widget(main_struct->xml, "menu_window");
  GList *test_list = NULL;
  test_t *one_test = NULL;
  gboolean ok = TRUE;

  if (main_struct->dirname == NULL)
	  {
		  main_struct->dirname = select_a_folder(main_window);
	  }

  if (main_struct->dirname != NULL)
	  {
		  ldt_log_message(main_struct->log, G_LOG_LEVEL_DEBUG, "dirname : %s", main_struct->dirname);
		  test_list = main_struct->test_list;

		  while (test_list != NULL && ok == TRUE)
			  {
				  one_test = (test_t *) test_list->data;
				  if (one_test != NULL)
					  {
						  ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Now Running %s test (many, threaded and many threaded !)"), one_test->name);
						  ok = run_one_test_increasing(main_struct, one_test);
						  ok = ok && run_one_test_threaded_increasing(main_struct, one_test);
						  ok = ok && run_one_test_many_threaded_increasing(main_struct, one_test);

						  if (ok != TRUE)
							  { 
								  ldt_log_message(main_struct->log, G_LOG_LEVEL_WARNING, _("Something went wrong with test %s"), one_test->name);
							  }
						  else 
							  {
								  ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Done !"));
							  }
					  }
				  test_list = g_list_next(test_list);
			  }
	  }
}

/**
 *  Tries (and sometimes succeeds) to find the test_t
 *  structure that corresponds to the widget that was
 *  activated by a signal !!
 */
static test_t *find_the_test_that_called_me(main_struct_t *main_struct, GtkWidget *widget)
{
	const gchar *name = NULL;
	gboolean ok = TRUE;
	test_t *one_test = NULL;
	GList *test_list = main_struct->test_list;

	name = gtk_widget_get_name(widget); /* the tricky part ! It's bad, I know ... :)      */
                                        /* the widget name got it's test name at creation */
                                        /* We use this to find it in the test list !!!    */

	while (ok == TRUE && test_list != NULL)
		{
			one_test = (test_t *) test_list->data;
			
			if (g_ascii_strcasecmp(one_test->name, name) == 0)
				{
					ok = FALSE;
				}
			else
				{
					test_list = g_list_next(test_list);
				}
		}

	if (ok == FALSE)
		{
			return one_test;
		}
	else
		{
			return NULL;
		}
}



/**
 *  Function called by the submenus of a single test that runs
 *  the test only once
 */
static void menu_run_one_test(GtkWidget *widget, gpointer data)
{
	main_struct_t *main_struct = (main_struct_t *) data;
	GtkWidget *main_window = glade_xml_get_widget(main_struct->xml, "main_window");
	test_t *one_test = NULL;
	gboolean ok = FALSE;
	gdouble time = 0;
	gulong microseconds = 0;
	GTimer *timer = NULL;
	exec_time_t *exec_time = NULL;
	struct rusage *rusage = NULL;

	one_test = find_the_test_that_called_me(main_struct, widget);
	
	timer = g_timer_new();

	if (one_test != NULL)
		{
			ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Selected test : %s"), one_test->name);

			if (main_struct->dirname == NULL)
				{
					main_struct->dirname = select_a_folder(main_window);
				}

			if (main_struct->dirname != NULL)
				{ 
					/* directory making */
					if (build_and_create_directory_name(main_struct, one_test, main_struct->dirname, 1, 1) == TRUE)
						{
							ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Now Running %s test (%s)"), one_test->name, one_test->exec_dirname);

							/* The test is started (and will run 1 time) */
							g_timer_start(timer);
							ok = run_one_test(main_struct, one_test, 1, 1);
							g_timer_stop(timer); 
					
							/* time is calculated and logged */
							time = g_timer_elapsed(timer, &microseconds);
							rusage = (struct rusage *) g_malloc0(sizeof(struct rusage));
							getrusage(RUSAGE_CHILDREN, rusage);
							exec_time = new_exec_time_t(1, 1, time, rusage);
							one_test->exec_time_list = g_list_append(one_test->exec_time_list, exec_time);
							ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("%s ran and took %f seconds"), one_test->name, time);
							remove_test_directory(main_struct, one_test);

							if (ok != TRUE)
								{ 
									ldt_log_message(main_struct->log, G_LOG_LEVEL_WARNING, _("Something went wrong with test %s"), one_test->name);
								}
							else
								{
									ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Done !"));
								}
						}
					else
						{
							ok = FALSE;
						}
				}
		}
}


/**
 *  Function called by the submenus of a single test that runs
 *  the test many times (increasing)
 */
static void menu_run_many_tests(GtkWidget *widget, gpointer data)
{
	main_struct_t *main_struct = (main_struct_t *) data;
	GtkWidget *main_window = glade_xml_get_widget(main_struct->xml, "main_window");
	test_t *one_test = NULL;
	gboolean ok = FALSE;

	one_test = find_the_test_that_called_me(main_struct, widget);

	if (one_test != NULL)
		{
			ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Selected test : %s"), one_test->name);

			if (main_struct->dirname == NULL)
				{
					main_struct->dirname = select_a_folder(main_window);
				}

			if (main_struct->dirname != NULL)
				{
					ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Now Running %s test"), one_test->name);

					ok = run_one_test_increasing(main_struct, one_test);

					if (ok != TRUE)
						{ 
							ldt_log_message(main_struct->log, G_LOG_LEVEL_WARNING, _("Something went wrong with test %s"), one_test->name);
						}
					else
						{
							ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Done !"));
						}
					
				}
		}
}


/**
 *  Function called by the submenusof a single test that runs
 *  the test in a threaded way
 */
static void menu_run_threaded_tests(GtkWidget *widget, gpointer data)
{
	main_struct_t *main_struct = (main_struct_t *) data;
	GtkWidget *main_window = glade_xml_get_widget(main_struct->xml, "main_window");
	test_t *one_test = NULL;
	gboolean ok = FALSE;

	one_test = find_the_test_that_called_me(main_struct, widget);

	if (one_test != NULL)
		{

			if (main_struct->dirname == NULL)
				{
					main_struct->dirname = select_a_folder(main_window);
				}

			if (main_struct->dirname != NULL)
				{
					ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Now Running %s test"), one_test->name);

					ok = run_one_test_threaded_increasing(main_struct, one_test);

					if (ok != TRUE)
						{ 
							ldt_log_message(main_struct->log, G_LOG_LEVEL_WARNING, _("Something went wrong with test %s"), one_test->name);
						}
					else
						{
							ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Done !"));
						}
				}
		}
}


/**
 *  Function called by the submenusof a single test that runs
 *  the test in a threaded way
 */
static void menu_run_many_threaded_tests(GtkWidget *widget, gpointer data)
{
	main_struct_t *main_struct = (main_struct_t *) data;
	GtkWidget *main_window = glade_xml_get_widget(main_struct->xml, "main_window");
	test_t *one_test = NULL;
	gboolean ok = FALSE;

	one_test = find_the_test_that_called_me(main_struct, widget);

	if (one_test != NULL)
		{

			if (main_struct->dirname == NULL)
				{
					main_struct->dirname = select_a_folder(main_window);
				}

			if (main_struct->dirname != NULL)
				{
					ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Now Running %s test"), one_test->name);

					ok = run_one_test_many_threaded_increasing(main_struct, one_test);

					if (ok != TRUE)
						{ 
							ldt_log_message(main_struct->log, G_LOG_LEVEL_WARNING, _("Something went wrong with test %s"), one_test->name);
						}
					else
						{
							ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Done !"));
						}
				}
		}
}



/**
 *  Function called to run a test with varying conditions
 */
static void menu_run_vary(GtkWidget *widget, gpointer data)
{
	main_struct_t *main_struct = (main_struct_t *) data;
	GtkWidget *main_window = glade_xml_get_widget(main_struct->xml, "main_window");
	test_t *one_test = NULL;
	gboolean ok = FALSE;

	one_test = find_the_test_that_called_me(main_struct, widget);

	if (one_test != NULL)
		{

			if (main_struct->dirname == NULL)
				{
					main_struct->dirname = select_a_folder(main_window);
				}

			if (main_struct->dirname != NULL)
				{
					ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Now Running %s test"), one_test->name);

					ok = run_one_test_varying(main_struct, one_test);

					if (ok != TRUE)
						{ 
							ldt_log_message(main_struct->log, G_LOG_LEVEL_WARNING, _("Something went wrong with test %s"), one_test->name);
						}
					else
						{
							ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("Done !"));
						}
				}
		}
}




/**
 *  Looks for a groupname in the menu "menu_stress"
 *  Returns the corresponding menu_item or NULL if
 *  not find
 */
static GtkWidget *find_groupname(GtkMenuShell *menu_stress, gchar *groupname)
{
	GList *children = menu_stress->children;
	GtkMenuItem *menu_item = NULL;
	GtkLabel *label = NULL;
	const gchar *text = NULL;
	GtkWidget *the_widget = NULL;

	while (the_widget == NULL && children != NULL)
		{
			menu_item = GTK_MENU_ITEM(children->data);
			
			if (menu_item != NULL)
				{

					label = GTK_LABEL(gtk_bin_get_child(GTK_BIN(menu_item)));
					
					if (label != NULL)
						{

							text =  gtk_label_get_text(label);
							
							if (text != NULL)
								{

									if (g_ascii_strcasecmp(groupname, text) == 0)
										{
											the_widget = GTK_WIDGET(menu_item);
										}
								}
						}
				}

			children = g_list_next(children);
		}

	return the_widget;
}


/**
 *  Menu and submenus creations for the specified test
 */
static void create_sub_menus(main_struct_t *main_struct, GtkWidget *menu_stress, test_t *one_test)
{
	GtkWidget *new_menu_item = NULL;
	GtkWidget *nmi_many = NULL;
	GtkWidget *nmi_threaded = NULL;
	GtkWidget *nmi_threaded_many = NULL;
	GtkWidget *nmi_vary = NULL;
	GtkWidget *nmi_one = NULL;
	GtkWidget *new_submenu = NULL;

	GtkWidget *group_menu_item = NULL;
	GtkWidget *group_menu = NULL;


	group_menu_item = find_groupname(GTK_MENU_SHELL(menu_stress), one_test->groupname);

	if (group_menu_item != NULL)
		{
			group_menu = gtk_menu_item_get_submenu(GTK_MENU_ITEM(group_menu_item));
		}
	else
		{
			group_menu_item = gtk_menu_item_new_with_label(one_test->groupname);
			gtk_menu_shell_append(GTK_MENU_SHELL(menu_stress), group_menu_item);
			gtk_widget_show(group_menu_item);

			group_menu = gtk_menu_new();
			gtk_menu_item_set_submenu(GTK_MENU_ITEM(group_menu_item), group_menu);
			gtk_widget_show(group_menu);
		}

	new_menu_item = gtk_menu_item_new_with_label(one_test->name);
	gtk_menu_shell_append(GTK_MENU_SHELL(group_menu), new_menu_item);
	gtk_widget_show(new_menu_item);
	
	new_submenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(new_menu_item), new_submenu);
	gtk_widget_show(new_submenu);

	nmi_one = gtk_menu_item_new_with_label(_("One atomic run"));
	gtk_widget_set_name(nmi_one, one_test->name);
	gtk_menu_shell_append(GTK_MENU_SHELL(new_submenu), nmi_one);
	g_signal_connect(G_OBJECT(nmi_one), "activate", 
					 G_CALLBACK(menu_run_one_test), main_struct);
	gtk_widget_show(nmi_one);

	nmi_many = gtk_menu_item_new_with_label(_("Many atomic runs"));
	gtk_widget_set_name(nmi_many, one_test->name);
	gtk_menu_shell_append(GTK_MENU_SHELL(new_submenu), nmi_many);
	g_signal_connect(G_OBJECT(nmi_many), "activate", 
					 G_CALLBACK(menu_run_many_tests), main_struct);
	gtk_widget_show(nmi_many);

	nmi_threaded = gtk_menu_item_new_with_label(_("Many threaded runs"));
	gtk_widget_set_name(nmi_threaded, one_test->name);
	gtk_menu_shell_append(GTK_MENU_SHELL(new_submenu), nmi_threaded);	
	g_signal_connect(G_OBJECT(nmi_threaded), "activate", 
					 G_CALLBACK(menu_run_threaded_tests), main_struct);
	gtk_widget_show(nmi_threaded);

	nmi_threaded_many = gtk_menu_item_new_with_label(_("Many threaded runs many times"));
	gtk_widget_set_name(nmi_threaded_many, one_test->name);
	gtk_menu_shell_append(GTK_MENU_SHELL(new_submenu), nmi_threaded_many);	
	g_signal_connect(G_OBJECT(nmi_threaded_many), "activate", 
					 G_CALLBACK(menu_run_many_threaded_tests), main_struct);
	gtk_widget_show(nmi_threaded_many);

	if (one_test->vary_func_list != NULL)
		{
			nmi_vary = gtk_menu_item_new_with_label(_("Many threaded runs many times with variations"));
			gtk_widget_set_name(nmi_vary, one_test->name);
			gtk_menu_shell_append(GTK_MENU_SHELL(new_submenu), nmi_vary);	
			g_signal_connect(G_OBJECT(nmi_vary), "activate", 
							 G_CALLBACK(menu_run_vary), main_struct);
			gtk_widget_show(nmi_vary);
		}
}



/**
 *  Inserts new menus to the interface (one for each existing test)
 */
static void insert_all_tests_in_menus(main_struct_t *main_struct)
{
	GList *test_list = main_struct->test_list;
	test_t *one_test = NULL;
	GtkWidget *menu_stress = glade_xml_get_widget(main_struct->xml, "menu2");
	guint i = 2;

  while (test_list != NULL)
	  {
		  one_test = (test_t *) test_list->data;
		  create_sub_menus(main_struct, menu_stress, one_test);
		  test_list = g_list_next(test_list);
		  i++;
	  }

}


/**
 *  Let's the user choose where the tests might be running
 */
static void menu_open_folder(GtkWidget *widget, gpointer data)
{
	main_struct_t *main_struct = (main_struct_t *) data;
	GtkWidget *main_window = glade_xml_get_widget(main_struct->xml, "menu_window");

	if (main_struct->dirname != NULL)
		{
			g_free(main_struct->dirname);
		}

	main_struct->dirname = select_a_folder(main_window);
}


/** 
 *  Window closed
 */
static gboolean delete_main_window_event(GtkWidget *widget, GdkEvent  *event, gpointer data )
{
  gtk_widget_destroy(widget);

  return TRUE;
}

/** 
 *  Window destroyed
 */
static void destroy_main_window(GtkWidget *widget, gpointer data)
{
  main_struct_t *main_struct = (main_struct_t *) data;

  fin_du_programme(main_struct);
}

/**
 *  Connecting signals
 */
static void connect_menu_signals(main_struct_t *main_struct)
{

  /* Closing the program (Quit menu or Ctrl+q) */
  g_signal_connect(G_OBJECT(glade_xml_get_widget(main_struct->xml, "menu_quit")), "activate",
				   G_CALLBACK(menu_quit), main_struct);

  /* Menu Run All */
  g_signal_connect(G_OBJECT(glade_xml_get_widget(main_struct->xml, "menu_run_all")), "activate",
				   G_CALLBACK(menu_run_all), main_struct);

 /* Menu Open Folder */
  g_signal_connect(G_OBJECT(glade_xml_get_widget(main_struct->xml, "menu_open_folder")), "activate",
				   G_CALLBACK(menu_open_folder), main_struct);
}


/** 
 *  Signals generated by widgets conncetion to the right
 *  functions                                   
 */
static void connect_signals(main_struct_t *main_struct)
{
  /* Window closed */
  g_signal_connect(G_OBJECT(glade_xml_get_widget(main_struct->xml, "main_window")), "delete-event", 
		   G_CALLBACK(delete_main_window_event), NULL);

  /* Window destroyed */
  g_signal_connect(G_OBJECT(glade_xml_get_widget(main_struct->xml, "main_window")), "destroy", 
		   G_CALLBACK(destroy_main_window), main_struct);

  /* Menu signals connections  */
  connect_menu_signals(main_struct);
}


/**
 *  Loading the glade XML interface
 */
static gboolean load_xml_interface(main_struct_t *main_struct)
{
  main_struct->xml = ldt_load_glade_xml_file(main_struct->location_list, "gstressfs.glade");

  if (main_struct->xml == NULL)
    return FALSE;
  else
    return TRUE;
}

/**
 *  Inits internationalisation (don't know wether it works or not!)
 */
static void init_international_languages(void)
{
  gchar *result = NULL;

#ifdef WINDOWS
  gchar *locale_dir = NULL;

  locale_dir = g_strdup_printf("%s\\locale", g_get_current_dir());
  result = bindtextdomain(GETTEXT_PACKAGE, locale_dir);
  g_free(locale_dir);
#else
  result = bindtextdomain(GETTEXT_PACKAGE, PROGRAM_LOCALE_DIR);
#endif
  
  bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
  textdomain(GETTEXT_PACKAGE);
}


/**
 *  Structure inits
 */
static main_struct_t *init_main_struct(void)
{
  main_struct_t *main_struct = NULL;
 
  main_struct = (main_struct_t *) g_malloc0 (sizeof(main_struct_t));

  if (main_struct != NULL)
    {
      main_struct->xml = NULL;     /* Glade XML interface */
      main_struct->location_list = ldt_init_location_list(NULL, "gstressfs"); /* localisation list  */
      main_struct->log = NULL;     /* log system */
	  main_struct->dirname = NULL;
	  main_struct->test_list = init_test_list(main_struct);

      return main_struct;
    }
  else
    return NULL;
}

/** 
 *  Log system init
 */
static void init_log_window(main_struct_t *main_struct)
{
  GtkTextView *textview = GTK_TEXT_VIEW(glade_xml_get_widget(main_struct->xml, "log_textview"));

  /* adds the textview to the log system */
  main_struct->log = ldt_init_log_domain(textview, ProgName, TRUE); /* PROGRAM_DEBUG); */
}


/**
 *  inits the interface that has been previously loaded
 */
static void init_interface(main_struct_t *main_struct)
{
  /* main window */
  connect_signals(main_struct); 

  /* Log system initialisation */
  init_log_window(main_struct);

  /* inserts all tests menus in the Stress menu */
  insert_all_tests_in_menus(main_struct);

  /* showing all displayable widgets */
  gtk_widget_show_all(glade_xml_get_widget(main_struct->xml, "main_window"));

  /* Welcome message */
  ldt_log_message(main_struct->log, G_LOG_LEVEL_MESSAGE, _("GstressFS - Version %s - Date %s - %s"), ProgVersion, ProgDate, ProgAuthor);

  /* Logging all available tests  */
  log_all_reconnized_test(main_struct);

  
}

/** 
 *  Normal ending for the program
 */
static void fin_du_programme(main_struct_t *main_struct)
{
  gtk_main_quit();
}


/**
 *  main program (where all begins)
 */
int main(int argc, char **argv)
{
	gboolean exit_value = TRUE;
	main_struct_t *main_struct;
 

	/* Threads initialisation as soon as possible */
	if (!g_thread_supported())
		{
			g_thread_init (NULL);
		}
	else
		{
			return FALSE;
		}

	/* GTK Initialisation */
	exit_value = gtk_init_check(&argc, &argv);

	/* init_international_languages(); */

	main_struct = init_main_struct();

	if (main_struct != NULL)
		{	
			if (load_xml_interface(main_struct))
				{
					init_interface(main_struct);
	  
					/* main gtk loop */
					gtk_main();

				}
			else
				{
					fprintf(stderr, _("Error while loading main interface !"));
					exit_value = FALSE;
				}
		}
	else
		{
			fprintf(stderr, _("Unable to init the main structure !"));
			exit_value = FALSE;
		}

	return exit_value;
}
