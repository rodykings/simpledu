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


pid_t pids[MAX_PIDS];     

/* 07-04-2020 14:54 ©Rodrigo e Deborah */
//When parent receive SIGINT sends SIGSTOP to childs
void signalHandler(int signal){
    printf("\nSIGINT RECEIVED...\n");
    killpids(pids, 20);
    exit(0);
}

int main(int argc, char * argv[], char * envp[]){
   

    char * logFile = getenv("LOG_FILENAME");

    //Writes in LogFile
    log_create(logFile, argv, argc);

    struct flags spcFlags;              //The flags on the argument line
    int pathPos;


    if(strcmp(argv[1], "help") == 0 && argc == 2){
        print_help();
        log_exit(logFile, 0);
        //exit(0);
    }

    //Error conditions
    if(argc > 9 || argc == 1){
        printf("Usage: %s -l <flags> <dirname>\nFor more information about the flags, run %s help\n", argv[0], argv[0]);
        log_exit(logFile, 1);
        //exit(1);
    }


    if(fill_flags(&spcFlags, argv, argc, &pathPos)){
        printf("Someting went wrong when setting up the flags!\n");
        log_exit(logFile, 6);
    }   
    

    //printf("Starting with values: %s %s %s\n", argv[0],argv[1], argv[2]);
    
    struct dirent *dir_entry;           //directory entry currently reading
    struct stat filestat;               //info about the file currently reading

            //array of pids of child processes
    //it_pid=0,
    
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

    while((dir_entry = readdir(home))!=NULL){

        char *filename = dir_entry->d_name;

        if(strcmp(filename, ".")==0 || strcmp(filename, "..") ==0){
            continue;
        }

        //LOG
        //printf("Processing %s file in process %d\n", filename, getpid());

        char filepath[300];
        if(sprintf(filepath, "%s/%s", argv[pathPos], filename) < 0){
            printf("Error in sprintf\n");
            log_exit(logFile, 5);
            //exit(5);
        }

        //printf("filepath: %s\n",filepath);

        //Reads file
        if(stat(filepath, &filestat)!=0){
            printf("Error in stat\n");
            log_exit(logFile, 3);
            //exit(3);
        }
        
        if(spcFlags.all && S_ISREG(filestat.st_mode)){
            if(spcFlags.bytes)
                printf("%ld\t%s\n", filestat.st_size,filepath);
            else if(spcFlags.block_size)
                printf("%ld\t%s\n", filestat.st_size/spcFlags.nbytes,filepath);
            else
                printf("%ld\t%s\n", filestat.st_size/1024,filepath);

        }else if(S_ISDIR(filestat.st_mode)){          //Verifies if is directory  
            
            signal(SIGINT, signalHandler);

            pid_t pid = fork();

            putpid(pids, MAX_PIDS, pid);

            if(pid == 0){        //Child process
                
                char * new_arg[argc];
                new_arg[argc] = NULL;
                
                for(int i = 0; i < argc; i++)
                    new_arg[i] = malloc(100);
                

                copy_values(new_arg,argv,argc);
                
                if(sprintf(new_arg[pathPos], "%s/%s", argv[pathPos] ,filename) < 0){
                    printf("sprintf\n");
                    log_exit(logFile, 4);
                    //exit(4);
                }

                //write(STDOUT_FILENO,"after sprintf\n",14);
                
                /*
                if(strlen(argv[0]) == 10){
                    new_arg[0] = "../simpledu";
                }else {
                    sprintf(new_arg[0], "../%s", argv[0]);
                }
                */

                //printf("Going to %s using prog %s with flag %s in process %d\n", new_arg[argc-1], new_arg[0], new_arg[1], getpid());    
                
                execvp(new_arg[0], new_arg);
                printf("Error in executing recursive simpledu to %s\n", new_arg[argc-1]);
                log_exit(logFile, 3);
                //exit(3);
            }else{              //Parent process

               

             /*   
                if(putpid(pids, MAX_PIDS, pid)!=0){
                    printf("Maximum number of forks exceeded\n");
                    exit(5);
                }
                */
            }

            int ret;
            while ((ret = wait(&status)) > 0); 
            if(spcFlags.bytes)
                printf("%ld\t%s\n", filestat.st_size,filepath);
            else if(spcFlags.block_size)
                printf("%ld\t%s\n", filestat.st_size/spcFlags.nbytes,filepath);
            else
                printf("%ld\t%s\n", filestat.st_size/1024,filepath);

            //sleep(5);
        }
       
       

        /*
        int ret;
        while((ret = waitpid(-1, &status, WNOHANG) != 0) && ret != -1);
        */
        //printf("passei o while ma friends!\n");
        
        //Verifica se algum processo filho acabou
        /*
        it_pid = 0;
        while (pids[it_pid] != (pid_t) -1){
            
            if(pids[it_pid] != (pid_t) 0){
                waitpid(pids[it_pid], &status, WNOHANG);
            }
            it_pid++;
                
        }*/
    }
    
    //Waiting for all childs to end (needs to change!)
    //while(wait(&status)!=-1);
    int ret;
    while ((ret = wait(&status)) > 0);

    
    log_exit(logFile, 0);
    
}
