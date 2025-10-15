#include <stdio.h>
#include <string.h>

typedef struct { 
     char nome [30];
     char cor [10];
     int tropas;
} Territorio; 

int main () {
    //vetor para armazenar ate 5 territorios
    Territorio territorios[5];

    //laço para cadastro dos 5 territorios
    for (int i = 0; i < 5; i++) {
        printf("\n Cadastro do Territorio %d \n" , i + 1);

        //leitura do nome do territorio
        printf("\n Digite o nome do Territorio: \n ");
        scanf("\n %[^\n]", territorios[i].nome); //le até o enter

        //leitura da cor do exercito 
        printf("\n Digite a cor do exercito: \n");
        scanf("\n %[^\n]", territorios[i].cor);

        //leitura do numero de tropas 
        printf("\n Digite a quantidade de tropas: \n");
        scanf("\n %d", &territorios[i].tropas);
    }

   //exibição dos dados
   printf("\n============================\n");
   printf("LISTA DE TERRITORIOS\n");
   printf("\n============================\n");

     for (int i = 0; i < 5; i++){
        printf("\nTerritorio %d: \n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exercito: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
     }

      printf("\n======================================\n");
      printf("Cadastro concluído com sucesso!\n");
      printf("======================================\n");

      return 0;

}

