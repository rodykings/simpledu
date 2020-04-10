
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

#include <signal.h>

#include "LogFunction.h"
#include "utils.h"


pid_t *childpids;

struct sigaction actInt;
struct sigaction actCont;
struct sigaction actTerm;

void installSignalHandlers(pid_t *pids);

void signalHandlerInt(int signal);

void signalHandlerCont(int signal);

void signalHandlerTerm(int signal);

void load_log(char * filename);