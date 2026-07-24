#ifndef CONTA_H
#define CONTA_H

#define NUMERO_INICIAL_CONTA 1000

typedef struct 
{
    int numero;
    char *nome;
    float saldo;

}Conta;

typedef struct No
{
    Conta *conta;
    struct No *proximo;

}No;

typedef struct 
{
    No *cabeca;
    No *cauda;
}Lista;

//funções da Lista e NO
No *alocar_no(Conta *cliente);

int inserir_na_lista(Lista *lista,Conta *cliente);

void iniciar_lista(Lista *lista);


//funções da Conta
char *alocar_nome(size_t tamanho);

int entrada_nome(char **nome);

void libera_memoria_nome(char *nome);

void liberar_conta(Conta *conta);

Conta *criar_conta(void);

void cadastrando_conta(Conta *conta);





#endif

