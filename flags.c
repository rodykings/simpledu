#include "flags.h"



int fill_flags(struct flags *scpflags, char * argv[], int argc, int *pathPos){

    int i=1;
    scpflags->all=false; scpflags->block_size=false; scpflags->bytes=false; scpflags->clinks = false;
    scpflags->depth_level=false; scpflags->deref =false; scpflags->max_depth = false; scpflags->sep_dirs = false;

    while(argv[i] != NULL){
        

        if(!scpflags->all && (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--all")== 0) ){
            scpflags->all = true;

        }else if(!scpflags->bytes && (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--bytes")== 0) ){
            scpflags->bytes=true;
        
        }else if(!scpflags->block_size && (strncmp(argv[i], "-B",2) == 0 || strncmp(argv[i], "--block-size=", 13)== 0) ){
            scpflags->block_size = true;

            char temp[50];
            strcpy(temp, argv[i]);
            strtok(temp, "=");
            int itemp;

            if(sscanf(strtok(NULL, " "), "%d", &itemp) == EOF){
                printf("Error on sscnaf\n");
                return -1;
            }

            scpflags->nbytes = itemp;

        }else if(!scpflags->deref && (strcmp(argv[i], "-L") == 0 || strcmp(argv[i], "--dereference") == 0)){
            scpflags->deref = true;
        }else if(!scpflags->sep_dirs && (strcmp(argv[i], "-S") == 0 || strcmp(argv[i], "--separate-dirs") == 0)){
            scpflags->sep_dirs = true;
        
        }else if(!scpflags->max_depth && strncmp(argv[i], "--max-depth=", 12) == 0){
            scpflags->max_depth = true;
            
            char temp[50];
            strcpy(temp, argv[i]);
            strtok(temp, "=");
            int itemp;

            if(sscanf(strtok(NULL, " "), "%d", &itemp) == EOF) {
                printf("Error on sscnaf\n");
                return -1;
            }

            scpflags->depth_level = itemp;
        }else if(!scpflags->clinks && (strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--count-links") == 0) ){
            scpflags->clinks = true;
        
        }else if(strncmp(argv[i], "-", 1) != 0){
            *pathPos = i;
            
        }else {
            printf("Flag not supported detected\n");
            return -1;
        }
        i++;
    }

    return 0;
}

void print_flags(struct flags f){
    printf("All: %d\n", f.all);
    printf("Bytes: %d\n", f.bytes);
    printf("Blocks: %d %d\n", f.block_size,f.nbytes);
    printf("Link: %d\n", f.clinks);
    printf("Deref: %d\n", f.deref);
    printf("Separate: %d\n", f.sep_dirs);
    printf("Max Depth: %d %d\n", f.max_depth,f.depth_level);
}