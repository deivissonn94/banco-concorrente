#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "conta.h"

int gravar_dados_no_final(const Conta *conta);

int ler_dados(Conta *cliente, FILE *arquivo);

int carregar_contas(Lista *lista);

int gravando_dados_apos_alteracoes(Lista *lista);

int gravar_dados(const Conta *conta,FILE *arquivo);

// não e exclusivo
int inserindo_conta(Lista *lista, const char *nome, size_t tamanho_nome);

int depositar(Lista *lista, int numero_conta, float deposito);

int sacar(Lista *lista, int numero_conta, float saque);

int transferencia(Lista *lista, int n_conta_origem, int n_conta_destino, float valor);

#endif