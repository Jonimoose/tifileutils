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

enum modes {MODE_NONE, MODE_HELP, MODE_EX, MODE_LS, MODE_CREATE, MODE_ADD, MODE_DEL};


int
main(int argc, char *argv[])
{
    char c;
    int curmode=MODE_NONE;
    static struct option long_options[] = {
        {"help", 0, 0, 'h'},
        {"list", required_argument, 0, 'l'},
        {"create", required_argument, 0, 'c'},
        {"extract", required_argument, 0, 'x'},
        {"verbose", 0, &verbose_flag, 'v'},
        {0, 0, 0, 0}
    };
    char *ofile=NULL, *ifile=NULL;
    int option_index = 0;

    while ((c = getopt_long(argc, argv, ":hVvl:x:c:", long_options, &option_index)) != -1) {
    switch (c) {
        case 'V':
            printf("Insert Verison Text here\n");
            break;
        default:
        case 'h':
            curmode=MODE_NONE;
            break;
        case 'x':
            if (curmode==MODE_NONE && optarg){
                curmode = MODE_EX;
                ifile=optarg;
            }
            else {
                curmode = MODE_HELP;
            }
            break;
        case 'c':
            if (curmode==MODE_NONE && optarg){
                curmode = MODE_CREATE;
                ofile=optarg;
            }
            else {
                curmode = MODE_HELP;
            }
            break;
        case 'l':
            if (curmode==MODE_NONE && optarg){
                curmode = MODE_LS;
                ifile=optarg;
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
    
    if (curmode==MODE_HELP || curmode==MODE_NONE) printf("insert help text here\n");
    
    tifiles_library_init();
    
    if (curmode==MODE_EX){
        //printf("%x [%s]\n",(void*)ifile,ifile);
        if (tifiles_file_is_group(ifile))
            tifiles_ungroup_file(ifile, NULL);
	    else if(tifiles_file_is_tigroup(ifile))
            tifiles_untigroup_file(ifile, NULL);
        else
            fprintf(stderr, "invalid filetype");
    }
    
    
    
    if (curmode==MODE_CREATE){
        // //printf("%x [%s]\n",(void*)ifile,ifile);
        char **ifiles = NULL;
        int ret = 0;
        int n = (argc - optind + 1);
        
        ifiles = (char **)malloc((n + 1) * sizeof(char *));
        if (ifiles == NULL)
            return 1;
               
        int i;
        
        for (i = 0; i < (n); i++) 
        {
            ifiles[i] = (char *)malloc(sizeof(char));
            if (ifiles[i] == NULL)
                return 1;
            
            ifiles[i] = argv[optind + i];
            
        }
        ifiles[i] = NULL;
        
        // for (i = 0; i < (n - 1); i++)
            // ifiles[i] = argv[optind+i];
            // 
        // ifiles[n]= '\0';
        ret = tifiles_group_files(ifiles, ofile);
        
        return ret;
    }
   
    if (curmode==MODE_LS){
        //printf("%x [%s]\n",(void*)ifile,ifile);
        if (tifiles_file_is_regular(ifile))
            tifiles_file_display(ifile);
	    else if(tifiles_file_is_tigroup(ifile))
            tifiles_file_display_tigroup(ifile);
        else
            fprintf(stderr, "invalid filetype");
    }
    
    tifiles_library_exit();

    return 0;
}
