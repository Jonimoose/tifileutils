/*
 * Ti File Utils
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
#include <getopt.h>
#include <string.h>
#include <tifiles.h>
#include <ticonv.h>

/* Flag set by ‘--verbose’. */
static int verbose_flag;

enum modes {MODE_NONE, MODE_INFO, MODE_MOD, MODE_HELP};

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
    char c;
    int curmode=MODE_NONE;
    static struct option long_options[] = {
        {"help", 0, 0, 'h'},
        {"version", 0, 0, 'V'},
        {"entry", required_argument, 0, 'e'},
        {"attr", required_argument, 0, 'a'},
        {"rename", required_argument, 0, 'r'},
        {"folder", required_argument, 0, 'f'},
        {"info", 0, 0, 'i'},
        {"comment", required_argument, 0, 'C'},
        {"type", required_argument, 0, 't'},
        {"verbose", 0, &verbose_flag, 'v'},
        {0, 0, 0, 0}
    };
    char *ofile=NULL, *ifile=NULL;
    int option_index = 0;
    int entry=0;
    int attr=-1;
    char *name=NULL, *foldname=NULL, *comment=NULL;
    FileContent *regular;

    while ((c = getopt_long(argc, argv, ":hVvi:f:C:a:r:e:n:t:", long_options, &option_index)) != -1) {
    switch (c) {
        case 'V':
            printf("Insert Verison Text here\n");
            break;
        default:
        case 'h':
            curmode=MODE_HELP;
            break;
        case 'e':
            if (optarg) 
                entry= (int)strtol(optarg, NULL, 10);
            break;
        case 'r':
            if (curmode!=MODE_HELP && optarg){
                curmode = MODE_MOD;
                name=optarg;
            }
            else {
                curmode = MODE_HELP;
            }
            break;
        case 't':
            fprintf(stderr,"Changing of entry type is not yet implemented.\n");
            // if (curmode!=MODE_HELP && optarg){
                // curmode = MODE_MOD;
                // name=optarg;
            // }
            // else {
                // curmode = MODE_HELP;
            // }
            break;
        case 'n':
            if (curmode!=MODE_HELP && optarg){
                curmode = MODE_MOD;
                name=optarg;
            }
            else {
                curmode = MODE_HELP;
            }
            break;
        case 'f':
            if (curmode!=MODE_HELP && optarg){
                curmode = MODE_MOD;
                foldname=optarg;
            }
            else {
                curmode = MODE_HELP;
            }
            break;
        case 'C':
            if (curmode!=MODE_HELP && optarg){
                curmode = MODE_MOD;
                comment=optarg;
            }
            else {
                curmode = MODE_HELP;
            }
            break;
        case 'a':
            if (curmode!=MODE_HELP && optarg){
                curmode = MODE_MOD;
                if (!strcmp("locked",optarg)) {
                    attr= ATTRB_LOCKED;
                }
                else if (!strcmp("archived",optarg)){
                    attr= ATTRB_ARCHIVED;
                }
                else if (!strcmp("none",optarg)) {
                    attr = ATTRB_NONE;
                }
                else {
                    curmode = MODE_HELP;
                    fprintf(stderr,"Valid Attributes are locked, archived and none.\n");
                }
            }
            else { 
                curmode=MODE_HELP;
            }
            break;
        case 'i':
            if (curmode==MODE_NONE && optarg){
                curmode = MODE_INFO;
            }
            else {
                curmode = MODE_HELP;
            };
            break;
        case ':':       /* -f or -o without operand */
            fprintf(stderr,
                    "Option -%c requires an operand\n", optopt);
            curmode=MODE_HELP;
            break;
        case '?':
            fprintf(stderr,
                    "Unrecognised option: -%c\n", optopt);
            curmode=MODE_HELP;
        case 'v':
            verbose_flag++;
            break;
        }
    }
      
    
    
    if (curmode==MODE_HELP || curmode==MODE_NONE) 
        printf("insert help text here\n");
    
    tifiles_library_init();
    
    if (curmode==MODE_MOD){
        if (optind++ <= argc) {
            ifile=argv[optind];
        } 
        else {
            fprintf(stderr, "No file specified.\n");
            return 0;
        }
        
        // printf("%x [%s]\n",(void*)ifile,ifile);
        if (tifiles_file_is_regular(ifile)){
            CalcModel model = tifiles_file_get_model(ifile);
            regular = tifiles_content_create_regular(model);
            tifiles_file_read_regular(ifile, regular);
            if (entry > regular->num_entries || entry <0) {
                fprintf(stderr, "invalid entry specified\n");
                return 0;
            }
            
            if (comment!=NULL)
                strcpy(regular->comment, comment);
            
            if (attr!= -1){
                
                regular->entries[entry]->attr=attr;
            }
            
            if (foldname!=NULL && tifiles_calc_is_ti9x(model)){
                if(strlen(foldname) > 8)
                    foldname[8] = '\0';
                
                strcpy(regular->entries[entry]->folder, foldname);
            }
            
            
            if(name!=NULL){
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
            if (verbose_flag)
                tifiles_file_display(ifile);
            tifiles_file_write_regular(ifile, regular, NULL);
        }else
            fprintf(stderr, "invalid filetype\n");
        
    }
    
    if (curmode==MODE_INFO){
        if (optind++ <= argc) {
            ifile=argv[optind];
        } 
        else {
            fprintf(stderr, "No file specified.\n");
            return 0;
        }
                
        // printf("%x [%s]\n",(void*)ifile,ifile);
        if (tifiles_file_is_regular(ifile))
            tifiles_file_display(ifile);
        else
            fprintf(stderr, "invalid filetype\n");
    }
    
    tifiles_library_exit();

    return 0;
}
