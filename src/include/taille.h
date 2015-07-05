/* -*- indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */
/*
   taille.h
   taille project.

   (C) Copyright 2008 - 2015 Olivier Delhomme
   e-mail : olivier.delhomme@free.fr
 
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

#ifndef _TAILLE_H_
#define _TAILLE_H_

#include <glib.h>
#include <stdio.h>
#include <getopt.h>

#include "config.h"


#define ProgName "taille"
#define ProgVersion "0.0.1"
#define ProgDate "05.08.2008"
#define ProgAuthor "Olivier Delhomme <olivier.delhomme@free.fr>"

typedef struct
{
	char usage;        /* To know if we displayed the usage message */
} Options;

static struct option const long_options[] =
{
	{"version", no_argument, NULL, 'v'},
	{"help", no_argument, NULL, 'h'},
	{NULL, 0, NULL, 0} 
};


#endif /* _TAILLE_H_ */
