#include <stdio.h>  // printf(), perror()
#include <stdlib.h> // exit(), EXIT_FAILURE
#include <string.h> // memset()

#include <unistd.h> // fork(), close(), execve()

#include <sys/types.h>  // Tipos utilizados em sockets
#include <sys/socket.h> // socket(), bind(), listen(), accept()

#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // htons(), inet_addr()

#include <sys/wait.h> // wait(), waitpid()
#include <signal.h>   //signal()

#include <unistd.h> //sleep()

#include "arquivo.h"

void capturar_entrada(char *buffer, size_t tamanho);

int envia_lista(const Lista *lista, int socket_cliente);

int main()
{

    // iniciando lista
    Lista lista;
    iniciar_lista(&lista);

    // iniciando FILE
    FILE *arquivo;

    carregar_contas(&lista, &arquivo);

    // Cria o socket do servidor
    int socket_servidor = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_servidor == -1)
    {

        perror("Erro ao criar socket princial");
        exit(EXIT_FAILURE);
    }

    // Associa o endereço e a porta ao socket
    struct sockaddr_in servidor_addr;

    memset(&servidor_addr, 0, sizeof(servidor_addr));

    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(8080);
    servidor_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_servidor, (struct sockaddr *)&servidor_addr, sizeof(servidor_addr)) == -1)
    {

        perror("Erro ao associar dados ao socket");
        close(socket_servidor);
        exit(EXIT_FAILURE);
    }

    // Coloca o socket em modo de escuta
    if (listen(socket_servidor, 5) == -1)
    {

        perror("Erro ao colocar socket na escuta");
        close(socket_servidor);
        exit(EXIT_FAILURE);
    }

    // Aceita conexões de clientes

    printf("Aguardando cliente conectar.....\n\n");
    int socket_cliente = accept(socket_servidor, NULL, NULL);

    while (1)
    {

        char buffer[1024] = {0};

        if (socket_cliente == -1)
        {

            perror("Erro ao conectar com o cliente");
            continue;
        }

        // Recebe dados do cliente

        int bytes_recebidos = recv(socket_cliente, buffer, sizeof(buffer) - 1, 0);

        // Trata o resultado do recv()
        if (bytes_recebidos == -1)
        {

            perror("Erro ao receber bytes");
            break;
        }
        else if (bytes_recebidos == 0)
        {

            printf("Conexão encerrada.\n");
            break;
        }

        else
        {

            int opcao = atoi(buffer);

            char *mensagem = NULL;

            switch (opcao)
            {
            case 1:
            {

                char nome[1024];

                int bytes_recebidos_cadastro = recv(socket_cliente, nome, sizeof(nome) - 1, 0);

                if (bytes_recebidos_cadastro == -1)
                {

                    perror("Erro ao receber bytes");
                    break;
                }

                nome[bytes_recebidos_cadastro] = '\0';

                

                if(inserindo_conta(&lista,&arquivo,nome,strlen(nome)) == -1){

                    mensagem = "Erro ao cadastrar conta.\n";
                }else{

                    mensagem = "Cadastro realizado com sucesso.\n";
                }

                break;
            }

            case 2:
                mensagem = "Depositando...\n";
                break;
            case 3:
                mensagem = "Sacando...\n";
                break;
            case 4:
                mensagem = "Saldo...\n";
                break;
            case 5:
                envia_lista(&lista, socket_cliente);
                break;
            case 0:
                break;

            default:

                mensagem = "Opcao invalida\n";
                break;
            }

            if (opcao == 0)
            {

                break;
            }

            if (opcao != 5)
            {

                int bytes_enviados = send(socket_cliente, mensagem, strlen(mensagem), 0);

                if (bytes_enviados == -1)
                {

                    perror("Erro ao enviar os dados.\n");
                    break;
                }
            }
        }
    }
    close(socket_cliente);

    close(socket_servidor);

    return 0;
}

void capturar_entrada(char *buffer, size_t tamanho)
{

    printf("Escolha a mensagem: ");

    fgets(buffer, tamanho, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

int envia_lista(const Lista *lista, int socket_cliente)
{

    if (lista == NULL)
    {

        return -1;
    }

    char buffer_envio[1024] = {0};

    No *atual = lista->cabeca;

    while (atual != NULL)
    {
        snprintf(buffer_envio, sizeof(buffer_envio), "%d;%s;%.2f\n",
                 atual->conta->numero,
                 atual->conta->nome,
                 atual->conta->saldo);

        int envio = send(socket_cliente, buffer_envio, strlen(buffer_envio), 0);
        if (envio == -1)
        {
            perror("Erro ao enviar algum dado.\n");
            return -1;
        }

        atual = atual->proximo;
    }

    strcpy(buffer_envio, "FIM\n");

    int envio = send(socket_cliente, buffer_envio, strlen(buffer_envio), 0);
    if (envio == -1)
    {
        perror("Erro ao enviar FIM.\n");
        return -1;
    }
    return 0;
}

// 1001;Maria;900.00