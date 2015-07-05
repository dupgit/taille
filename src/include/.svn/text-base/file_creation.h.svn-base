/* -*- indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */
/*
   file_creation.h
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
#ifndef _FILE_CREATION_H_
#define _FILE_CREATION_H_

#define MAX_TIMES_TO_RUN 1000    /* those may be a value defined in a tab preference */
#define FILES_HOW_MANY 25
#define FC_MAX_NB_THREADS 24
#define DEFAULT_FILE_SIZE 4096  
#define DEFAULT_BUF_SIZE 2048
#define MAX_FILE_SIZE 1048576

typedef struct
{
	guint howmany;     /* how many files may be created                          */
	guint buf_size;    /* buffer size used to write into the files               */
	guint64 file_size; /* file_size to create (should be a multiple of buf_size) */
	gchar *buffer;     /* the buffer it-self                                     */
} file_creation_t;

extern GList *init_test_creation_file(GList *test_list);

#endif /*_FILE_CREATION_H_ */
