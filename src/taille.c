/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*
 *  taille.c
 *  taille - a simple tool that prints out sizes.
 *
 *  (C) Copyright 2008 - 2015 Olivier Delhomme
 *  e-mail : olivier.delhomme@free.fr
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or  (at your option) 
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY;  without even the implied warranty of
 *  MERCHANTABILITY  or  FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. 
 */

#include "taille.h"

static gboolean version(void);
static gboolean usage(int status);
static gboolean manage_command_line_options(Options *opt, int argc, char ** argv);
static void init_international_languages(void);
static void print_int_types(void);


/**
 *  prints program name, version, author, date and licence
 *  to the standard output
 */
static gboolean version(void)
{
	fprintf (stdout, _("%s, %s - %s - Version %s - License %s\n"), ProgName, ProgAuthor, ProgDate, ProgVersion, _("GPL V2 or higher"));
	return TRUE;
}


/**
 *  Function that informs the user about the command line options
 *  available with taille
 */
static gboolean usage(int status)
{
	if (status == 0)
		{
			fprintf (stderr, 
					 _("Try `%s --help' for more information.\n"), ProgName);
			return FALSE;
		}
	else
		{
			version();
			fprintf(stdout, _("\n%s is a tool that prints struct sizes"), ProgName);
			fprintf(stdout, _("\nUsage:\n  %s [options]\n"), ProgName);
			fprintf(stdout, _("\nOptions:\n\
  -h, --help\tThis help.\n\
  -v, --version\tProgram version information.\n"));
			return TRUE;
		}
}


/**
 *  Manages all the command line options and populates the
 *  Options *opt structure accordingly
 */
static gboolean manage_command_line_options(Options *opt, int argc, char ** argv)
{
	int exit_value = TRUE;
	int c = 0;

	while ((c = getopt_long (argc, argv, "vh", long_options, NULL)) != -1)
		{
			switch (c)
				{
				case 0:				/* long options handler */
					break;
	  
				case 'v':
					exit_value = version();
					opt->usage = TRUE; 	/* We do not want to continue after */
					break;
	 
				case 'h':
					exit_value = usage(1);
					opt->usage = TRUE;
					break;

				default:
					exit_value = usage(0);
					opt->usage = TRUE;
				}
		}

	if (optind < argc) /* filename */
		{
			exit_value = usage(0);
			opt->usage = TRUE;
		}

	return exit_value;
}


/**
 *  Inits internationalization domain for taille project
 */
static void init_international_languages(void)
{
    gchar *result = NULL;
    gchar *codeset = NULL;
    gchar *text_domain = NULL;

    setlocale(LC_ALL, "");
    result = bindtextdomain(GETTEXT_PACKAGE, LOCALE_DIR);
    codeset = bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    text_domain = textdomain(GETTEXT_PACKAGE);

	if (DEBUG == TRUE)
		{
			fprintf(stdout, "Gettext package : %s\n", GETTEXT_PACKAGE);

			if (result != NULL)
				{
					fprintf(stdout, "Bindtextdomain : %s\n", result);
				}

			if (codeset != NULL)
				{
					fprintf(stdout, "Code set : %s\n", codeset);
				}

			if (text_domain != NULL)
				{
					fprintf(stdout, "Text domain : %s\n", text_domain);
				}
		}
}




/**
 *  Prints int types
 */
static void print_int_types(void)
{
	unsigned long taille = 0;

	fprintf(stdout, _("Integer types:\n"));
	
	taille = sizeof(short int);
	fprintf(stdout, "\tshort int    : %ld (%ld bits)\n", taille, taille*8);

	taille = sizeof(int);	
	fprintf(stdout, "\tint          : %ld (%ld bits)\n", taille, taille*8);
	
	taille = sizeof(long int);
	fprintf(stdout, "\tlong int     : %ld (%ld bits)\n", taille, taille*8);

    taille = sizeof(long long int);
	fprintf(stdout, "\tlong long int: %ld (%ld bits)\n", taille, taille*8); 
}



static void print_ids(void)
{
	fprintf(stdout, "pid_t: %zu\n", sizeof(pid_t));
    	fprintf(stdout, "uid_t: %zu\n", sizeof(uid_t));
    	fprintf(stdout, "gid_t: %zu\n", sizeof(gid_t));
}


/**
 *  main program
 *  options :
 *   --version
 *   --help
 */
int main (int argc, char ** argv) 
{  
	Options *opt;   /* A structure to manage the command line options  */
	gboolean exit_value = TRUE;

	opt = (Options *) g_malloc0(sizeof(Options));
	opt->usage = FALSE;
	
	init_international_languages();

	/* Command line options evaluation */
	exit_value = manage_command_line_options(opt, argc, argv);
	
	if (opt->usage != TRUE)
		{
			print_int_types();
			print_ids();
		}

	g_free(opt);

	return exit_value;
}
