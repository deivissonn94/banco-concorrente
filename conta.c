#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include "conta.h"

//funções da Lista e NO
void liberar_lista(Lista *lista){

    if (lista == NULL)
        return;

    No *atual = lista->cabeca;

    while (atual != NULL)
    {   
        No *aux = atual;

        atual= atual->proximo;
        
        liberar_conta(aux->conta);
        free(aux);



    }
    
    lista->cabeca =lista->cauda =NULL;
}


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
    lista->proximo_numero = NUMERO_INICIAL_CONTA;
}


Conta *buscar_conta(Lista *lista,int numero){

    if(!lista){

        return NULL;
    }

    No *atual = lista->cabeca;

    while (atual != NULL)
    {
        if(atual->conta->numero == numero){

            return atual->conta;

        }

        atual = atual->proximo;
    }

    return NULL;
    
}

void listar_contas(const Lista *lista){

    No *atual = lista->cabeca;

    while (atual != NULL)
    {
        printf("Numero : %d \n",atual->conta->numero);
        printf("Nome : %s \n",atual->conta->nome);
        printf("Saldo : %.2f \n\n",atual->conta->saldo);

        atual = atual->proximo;
    }

    
}





//funções da Conta
void cadastrando_conta(Conta *conta,const char *buffer,size_t tamanho_nome,Lista *lista){

    if(conta == NULL){

        return;
    }


    conta->nome = alocar_nome(tamanho_nome);
    strcpy(conta->nome,buffer);
    conta->numero = lista->proximo_numero++;

}



Conta *criar_conta(){

    Conta *novo = calloc(1,sizeof(Conta));

    if (!novo)
    {
        return NULL;
    }
     return novo;
}



char *alocar_nome(size_t tamanho){

    char *novo = malloc(tamanho + 1);

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