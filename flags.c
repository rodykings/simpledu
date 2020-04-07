#include "flags.h"



void verify_args(char * argv[], int argc){
    if(strcmp(argv[1], "help") == 0 && argc == 2){
        print_help();
        exit(0);
    }

    //Error conditions
    if(argc > 9 || argc == 1){
        printf("Usage: %s -l <flags> <dirname>\nFor more information about the flags, run %s help\n", argv[0], argv[0]);
        exit(1);
    }
}


int fill_args(struct args *args_st, char * argv[], int argc){

    int i=1;
    args_st->all=false; args_st->block_size=false; args_st->bytes=false; args_st->clinks = false;
    args_st->depth_level=false; args_st->deref =false; args_st->max_depth = false; args_st->sep_dirs = false;
    args_st->nbytes=1024;

    while(argc != 1){
        
        if(!args_st->all && (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all")== 0) ){
            args_st->all = true;

        }else if(!args_st->bytes && (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bytes")== 0) ){
            args_st->bytes=true;
        
        }else if(!args_st->block_size && (strncmp(argv[i], "-B",2) == 0 || strncmp(argv[i], "--block-size=", 13)== 0) ){
            args_st->block_size = true;

            char temp[50];
            strcpy(temp, argv[i]);
            strtok(temp, "=");
            int itemp;

            if(sscanf(strtok(NULL, " "), "%d", &itemp) == EOF){
                printf("Error on sscna\n");
                return -1;
            }

            args_st->nbytes = itemp;

        }else if(!args_st->deref && (strcmp(argv[i], "-L") == 0 || strcmp(argv[i], "--dereference") == 0)){
            args_st->deref = true;
        }else if(!args_st->sep_dirs && (strcmp(argv[i], "-S") == 0 || strcmp(argv[i], "--separate-dirs") == 0)){
            args_st->sep_dirs = true;
        
        }else if(!args_st->max_depth && strncmp(argv[i], "--max-depth=", 12) == 0){
            args_st->max_depth = true;
            
            char temp[50];
            strcpy(temp, argv[i]);
            strtok(temp, "=");
            int itemp;

            if(sscanf(strtok(NULL, " "), "%d", &itemp) == EOF) {
                printf("Error on sscna\n");
                return -1;
            }

            args_st->depth_level = itemp;
        }else if(!args_st->clinks && (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--count-links") == 0) ){
            args_st->clinks = true;
          
        }else if(strchr(argv[i], '-') != NULL){
            printf("Flag not supported detected\n");
            return -1;
        
        }else {
            args_st->path = argv[i];
        }
        i++;
        argc--;
    }

    return 0;
}

void print_args(struct args *args_st){

    printf("Path: %s\n", args_st->path);
    printf("All: %d\n", args_st->all);
    printf("Bytes: %d\n", args_st->bytes);
    printf("Blocks: %d %d\n", args_st->block_size, args_st->nbytes);
    printf("Link: %d\n", args_st->clinks);
    printf("Dere: %d\n", args_st->deref);
    printf("Separate: %d\n", args_st->sep_dirs);
    printf("Max Depth: %d %d\n", args_st->max_depth,args_st->depth_level);
}

