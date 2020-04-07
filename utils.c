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

    char copy[256];
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

    *result = res;

    return 0;
}