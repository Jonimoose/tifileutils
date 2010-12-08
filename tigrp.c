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



#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <locale.h>
#include <string.h>
#include <tifiles.h>
#include <ticonv.h>

static gboolean verbose = FALSE;
static gboolean showversion = FALSE;
static char *list = FALSE;
static char **input_files = NULL;
static char *extract = NULL;
static char *create = NULL;
static char *append = NULL;

static const GOptionEntry options[] =
  {{ "extract", 'x', 0, G_OPTION_ARG_STRING, &extract,
     "extract FILE contents", "FILE" },
   { "list", 't', 0, G_OPTION_ARG_STRING, &list,
     "list FILE contents", "FILE" },
   { "create", 'c', 0, G_OPTION_ARG_STRING, &create,
     "create new group FILE", "FILE" },
   { "append", 'r', 0, G_OPTION_ARG_STRING, &append,
     "append FILE to group", "FILE" },
   { "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose,
     "increase verbosity", NULL },
   { "version", 0, 0, G_OPTION_ARG_NONE, &showversion,
     "print program version info", NULL },
   { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &input_files,
     NULL, "[FILES]" },
   { 0, 0, 0, 0, 0, 0, 0 }};

static void print_usage(GOptionContext *ctx)
{
	char *usage = g_option_context_get_help(ctx, TRUE, NULL);
	g_printerr("%s", usage);
	g_free(usage);
	g_option_context_free(ctx);
	exit(1);
}

int
main(int argc, char *argv[])
{
    char *ifile=NULL;
    int ret = 0;

	GError *err = NULL;
    GOptionContext *ctx;
    
    setlocale(LC_ALL, "");
	
	ctx = g_option_context_new ("");
	g_option_context_add_main_entries (ctx, options, NULL);

      
    if (!g_option_context_parse(ctx, &argc, &argv, &err)) {
		g_printerr("%s: %s\n", g_get_prgname(), err->message);
		g_error_free(err);
		print_usage(ctx);
	}
    
    if (showversion) {
	    g_print("%s (%s)\n"
		    "Copyright (C) 2010 Jon Sturm\n"
		    "This program is free software. "
		    " There is NO WARRANTY of any kind.\n"
		    "Report bugs to %s.\n",
			g_get_prgname(), "Ti File Utils", "jonimoose@gmail.com");
		exit(0);
	}
    
    /* check for unparsed options/filenames */
	if (argc != 1)
		print_usage(ctx);
    
	tifiles_library_init();
	
	if (extract){
		ifile = extract;
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
		// //printf("%x [%s]\n",(void*)ifile,ifile);
		//char **ifiles = NULL;
		//
		//int n = (argc - optind + 1);
		//
		//if (( ifiles = (char **)malloc((n + 1) * sizeof(char *))) == NULL)
			//ret = 1;
		//else {
			//int i;
			//for (i = 0; i < (n); i++) 
			//{
				//ifiles[i] = argv[optind + i];              
			//}
			//ifiles[i] = NULL;
			
			ret = tifiles_group_files(input_files, create);
						
			//free(ifiles);
		// }
	}
	else if (append) {
	    if (tifiles_file_is_group(input_files[0]) && tifiles_file_is_single(append))
		ret = tifiles_group_add_file(append, input_files[0]);
	    else if(tifiles_file_is_tigroup(input_files[0]) && tifiles_file_is_single(append))
		ret = tifiles_tigroup_add_file(append, input_files[0]);
	    else {
		fprintf(stderr, "Invalid or missing input file.\n");
			ret = 1;
	    }
	}
	if (list || verbose) {
	    if (list)
	    ifile = list;
	    else
		ifile = input_files[0];

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
        
	tifiles_library_exit();
	
    return ret;
}
