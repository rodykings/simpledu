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
#include "signal.h"


#define READ 0
#define WRITE 1
#define MAX_LINE 4096

pid_t pids[MAX_PIDS];
//bool received_int = false;

int main(int argc, char * argv[], char * envp[]){
    
    char * logFile = getenv("LOG_FILENAME");
    
    //Writes in LogFile
    log_create(logFile, argv, argc);

    struct flags spcFlags;              //The flags on the argument line
    int pathPos, depth_pos;

    check_args(argc, argv, logFile);

    if(fill_flags(&spcFlags, argv, argc, &pathPos, &depth_pos)==-1){
        printf("Someting went wrong when setting up the flags!\n");
        log_exit(logFile, 2);
    }   

    struct dirent *dir_entry;           //directory entry currently reading
    struct stat filestat;               //info about the file currently reading
    
    long int sum = 0;                   //Sum of the subdirectories and files of the directory
    
    int status;

    DIR * home;                         //directory curently opening

    char path[MAX_LINE];                     //current path
    getcwd(path,MAX_LINE);

    sprintf(path, "%s/%s", path, argv[pathPos]);
    
    //printf("Path scanning: %s\n",path);

    //Opening directory
    if((home = opendir(path)) == NULL){
        write(STDOUT_FILENO, "Error opening dir\n",18);
        log_exit(logFile, 3);
        //exit(2);
    }
            

    //Filling pid array
    fillpids(pids, MAX_PIDS);

    //set signal handler
    installSignalHandlers(pids);
    
    //pause();

    while((dir_entry = readdir(home))!=NULL){

        char *filename = dir_entry->d_name;

        if(strcmp(filename, ".")==0 || strcmp(filename, "..") ==0){
            continue;
        }

        char filepath[MAX_LINE];

        if(argv[pathPos][strlen(argv[pathPos])-1] != '/'){
            if(sprintf(filepath, "%s/%s", argv[pathPos], filename) < 0){
                write(STDOUT_FILENO, "Error in sprintf\n",17);
                log_exit(logFile, 4);
            }
        }else {
             if(sprintf(filepath, "%s%s", argv[pathPos], filename) < 0){
                write(STDOUT_FILENO, "Error in sprintf\n",17);
                log_exit(logFile, 4);
            }
        }

        if(spcFlags.deref){
            //Reads file
            if(stat(filepath, &filestat)!=0){
                write(STDOUT_FILENO, "Error in stat\n",14);
                log_exit(logFile, 5);
            }
        }else {
            if(lstat(filepath, &filestat) != 0){
                write(STDOUT_FILENO, "Error in lstat\n",15);
                log_exit(logFile, 5);
            }
        }
        
        //Processes regular file
        if(S_ISREG(filestat.st_mode) || S_ISLNK(filestat.st_mode)){
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
                    
                    if(spcFlags.block_size){
                        sprintf(line,"%ld\t%s\n", (filestat.st_blocks*512)/spcFlags.nbytes,filepath); 
                        write(STDOUT_FILENO, line, strlen(line));
                        log_pipe(logFile,line,'s');

                        
                    }else{
                        sprintf(line,"%ld\t%s\n", filestat.st_blocks/2,filepath); 
                        write(STDOUT_FILENO, line, strlen(line));
                        log_pipe(logFile,line,'s');

                        
                    }
                }
                if(spcFlags.block_size)
                    sum += (filestat.st_blocks*512)/spcFlags.nbytes;
                else
                    sum += filestat.st_blocks/2;

                
            }
            

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
                    new_arg[i] = malloc(MAX_LINE);
                

                copy_values(new_arg,argv,argc);
                
                if(spcFlags.max_depth){
                    char new_depth[MAX_LINE];
                    sprintf(new_depth, "--max-depth=%d", spcFlags.depth_level);
                    strcpy(new_arg[depth_pos],new_depth);
                }
                
                if(argv[pathPos][strlen(argv[pathPos])-1] != '/'){
                    if(sprintf(new_arg[pathPos], "%s/%s", argv[pathPos] ,filename) < 0){
                        printf("sprintf\n");
                        log_exit(logFile, 7);
                    }
                }else {
                    if(sprintf(new_arg[pathPos], "%s%s", argv[pathPos] ,filename) < 0){
                        printf("sprintf\n");
                        log_exit(logFile, 7);
                    }
                }

                
                execvp(new_arg[0], new_arg);
                printf("Error in executing recursive simpledu to %s\n", new_arg[argc-1]);
                log_exit(logFile, 8);
                
            }else{              //Parent process
                
                if(putpid(pids, MAX_PIDS, pid)!=0){
                    printf("Maximum number of forks exceeded\n");
                    exit(5);
                }
                
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

                    if(!spcFlags.sep_dirs){
                        if(extract_number(line, &res) == -1) log_exit(logFile,9);
                        sum+=res;
                        
                    }
                }


                close(fd[READ]);
            
                
            }

            sleep(5);
        }
       
       

    }
    

    char line[MAX_LINE];

    //Reads information about file
    if(stat(".", &filestat)!=0){
        printf("Error in stat\n");
        log_exit(logFile,10);
    }

    if(spcFlags.bytes){

        sum+= filestat.st_size;
        sprintf(line,"%ld\t%s\n", sum, argv[pathPos]);
        write(STDOUT_FILENO, line, strlen(line));

    }else{
        if(spcFlags.block_size)
            sum+= filestat.st_blocks*512/spcFlags.nbytes;
        else
            sum+= filestat.st_blocks/2;
        sprintf(line,"%ld\t%s\n", sum, argv[pathPos]);
        write(STDOUT_FILENO, line, strlen(line));
    }
    log_entry(logFile, sum);
    log_pipe(logFile,line,'s');

                
    log_exit(logFile, 0);   
    
}