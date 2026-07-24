#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include "conta.h"

//funções da Lista e NO
No *alocar_no(Conta *cliente){

    No *novo = calloc(1,sizeof(No));

    if(!novo) return NULL;

    novo->conta = cliente;

    return novo;
}


int inserir_na_lista(Lista *lista,Conta *cliente){

    No *novo = alocar_no(cliente);

    if(!novo) return-1;

    if(lista->cabeca == NULL){

        lista->cabeca = lista->cauda = novo;
        return 0;
    }

    lista->cauda->proximo = novo;
    lista->cauda = novo;

    return 0;
}

void iniciar_lista(Lista *lista){

    *lista = (Lista){0};
}







//funções da Conta
void cadastrando_conta(Conta *conta){

    if(entrada_nome(&conta->nome) == -1){

        printf("Erro ao cadastrar nome.\n");
        return;
    }

    conta->numero = NUMERO_INICIAL_CONTA +1;

}



Conta *criar_conta(){

    Conta *novo = calloc(1,sizeof(Conta));

    if (!novo)
    {
        return NULL;
    }
     return novo;
}

int entrada_nome(char **nome){

    char buffer[500];

    printf("Digite seu nome para cadastro : ");

    fgets(buffer,sizeof(buffer),stdin);
    buffer[strcspn(buffer,"\n")] = '\0';

    *nome = alocar_nome(strlen(buffer)+1);

    if(*nome == NULL){

        printf("Erro na alocacao do ponteiro.\n");
        return -1;
    }

    strcpy(*nome,buffer);

    return 0;

}

char *alocar_nome(size_t tamanho){

    char *novo = malloc(tamanho);

    if(!novo){

        
        return NULL;
    }

    return novo;
}

void liberar_conta(Conta *conta){

    if (conta == NULL)
    return;

    libera_memoria_nome(conta->nome);
    free(conta);
}


void libera_memoria_nome(char *nome){

    free(nome);
}