#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

struct args
{
    char *path;
    bool all, bytes, block_size, deref, sep_dirs, max_depth, clinks;
    int nbytes, depth_level;
};

int fill_args(struct args *args, char * argv[], int argc);

void print_args(struct args *args);

void verify_args(char * argv[], int argc);