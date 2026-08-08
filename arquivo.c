#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arquivo.h"

// outras funcoes

int sacar(Lista *lista, int numero_conta, float saque)
{

    if (lista == NULL)
    {

        return -1;
    }

    Conta *buscada = buscar_conta(lista, numero_conta);
    if (buscada == NULL)
    {

        return -1;
    }

    if (buscada->saldo - saque < 0)
    {

        return -2;
    }

    buscada->saldo -= saque;

    return gravando_dados_apos_alteracoes(lista);
}

int depositar(Lista *lista, int numero_conta, float deposito)
{

    if (lista == NULL)
    {

        return -1;
    }

    Conta *buscada = buscar_conta(lista, numero_conta);
    if (buscada == NULL)
    {

        return -1;
    }

    buscada->saldo += deposito;

    return gravando_dados_apos_alteracoes(lista);
}

int inserindo_conta(Lista *lista, const char *nome, size_t tamanho_nome)
{

    Conta *nova = criar_conta();
    if (!nova)
    {

        return -1;
    }

    cadastrando_conta(nova, nome, tamanho_nome, lista);

    if (inserir_na_lista(lista, nova) == -1)
    {
        liberar_conta(nova);
        return -1;
    }

    // persistencia
    return gravar_dados_no_final(nova);
}

int transferencia(Lista *lista, int n_conta_origem, int n_conta_destino, float valor)
{

    if (!lista)
    {

        return -1;
    }

    if (valor <= 0)
        return -3;

    if (n_conta_destino == n_conta_origem)
        return -4;

    Conta *conta_origem = buscar_conta(lista, n_conta_origem);
    if (!conta_origem)
        return -1;

    Conta *conta_destino = buscar_conta(lista, n_conta_destino);
    if (!conta_destino)
        return -1;

    if (conta_origem->saldo - valor < 0)
    {

        return -2;
    }

    conta_origem->saldo -= valor;

    conta_destino->saldo += valor;

    int resultado = gravando_dados_apos_alteracoes(lista);

    if (resultado == -1)
    {

        conta_origem->saldo += valor;
        conta_destino->saldo -= valor;
    }

    return resultado;
}

//--------------------------------------------------------
int gravando_dados_apos_alteracoes(Lista *lista)
{

    if (!lista)
        return -1;

    FILE *arquivo = fopen("arquivo.dat", "wb");

    if (arquivo == NULL)
        return -1;

    No *atual = lista->cabeca;

    while (atual != NULL)
    {
        if (gravar_dados(atual->conta, arquivo) != 0)
        {

            fclose(arquivo);
            return -1;
        }

        atual = atual->proximo;
    }

    fclose(arquivo);

    return 0;
}

int carregar_contas(Lista *lista)
{

    if (lista == NULL)
        return -1;

    FILE *arquivo = fopen("arquivo.dat", "rb");

    if (!arquivo)
        return -1;

    while (1)
    {

        Conta *aux = criar_conta();
        if (!aux)
            return -1;

        int resultado;

        resultado = ler_dados(aux, arquivo);

        if (resultado == 1)
        {

            if (aux->numero >= lista->proximo_numero)
            {

                lista->proximo_numero = aux->numero + 1;
            }

            if (inserir_na_lista(lista, aux) == -1)
            {

                liberar_conta(aux);
                fclose(arquivo);
                return -1;
            }
        }
        else
        {

            liberar_conta(aux);
            fclose(arquivo);
            return resultado;
        }
    }
}

int ler_dados(Conta *cliente, FILE *arquivo)
{

    if ((fread(&cliente->numero, sizeof(cliente->numero), 1, arquivo) == 0))
        return 0;

    size_t tamanho_nome;

    fread(&tamanho_nome, sizeof(size_t), 1, arquivo);

    cliente->nome = alocar_nome(tamanho_nome);

    if (!cliente->nome)
        return -1;

    fread(cliente->nome, sizeof(*cliente->nome), tamanho_nome, arquivo);

    fread(&cliente->saldo, sizeof(cliente->saldo), 1, arquivo);

    return 1;
}

int gravar_dados_no_final(const Conta *conta)
{

    FILE *arquivo = fopen("arquivo.dat", "ab");
    if (!arquivo)
        return -2;

    int resultado = gravar_dados(conta,arquivo);

    fclose(arquivo);

    return resultado;
}

int gravar_dados(const Conta *conta,FILE *arquivo)
{

    fwrite(&conta->numero, sizeof(conta->numero), 1, arquivo);

    size_t tamanho_nome = strlen(conta->nome) + 1;

    fwrite(&tamanho_nome, sizeof(tamanho_nome), 1, arquivo);

    fwrite(conta->nome, tamanho_nome, 1, arquivo);

    fwrite(&conta->saldo, sizeof(conta->saldo), 1, arquivo);



    return 0;
}
