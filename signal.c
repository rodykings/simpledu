#include "signal.h"
/* 07-04-2020 14:54 ©Rodrigo e Deborah */
//When parent receive SIGINT sends SIGSTOP to childs
void signalHandlerInt(int signal){
    char *logForSignal;
    log_signal(logForSignal,"SIGINT",'r');
    write(STDOUT_FILENO, "Do you desire to terminate ou continue running?\n", 48);
    killpids(childpids, MAX_PIDS, SIGSTOP);
    log_signal(logForSignal,"SIGINT",'s');
    //received_int = true;
    
    sigset_t mask;
    sigfillset(&mask);
    sigdelset(&mask, SIGTERM);
    sigdelset(&mask, SIGCONT);
    sigsuspend(&mask);
    
}

void signalHandlerCont(int signal){
    char *logForSignal;
    //if(!received_int) return;
    log_signal(logForSignal, "SIGCONT",'r');

    killpids(childpids, MAX_PIDS, SIGCONT);
    log_signal(logForSignal, "SIGCONT",'s');
    //received_int = false;

}

void signalHandlerTerm(int signal){
    char *logForSignal;
    log_signal(logForSignal, "SIGTERM",'r');
    killpids(childpids, MAX_PIDS, SIGTERM);
    log_signal(logForSignal, "SIGTERM",'s');
    log_exit(logForSignal, 0);
}


void installSignalHandlers(pid_t *pids){
    //set signal handler
    
    childpids = pids;

    actInt.sa_handler = signalHandlerInt;
    sigemptyset(&actInt.sa_mask);
    actInt.sa_flags = 0;
    sigaction(SIGINT, &actInt, NULL);

    
    actCont.sa_handler = signalHandlerCont;
    sigemptyset(&actCont.sa_mask);
    actCont.sa_flags = 0;
    sigaction(SIGCONT, &actCont, NULL);

    
    actTerm.sa_handler = signalHandlerTerm;
    sigemptyset(&actTerm.sa_mask);
    actTerm.sa_flags = 0;
    sigaction(SIGTERM, &actTerm, NULL);
}
