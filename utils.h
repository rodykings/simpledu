#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <wait.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>

void print_help(void);

void copy_values(char *dest[], char *copy[], int n);

int extract_number(const char *line, long int *result);

void fillpids(pid_t *a, int n);

int putpid(pid_t *a, int n, pid_t pid);

void killpids(pid_t *a, int n);