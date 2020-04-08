#include "utils.h"

void print_help(void){
    printf("-a , --all \t\t- a informação exibida diz respeito também a ficheiros\n");
    printf("-b , --bytes \t\t- apresenta o número real de bytes de dados (ficheiros) ou alocados (diretórios)\n");
    printf("-B , --block-size=SIZE \t- define o tamanho (bytes) do bloco para efeitos de representação\n");
    printf("-l , --count-links \t- contabiliza múltiplas vezes o mesmo ficheiro\n");
    printf("-L , --dereference \t- segue links simbolicos \n");
    printf(" -S , --separate-dirs \t- a informação exibida não inclui o tamanho dos subdiretorios\n");
    printf("--max-depth=N \t\t- limita a informação exibida a N (0,1, ...) níveis de profundidade de diretorios\n ");
}


void copy_values(char *dest[], char *copy[], int n){
    for (int i =0; i<n; i++){
        strcpy(dest[i], copy[i]);
    }
}

int extract_number(const char *line, long int *result){
    
    if(line == NULL || result == NULL)  return -1;

    char copy[MAX_LINE];
    strcpy(copy,line);

    long int res=0, temp=0;
    
    char *response1, *response2;
    response1 = strtok(copy,"\t");
    
    if(sscanf(response1, "%ld", &temp) == EOF) return -1;
    
    res+=temp;
    
    response2 = strtok(NULL, "\n");
    
    while((response1 = strtok(NULL, "\t"))!=NULL){

        if(sscanf(response1, "%ld", &temp) == EOF) return -1;
        res+=temp;

        response2 = strtok(NULL, "\n");
    }

    *result = temp;

    return 0;
}

void fillpids(pid_t *a, int n){
    for (int i = 0; i<n; i++){
        a[i]= (pid_t) 0;
    }
    a[n-1]= (pid_t) -1;
}

int putpid(pid_t *a, int n, pid_t pid){
    int i=0;
    while(a[i]!=-1){
        if(a[i]==0){
            a[i] = pid;
            return 0;
        }
        i++;
    }
    return 1;
}


void killpids(pid_t *a, int n, int signo){
    for (int i = 0; i<n; i++){
        if(a[i] != -1 && a[i] != 0)
            kill(a[i], signo);
    }
}