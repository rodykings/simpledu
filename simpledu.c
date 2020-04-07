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
#define READ 0
#define WRITE 1
#define MAX_LINE 256

void fillpids(pid_t *a, int n);

int putpid(pid_t *a, int n, pid_t pid);


int main(int argc, char * argv[], char * envp[]){
    

    char * logFile = getenv("LOG_FILENAME");

    
    //Writes in LogFile
    log_create(logFile, argv, argc);

    struct flags spcFlags;              //The flags on the argument line
    int pathPos, depth_pos;


    if(argc == 2 && strcmp(argv[1], "help") == 0){
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


    if(fill_flags(&spcFlags, argv, argc, &pathPos, &depth_pos)){
        printf("Someting went wrong when setting up the flags!\n");
        log_exit(logFile, 6);
    }   
    

    //printf("Starting with values: %s %s %s\n", argv[0],argv[1], argv[2]);
    
    struct dirent *dir_entry;           //directory entry currently reading
    struct stat filestat;               //info about the file currently reading

    //pid_t pids[MAX_PIDS];             //array of pids of child processes
    //it_pid=0,

    
    long int sum = 0;                   //Sum of the subdirectories and files of the directory
    
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
    //fillpids(pids, MAX_PIDS);

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
        }
        
        //Processes regular file
        if(S_ISREG(filestat.st_mode)){
            char line[MAX_LINE];
            
            if(spcFlags.bytes){

                if(spcFlags.all && (!spcFlags.max_depth || spcFlags.depth_level > 0)){ 

                    sprintf(line,"%ld\t%s\n", filestat.st_size,filepath);
                    write(STDOUT_FILENO, line, strlen(line));
                    log_pipe(logFile,line,'s');
                }

                sum += filestat.st_size;

            }else{

                if(spcFlags.all && (!spcFlags.max_depth || spcFlags.depth_level > 0)){ 

                    sprintf(line,"%ld\t%s\n", filestat.st_blocks/2,filepath); 
                    write(STDOUT_FILENO, line, strlen(line));
                    log_pipe(logFile,line,'s');   
                }

                sum += filestat.st_blocks/2;
            }
            
        
            //log_write(logFile,sum);

        }else if(S_ISDIR(filestat.st_mode)){          //Verifies if is directory  
            
            
            int fd[2];

            if(pipe(fd) == -1){
                log_exit(logFile,6);
            }

            pid_t pid = fork();

            if(pid == 0){        //Child process

                if(spcFlags.max_depth && spcFlags.depth_level > 0)
                    spcFlags.depth_level--;

                close(fd[READ]);

                dup2(fd[WRITE], STDOUT_FILENO);

            
                char * new_arg[argc];
                new_arg[argc] = NULL;
                
                for(int i = 0; i < argc; i++)
                    new_arg[i] = malloc(100);
                

                copy_values(new_arg,argv,argc);
                
                if(spcFlags.max_depth){
                    char new_depth[20];
                    sprintf(new_depth, "--max-depth=%d", spcFlags.depth_level);
                    strcpy(new_arg[depth_pos],new_depth);
                }
                
                if(sprintf(new_arg[pathPos], "%s/%s", argv[pathPos] ,filename) < 0){
                    printf("sprintf\n");
                    log_exit(logFile, 4);
                }

                //printf("Going to %s using prog %s with flag %s in process %d\n", new_arg[argc-1], new_arg[0], new_arg[1], getpid());    
                
                execvp(new_arg[0], new_arg);
                printf("Error in executing recursive simpledu to %s\n", new_arg[argc-1]);
                log_exit(logFile, 3);
                
            }else{              //Parent process
                
                
                close(fd[WRITE]);
                
                int ret = 1;
                while ((ret = wait(&status)) > 0); 
                
                int n;
                long int res;
                char line[MAX_LINE];

                while((n=read(fd[READ],line, MAX_LINE)) != 0){
                
                    log_pipe(logFile,line,'r');
                    
                    if(!spcFlags.max_depth || spcFlags.depth_level > 0){

                        write(STDOUT_FILENO, line, n);
                        log_pipe(logFile,line,'s');
                    }

                    if(extract_number(line, &res) == -1) log_exit(logFile,8);
                    sum+=res;
                }

                //log_write(logFile, sum);

                //Close up files
                close(fd[READ]);
            
                
             /*   
                if(putpid(pids, MAX_PIDS, pid)!=0){
                    printf("Maximum number of forks exceeded\n");
                    exit(5);
                }
                */
            }

            
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
    //int ret;
    //while ((ret = wait(&status)) > 0);
    char line[MAX_LINE];

    //Reads information about file
    if(stat(".", &filestat)!=0){
        printf("Error in stat\n");
        log_exit(logFile, 3);
    }

    if(spcFlags.bytes){
        sum+= filestat.st_size;
        sprintf(line,"%ld\t%s\n", sum, argv[pathPos]);
        write(STDOUT_FILENO, line, strlen(line));

    }else{
        sum+= filestat.st_blocks/2;
        sprintf(line,"%ld\t%s\n", sum, argv[pathPos]);
        write(STDOUT_FILENO, line, strlen(line));
    }
    log_pipe(logFile,line,'s');

                
    log_exit(logFile, 0);

    
}

void fillpids(pid_t *a, int n){
    for (int i = 0; i<n; i++){
        a[i]= (pid_t) 0;
    }
    a[n-1]= (pid_t) -1;
}

int putpid(pid_t *a, int n, pid_t pid){
    int i=0;
    while(a[i]!=-1){
        if(a[i]==0){
            a[i] = pid;
            return 0;
        }
    }
    return 1;
}
