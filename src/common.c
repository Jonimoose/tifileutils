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
#include <string.h>
#include <locale.h>
#include "tiutils.h"

gboolean verbose = FALSE;
gboolean showversion = FALSE;

static const GOptionEntry comm_options[] =
  {{ "verbose", 'v', 0, G_OPTION_ARG_NONE, &verbose,
     "Show details of link operations", NULL },
   { "version", 0, 0, G_OPTION_ARG_NONE, &showversion,
     "Display program version info", NULL },
   { 0, 0, 0, 0, 0, 0, 0 }};


void log_output(const gchar *domain, GLogLevelFlags level,
		       const gchar *message,
		       G_GNUC_UNUSED gpointer data)
{
  g_printerr(" [%s] ", domain);

  switch (level & G_LOG_LEVEL_MASK) {
  case G_LOG_LEVEL_ERROR: g_printerr("ERROR: "); break;
  case G_LOG_LEVEL_CRITICAL: g_printerr("CRITICAL: "); break;
  case G_LOG_LEVEL_WARNING: g_printerr("WARNING: "); break;
  }

  g_printerr("%s\n", message);
}

void print_usage(GOptionContext *ctx) 
{
    char *usage = g_option_context_get_help(ctx, TRUE, NULL);
    g_printerr("%s", usage);
    g_free(usage);
    g_option_context_free(ctx);
    exit(1);
}


tu_exit()
{
	tifiles_library_exit();
}

void tu_init(int argc, char **argv, const GOptionEntry *app_options) 
{
	GError *err = NULL;
    GOptionContext *ctx;
    
    setlocale(LC_ALL, "");
	
    ctx = g_option_context_new ("");
    if (app_options)
		g_option_context_add_main_entries(ctx, app_options, NULL);

    g_option_context_add_main_entries(ctx, comm_options, NULL);
      
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
		g_get_prgname(), PACKAGE_STRING, PACKAGE_BUGREPORT);
	exit(0);
    }
    
    /* check for unparsed options/filenames */
    if (argc != 1)
		print_usage(ctx);
	
	g_option_context_free(ctx);
	
	g_log_set_handler("tifiles", G_LOG_LEVEL_MASK, &log_output, 0);
	tifiles_library_init();

}
