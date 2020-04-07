#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>

#include "utils.h"
#include "LogFunction.h"
#include "flags.h"

#define MAX_PIDS 20

int dp_level = 0;
pid_t pids[MAX_PIDS]; 
struct args args_st;    

/* 07-04-2020 14:54 ©Rodrigo e Deborah */
//When parent receive SIGINT sends SIGSTOP to childs
void signalHandler(int signal){
    printf("\nSIGINT RECEIVED...\n");
    killpids(pids, 20);
    exit(0);
}

int main(int argc, char * argv[], char * envp[]){

    verify_args(argv, argc);
    if(fill_args(&args_st, argv, argc)==-1){
        exit(1);
    };
    print_args(&args_st);
}

/*
    //Writes in LogFile
    char * logFile = getenv("LOG_FILENAME");
    log_create(logFile, argv, argc);

    //The flags on the argument line
    int pathPos;

    
    


    
    struct dirent *dir_entry;           //directory entry currently reading
    struct stat filestat;               //info about the file currently reading

    int status;

    DIR * home;                         //directory curently opening

    char path[200];                     //current path
    getcwd(path,200);

    sprintf(path, "%s/%s", path, argv[pathPos]);
    
    //printf("Path scanning: %s\n",path);

    //Opening directory
    if((home = opendir(path)) == NULL){

        printf("Error opening dir\n");
        log_exit(logFile, 2);
        //exit(2);
    }

    //Goes to the pretended directory
    //chdir(argv[2]);                  

    //Filling pid array
    fillpids(pids, MAX_PIDS);

    createprocess(path, 0);
    */

