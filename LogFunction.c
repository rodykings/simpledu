#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>



void log_create(char *logFileName, char *argv[], int argc){
    //Write to LogFile
    if(logFileName!=NULL){

        int fdLog = open(logFileName,O_WRONLY | O_CREAT | O_APPEND, 0644);
    
        
        double time = ( (double) clock() / (double) (CLOCKS_PER_SEC) )*1000;

        char buff[100], cmdString[100];

        transformToString(cmdString, argv, argc);
        
        sprintf(buff, "%.4f - %d - CREATE - %s\n", time, getpid(), cmdString);

        write(fdLog, buff, strlen(buff));
        close(fdLog);   
    }
}



void log_exit(char * logFileName, int exit_status){

    if(logFileName==NULL) exit(exit_status);

    int fdLog = open(logFileName,O_WRONLY | O_CREAT | O_APPEND, 0644);
    
    double time = ( (double) clock() / (double) (CLOCKS_PER_SEC) )*1000;
    
    char buff[100];
    
    sprintf(buff, "%.4f - %d - EXIT - %d\n", time, getpid(), exit_status);
    
    write(fdLog, buff, strlen(buff));
    close(fdLog);

    exit(exit_status);
    
}