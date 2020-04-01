#include <stdio.h>

void print_help(void){
    printf("-a , --all \t\t- a informação exibida diz respeito também a ficheiros\n");
    printf("-b , --bytes \t\t- apresenta o número real de bytes de dados (ficheiros) ou alocados (diretórios)\n");
    printf("-B , --block-size=SIZE \t- define o tamanho (bytes) do bloco para efeitos de representação\n");
    printf("-l , --count-links \t- contabiliza múltiplas vezes o mesmo ficheiro\n");
    printf("-L , --dereference \t- segue links simbolicos \n");
    printf(" -S , --separate-dirs \t- a informação exibida não inclui o tamanho dos subdiretorios\n");
    printf("--max-depth=N \t\t- limita a informação exibida a N (0,1, ...) níveis de profundidade de diretorios\n ");
}

void transformToString(char *result, char *array[], int n){
    int i=0;
    while(array[i] != NULL && i<n){
        if (i==0) 
            sprintf(result, "%s", array[i]);
        else
            sprintf(result, "%s %s", result, array[i]);
        i++;
    }
}

void copy_values(char *dest[], char *copy[], int n){
    for (int i =0; i<n; i++){
        strcpy(dest[i], copy[i]);
    }
}