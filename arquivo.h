#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "conta.h"


int abrir_arquivo_leitura(FILE **arquivo);

int abrir_arquivo_escrita(FILE **arquivo);

int gravar_dados(const Conta *conta,FILE *arquivo);

int ler_dados(Conta *cliente,FILE *arquivo);

void fechar_arquivo(FILE *arquivo);

int carregar_contas(Lista *lista,FILE **arquivo);

int gravando_dados_apos_alteracoes(Lista *lista);

//não e exclusivo 
int inserindo_conta(Lista *lista,FILE **arquivo,const char *nome,size_t tamanho_nome);

int depositar(Lista *lista,int numero_conta,float deposito);

int sacar(Lista *lista,int numero_conta,float saque);

#endif