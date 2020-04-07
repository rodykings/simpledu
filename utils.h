#include <stdio.h>
#include <string.h>
#include <stdbool.h>


void print_help(void);

void copy_values(char *dest[], char *copy[], int n);

int extract_number(const char *line, long int *result);

bool has_bar(const char *path);