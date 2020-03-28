
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

#define MAX_PIDS 20

void copy_values(char *dest[], char *copy[], int n);

void fillpids(pid_t *a, int n);

int putpid(pid_t *a, int n, pid_t pid);

int main(int argc, char * argv[], char * envp[]){

 
    //Error conditions
    if(argc != 3){
        printf("Usage: %s -l <dirname>\n", argv[0]);
        exit(1);
    }



    
    //printf("Starting with values: %s %s %s\n", argv[0],argv[1], argv[2]);
    
    struct dirent *dir_entry;           //directory entry currently reading
    struct stat filestat;               //info about the file currently reading

    //pid_t pids[MAX_PIDS];             //array of pids of child processes
     //it_pid=0,
    
    int status;

    DIR * home;                     //directory curently opening


    //Opening directory
    if((home = opendir(argv[2])) == NULL){
        //perror(argv[1]);
        printf("Error opening dir\n");
        exit(2);
    }

    //Goes to the pretended directory
    chdir(argv[2]);                  
    
    char buf[200];
    printf("Path: %s\n", getcwd(buf, 200));
    
    //Filling pid array
    //fillpids(pids, MAX_PIDS);

    while((dir_entry = readdir(home))!=NULL){

        //LOG
        //printf("Processing %s file in process %d\n", dir_entry->d_name, getpid());

        //Reads file
        stat(dir_entry->d_name, &filestat);


        if(strcmp(dir_entry->d_name, ".")==0 || strcmp(dir_entry->d_name, "..") ==0){
            continue;
        }
        

        //Verifies if is directory    
        if(S_ISDIR(filestat.st_mode)){
            
            pid_t pid = fork();

            if(pid == 0){        //Child process
                
                char *new_arg[3];
                
                //copy_values(new_argv,argv,argc);

                sprintf(new_arg[2], "%s", dir_entry->d_name);
                
                if(strlen(argv[0]) == 10){
                    new_arg[0] = "../simpledu";
                }else {
                    sprintf(new_arg[0], "../%s", argv[0]);
                }

                new_arg[1] = "-l";

                //printf("Going to %s using prog %s with flag %s\n", new_arg[2], new_arg[0], new_arg[1]);    

                execvp(new_arg[0], new_arg);
                printf("Error in executing recursive simpledu to %s\n", new_arg[2]);
                exit(3);
            }/*else{              //Parent process
                
                if(putpid(pids, MAX_PIDS, pid)!=0){
                    printf("Maximum number of forks exceeded\n");
                    exit(5);
                }
            }*/
           
        }
       
        printf("%ld\t%s\n", filestat.st_size/1024,dir_entry->d_name);

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
    
    //printf("hello\n");
    //Waiting for all childs to end (needs to change!)
    //while(wait(&status)!=-1);
    int ret;
    while ((ret = wait(&status)) > 0);

    return 0;
}

void copy_values(char *dest[], char *copy[], int n){
    for (int i =0; i<n; i++){
        strcpy(dest[i], copy[i]);
    }
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
