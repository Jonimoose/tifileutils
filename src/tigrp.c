/*
 * TIUtils, TI File Utilities
 *
 * Copyright (c) 2010 Jon Sturm
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <locale.h>
#include <string.h>
#include <tifiles.h>
#include <ticonv.h>
#include "tiutils.h"

static gboolean *list = FALSE;
static gboolean *extract = FALSE;
static gboolean *create = FALSE;
static gboolean *append = FALSE;
static char *ifile = NULL;
static char **input_files = NULL;

static const GOptionEntry options[] =
  {{ "extract", 'x', 0, G_OPTION_ARG_NONE, &extract,
     "extract group contents", NULL },
   { "list", 't', 0, G_OPTION_ARG_NONE, &list,
     "list group contents", NULL },
   { "create", 'c', 0, G_OPTION_ARG_NONE, &create,
     "create new group", NULL },
   { "append", 'r', 0, G_OPTION_ARG_NONE, &append,
     "append to group", NULL },
   { "file", 'f', 0, G_OPTION_ARG_STRING, &ifile,
     "use group file (required, stdin/stout not supported)", "FILE" },
   { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &input_files,
     NULL, "[FILES]" },
   { 0, 0, 0, 0, 0, 0, 0 }};

int
main(int argc, char *argv[])
{
    int ret = 0;

    tu_init(argc, argv, options);

    if (extract){
	//printf("%x [%s]\n",(void*)ifile,ifile);
	if (tifiles_file_is_group(ifile))
	    tifiles_ungroup_file(ifile, NULL);
	else if(tifiles_file_is_tigroup(ifile))
	    tifiles_untigroup_file(ifile, NULL);
	else {
	    fprintf(stderr, "Invalid or missing input file.\n");
	    ret = 1;
	}
    }
    else if (create) {
	ret = tifiles_group_files(input_files, ifile);
    }
    else if (append) {
	if (tifiles_file_is_group(ifile) && tifiles_file_is_single(input_files[0]))
	    ret = tifiles_group_add_file(input_files[0], ifile);
	else if(tifiles_file_is_tigroup(ifile) && tifiles_file_is_single(input_files[0]))
	    ret = tifiles_tigroup_add_file(ifile, ifile);
	else {
	    fprintf(stderr, "Invalid or missing input file.\n");
	    ret = 1;
	}
    }
    if ((list || verbose) && ifile) {
	//printf("%x [%s]\n",(void*)ifile,ifile);
	if (tifiles_file_is_regular(ifile))
	    tifiles_file_display(ifile);
	else if(tifiles_file_is_tigroup(ifile))
	    tifiles_file_display_tigroup(ifile);
	else {
	    fprintf(stderr, "invalid filetype\n");
	    ret =1;
	}
    }
    
    g_free(ifile);
    g_strfreev(input_files);
    
    tu_exit();
	
    exit(ret);
}
