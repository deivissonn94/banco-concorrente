#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arquivo.h"

int abrir_arquivo_leitura(FILE **arquivo);

int abrir_arquivo_escrita(FILE **arquivo);

int gravar_dados(const Conta *conta,FILE *arquivo);

int ler_dados(Conta *cliente,FILE *arquivo);

void fechar_arquivo(FILE *arquivo);

int main()
{
    FILE *arquivo = NULL;


    //salavando dados

    Conta *c1 = criar_conta();
    c1->numero = 1001;
    c1->nome = "João Silva";
    c1->saldo = 1500.50;

    Conta *c2 = criar_conta();
    c2->numero = 1002;
    c2->nome = "Maria Oliveira";
    c2->saldo = 2750.00;

    Conta *c3 = criar_conta();
    c3->numero = 1003;
    c3->nome = "Carlos Souza";
    c3->saldo = 980.75;

    Conta *c4 = criar_conta();
    c4->numero = 1004;
    c4->nome = "Ana Pereira";
    c4->saldo = 5200.30;

    Conta *c5 = criar_conta();
    c5->numero = 1005;
    c5->nome = "Lucas Santos";
    c5->saldo = 340.90;

    Conta *contas[5] = {c1,c2,c3,c4,c5};

    if(abrir_arquivo_escrita(&arquivo) == -1){

        printf("Erro ao abrir arquivo !");
        return -1;
    }

    for(int i =0;i<5;i++){

        if(gravar_dados(contas[i],arquivo) == -1){

            printf("Erro ao gravar dados.\n");
            return -1;
        }
    }

    fechar_arquivo(arquivo);



    //clientes que iram receber dados 
    Conta *c1_recebedor = criar_conta();
    Conta *c2_recebedor = criar_conta();
    Conta *c3_recebedor = criar_conta();
    Conta *c4_recebedor = criar_conta();
    Conta *c5_recebedor = criar_conta();

    Conta *clientes_r[5] ={c1_recebedor,c2_recebedor,c3_recebedor,c4_recebedor,c5_recebedor};

    abrir_arquivo_leitura(&arquivo);

    int contador = 0;

    while(ler_dados(clientes_r[contador],arquivo) != 0){

        printf("Numero : %d \n",clientes_r[contador]->numero);
        printf("Nome : %s \n",clientes_r[contador]->nome);
        printf("Saldo : %2.f \n\n\n",clientes_r[contador]->saldo);

        contador++;
    }

    fechar_arquivo(arquivo);

    /*

    for(int i = 0;i<5;i++){

        if(ler_dados(clientes_r[i],arquivo) == -1){

            printf("Erro ao receber os dados.\n");
            return -1;
        }

        printf("Numero : %d \n",clientes_r[i]->numero);
        printf("Nome : %s \n",clientes_r[i]->nome);
        printf("Saldo : %2.f \n\n\n",clientes_r[i]->saldo);
    }

    fechar_arquivo(arquivo);

    
    printf("Numero : %d \n",cliente_recebedor->numero);
    printf("Nome : %s \n",cliente_recebedor->nome);
    printf("Saldo : %f \n",cliente_recebedor->saldo);
    */

    return 0;
}

void fechar_arquivo(FILE *arquivo){

    fclose(arquivo);
}

int abrir_arquivo_escrita(FILE **arquivo){

    *arquivo = fopen("arquivo.dat", "ab");

    if (!(*arquivo))
        return -1;

    return 0;

}


int abrir_arquivo_leitura(FILE **arquivo){

    *arquivo = fopen("arquivo.dat", "rb");

    if (!(*arquivo))
        return -1;

    return 0;

}

int ler_dados(Conta *cliente,FILE *arquivo)
{

   

    if((fread(&cliente->numero, sizeof(cliente->numero), 1, arquivo) == 0) )
        return 0;

    size_t tamanho_nome;

    fread(&tamanho_nome, sizeof(size_t), 1, arquivo);

    cliente->nome = alocar_nome(tamanho_nome);

    if(!cliente->nome) return -1;

    fread(cliente->nome, sizeof(*cliente->nome), tamanho_nome, arquivo);

    fread(&cliente->saldo, sizeof(cliente->saldo), 1, arquivo);


    return 1;
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
