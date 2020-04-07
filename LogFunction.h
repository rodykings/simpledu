#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>


void log_create(char *logFileName, char *argv[], int argc);

void log_exit(char * logFileName, int exit_status);

void transformToString(char *result, char *array[], int n);