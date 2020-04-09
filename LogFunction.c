
#include "LogFunction.h"

void log_create(char *logFileName, char *argv[], int argc){
    //Write to LogFile
    if(logFileName!=NULL){

        int fdLog = open(logFileName,O_WRONLY | O_CREAT | O_APPEND, 0644);
    
        
        double time = ( (double) clock() / (double) (CLOCKS_PER_SEC) )*1000;

        char buff[MAX_LINE], cmdString[MAX_LINE];

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
    
    char buff[MAX_LINE];
    
    sprintf(buff, "%.4f - %d - EXIT - %d\n", time, getpid(), exit_status);
    
    write(fdLog, buff, strlen(buff));
    close(fdLog);

    exit(exit_status);
    
}

void log_pipe(char *logFileName, char *msg, char r){

    if(logFileName == NULL) return;

    int fdLog = open(logFileName,O_WRONLY | O_CREAT | O_APPEND, 0644);

    double time = ( (double) clock() / (double) (CLOCKS_PER_SEC) )*1000;

    char buff[MAX_LINE];
    
    if(r == 'r')
        sprintf(buff, "%.4f - %d - RECV_PIPE - %s\n", time, getpid(), msg);
    else
        sprintf(buff, "%.4f - %d - SEND_PIPE - %s\n", time, getpid(), msg);

    write(fdLog, buff, strlen(buff));

    close(fdLog);
}

void log_entry(char *logFileName, long int msg){

    if(logFileName == NULL) return;

    int fdLog = open(logFileName,O_WRONLY | O_CREAT | O_APPEND, 0644);

    double time = ( (double) clock() / (double) (CLOCKS_PER_SEC) )*1000;

    char buff[MAX_LINE];
    
    sprintf(buff, "%.4f - %d - ENTRY - %ld\n", time, getpid(), msg);

    write(fdLog, buff, strlen(buff));

    close(fdLog);
}

void log_write(char *logFileName, long int msg){
    if(logFileName == NULL) return;

    int fdLog = open(logFileName,O_WRONLY | O_CREAT | O_APPEND, 0644);

    char temp[MAX_LINE];
    sprintf(temp, "%ld\n", msg);

    write(fdLog, temp, strlen(temp));

    close(fdLog);
    
}

void log_signal(char *logFileName, char *msg, char r){
    if(logFileName == NULL) return;
    
    int fdLog = open(logFileName,O_WRONLY | O_CREAT | O_APPEND, 0644);

     double time = ( (double) clock() / (double) (CLOCKS_PER_SEC) )*1000;

    char buff[MAX_LINE];
    
    if(r == 'r')
        sprintf(buff, "%.4f - %d - RECV_SIGNAL - %s\n", time, getpid(), msg);
    else
        sprintf(buff, "%.4f - %d - SEND_SIGNAL - %s\n", time, getpid(), msg);

    write(fdLog, buff, strlen(buff));

    close(fdLog);
}

void transformToString(char *result, char *array[], int n){
    int i=0;
    while(array[i] != NULL && i<n){
        if (i==0) 
            sprintf(result, "%s", array[i]);
        else
            sprintf(result, "%s %s", result, array[i]);
        i++;
    }
}
