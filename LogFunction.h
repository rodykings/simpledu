#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

#define MAX_LINE 4096


void log_create(char *logFileName, char *argv[], int argc);

void log_exit(char * logFileName, int exit_status);

void log_pipe(char *logFileName, char *msg, char r);

void log_entry(char *logFileName, long int msg);

void log_write(char *logFileName, long int msg);

void log_signal(char *logFileName, char *msg, char r);

void transformToString(char *result, char *array[], int n);