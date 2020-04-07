#include <stdbool.h>
#include <stdio.h>
#include <string.h>


/*  Definição da struct que contém todas as flags
    A presença das flags está definida com as variáveis bool
    Algumas flags vêm com números associados, justificando-se assim as variaveis de tipo int
*/
struct flags
{
    bool all, bytes, block_size, deref, sep_dirs, max_depth, clinks;
    int nbytes, depth_level;
};

int fill_flags(struct flags *scpflags, char * argv[], int argc, int *pathPos, int * dethPos);

void print_flags(struct flags f);