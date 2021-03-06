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


static char *name = NULL;
static char *folder = NULL;
static char *comment = NULL;
static char *attrs = NULL;
static char **input_files = NULL;
static int entry = 0;
static gboolean info = FALSE;

static const GOptionEntry options[] =
  {{ "entry", 'e', 0, G_OPTION_ARG_INT, &entry,
     "File entry number N (indexed from 0)", "N" },
   { "attr", 'a', 0, G_OPTION_ARG_STRING, &attrs,
     "Set Attribute", "ATTR" },
   { "comment", 'C', 0, G_OPTION_ARG_STRING, &comment,
     "Set file comment", "COMMENT" },
   { "name", 'n', 0, G_OPTION_ARG_STRING, &name,
     "Set entry name", "NAME" },
   { "folder", 'f', 0, G_OPTION_ARG_STRING, &folder,
     "Set entry folder (86k calcs only otherwise ignored)", "FOLDER" },
   { "info", 'i', 0, G_OPTION_ARG_NONE, &info,
     "Display file contents and info", NULL },  
   { G_OPTION_REMAINING, 0, 0, G_OPTION_ARG_FILENAME_ARRAY, &input_files,
     NULL, "FILE" },
   { 0, 0, 0, 0, 0, 0, 0 }};

/* Check if variable name should be tokenized.
   ticonv_varname_tokenize() should probably be more selective... */
/* Taken from Ben Moody's Ti Tools */
static int is_tokenized_vartype(CalcModel model, int type)
{
    switch (model) {
    case CALC_TI73:
	if (type == 0x05 || type == 0x06 || type == 0x19 || type == 0x1A
	    || type >= 0x20)
	    return 0;
	else
	    return 1;

    case CALC_TI82:
    case CALC_TI83:
    case CALC_TI83P:
    case CALC_TI84P:
	if (type == 0x05 || type == 0x06 || type == 0x14 || type == 0x15
	    || type == 0x16 || type == 0x17 || type >= 0x20)
	    return 0;
	else
	    return 1;

    default:
	return 0;
  }
}

int
main(int argc, char *argv[])
{
    char *ifile = NULL;
    int attr = -1;
    int ret = 0;
    FileContent *regular;

    tu_init(argc, argv, options);

    if (attrs){
	if (!strcmp("locked",attrs)) {
	    attr = ATTRB_LOCKED;
	}
	else if (!strcmp("archived",attrs)){
	    attr = ATTRB_ARCHIVED;
	}
	else if (!strcmp("none",attrs)) {
	    attr = ATTRB_NONE;
	}
	else {
	    fprintf(stderr,"Valid Attributes are locked, archived and none.\n");
	}
    }
    
    if (!info && (attr || comment || folder || name) && input_files) {
        
        // printf("%x [%s]\n",(void*)ifile,ifile);
        ifile = input_files[0];
        if (input_files[0] && tifiles_file_is_regular(ifile)){
            CalcModel model = tifiles_file_get_model(ifile);
            regular = tifiles_content_create_regular(model);
            tifiles_file_read_regular(ifile, regular);
            if (entry > regular->num_entries || entry <0) {
                fprintf(stderr, "invalid entry specified\n");
                entry = 0;
            }
            else {
                if (comment!=NULL)
                    strcpy(regular->comment, comment);
                
                if (attr!= -1){
                    
                    regular->entries[entry]->attr=attr;
                }
                
                if (folder!=NULL && tifiles_calc_is_ti9x(model)){
                    if(strlen(folder) > 8)
                        folder[8] = '\0';
                    
                    strcpy(regular->entries[entry]->folder, folder);
                }
                
                
                if(name != NULL){
                    char *str;
                    
                    if (is_tokenized_vartype(model, regular->entries[entry]->type) &&
                        tifiles_calc_is_ti8x(model)){
                        str = ticonv_varname_tokenize(model, name, regular->entries[entry]->type);
                    }
                    else{
                        str = name;
                    }
                    
                    if(strlen(str) > 8)
                        str[8] = '\0';
                    
                    strcpy(regular->entries[entry]->name, str);
                }
                
		tifiles_file_write_regular(ifile, regular, NULL);
		
		if (verbose)
                    tifiles_file_display(ifile);
		
            }
        }
        else {
            fprintf(stderr, "Invalid or missing input file\n");
            ret = 1;
        }
    }
    else if (info) {
	ifile = input_files[0];
	// printf("%x [%s]\n",(void*)ifile,ifile);
	if ( input_files[0] && tifiles_file_is_regular(ifile))
	    tifiles_file_display(ifile);
	else {
	    fprintf(stderr, "Invalid or missing input file");
	    ret = 1;
	}
    } 
    
    
    tu_exit();

    exit(ret);
}
