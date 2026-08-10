#include <stdio.h> // printf(), scanf(), fgets()

#include <stdlib.h> // exit(), malloc(), free()

#include <string.h> // memset(), strlen(), strcmp(), memcpy()

#include <winsock2.h> // WSAStartup(), WSACleanup(), socket(), connect(),
                      // send(), recv(), closesocket()

#include <ws2tcpip.h> // inet_pton(), inet_ntop(), getaddrinfo()

#define MAX_CAMPOS 5

int parser_campos(char *buffer, char **campos);

void exibir_menu(void);

void capturar_entrada(char *buffer, size_t tamanho);

int entrada_nome(char **nome);

char *alocar_nome(size_t tamanho);

int main()
{

    WSADATA wsa;

    // iniciando Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {

        printf("Erro ao iniciar o winsock. Codigo: %d\n", WSAGetLastError());
        return -1;
    }

    // Iniciando SOCKET
    SOCKET socket_cliente = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (socket_cliente == INVALID_SOCKET)
    {

        printf("Falha ao criar socket cliente.  Codigo: %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }

    // Configurando porta e endereço para o socket
    struct sockaddr_in servidor_cliente;

    memset(&servidor_cliente, 0, sizeof(servidor_cliente));

    servidor_cliente.sin_family = AF_INET;
    servidor_cliente.sin_port = htons(8080);
    servidor_cliente.sin_addr.s_addr = inet_addr("192.168.2.114");

    // Conectando ao servidor linux
    if (connect(socket_cliente, (struct sockaddr *)&servidor_cliente, sizeof(servidor_cliente)) == SOCKET_ERROR)
    {

        printf("Erro ao conectar ao servidor. Codigo: %d\n", WSAGetLastError());
        closesocket(socket_cliente);
        WSACleanup();
        return -1;
    }

    // Enviando dados ao servidor

    int conectado = 1;

    while (1)
    {

        char buffer[1024] = {0};

        char buffer_erro[1024] = {0};

        exibir_menu();

        capturar_entrada(buffer, sizeof(buffer));

        strcpy(buffer_erro, buffer);

        char *campos_erro[MAX_CAMPOS] = {0};

        parser_campos(buffer_erro, campos_erro);

        int opcao_erro = atoi(campos_erro[0]);

        if (opcao_erro == 0 && conectado == 0)
        {

            printf("Conexao encerrado com servidor desconectado.\n");
            break;
        }

        int bytes_enviados = send(socket_cliente, buffer, strlen(buffer), 0);

        if (bytes_enviados == SOCKET_ERROR)
        {
            int erro = WSAGetLastError();

            if (erro == 10053)
            {
                conectado = 0;
                printf("Servidor desconectado.\n\n");
                continue;
            }
            else
            {

                printf("Erro ao enviar a mensagem. Codigo: %d\n", erro);
                closesocket(socket_cliente);
                WSACleanup();
                return -1;
            }
        }

        printf("Bytes enviados: %d\n", bytes_enviados);

        char *campos[MAX_CAMPOS] = {0};

        parser_campos(buffer, campos);

        int opcao = atoi(campos[0]);

        if (opcao == 4)
        {

            int bytes_recebidos = recv(socket_cliente, buffer, sizeof(buffer) - 1, 0);

            if (bytes_recebidos == SOCKET_ERROR)
            {
                int erro = WSAGetLastError();

                if (erro == 10053)
                {
                    conectado = 0;
                    printf("Servidor desconectado.\n\n");
                    continue;
                }
                else
                {

                    printf("Erro ao receber. Codigo: %d\n", erro);
                }

                break;
            }

            buffer[bytes_recebidos] = '\0';

            char *campos_r[4] = {0};

            parser_campos(buffer, campos_r);

            if (strcmp(campos_r[0], "ERRO") == 0)
            {

                printf("%s : %s\n", campos_r[0], campos_r[1]);
            }
            else if (strcmp(campos_r[0], "OK") == 0)
            {

                printf("Conta : %s\n", campos_r[1]);
                printf("Nome : %s\n", campos_r[2]);
                printf("Saldo : %s\n\n", campos_r[3]);
            }

            printf("Pressione enter para continuar.....");
            getchar();

            continue;
        }

        if (opcao == 5)
        {

            int loop = 1;

            while (loop)
            {
                int bytes_recebidos = recv(socket_cliente, buffer, sizeof(buffer) - 1, 0);

                if (bytes_recebidos == SOCKET_ERROR)
                {
                    int erro = WSAGetLastError();

                    printf("Erro ao receber. Codigo: %d\n", erro);
                    break;
                }

                buffer[bytes_recebidos] = '\0';

                char *linha = strtok(buffer, "\n");

                while (linha != NULL)
                {
                    if (strcmp(linha, "FIM") == 0)
                    {

                        loop = 0;

                        printf("\nFIM DA LISTA\n\n");
                        break;
                    }

                    char *campos[3] = {0};

                    int contador = 0;
                    char *marcador = linha;

                    for (int i = 0; linha[i] != '\0'; i++)
                    {

                        if (linha[i] == ';')
                        {

                            linha[i] = '\0';

                            campos[contador++] = marcador;
                            marcador = &linha[i + 1];
                        }
                    }

                    campos[contador] = marcador;

                    // printf("LINHA : %s\n", linha);
                    printf("Conta : %s\n", campos[0]);
                    printf("Nome : %s\n", campos[1]);
                    printf("Saldo : %s\n\n", campos[2]);

                    linha = strtok(NULL, "\n");
                }
            }

            printf("Pressione enter para continuar.....");
            getchar();

            continue;
        }

        int bytes_recebidos = recv(socket_cliente, buffer, sizeof(buffer) - 1, 0);

        if (bytes_recebidos == SOCKET_ERROR)
        {
            int erro = WSAGetLastError();
            if (erro == 10053)
            {
                conectado = 0;
                printf("servidor desconectado : %d\n", erro);
                printf("Pressione enter para continuar.....");
                getchar();
            }
            else
            {

                printf("Erro ao receber. Codigo: %d\n", erro);
                break;
            }
        }
        else if (bytes_recebidos == 0)
        {
            if (opcao == 0)
            {

                printf("Conexao encerrada normalmente.\n");
                break;
            }

            conectado == 0;

            printf("Servidor desconectado.\n");
        }
        else
        {

            buffer[bytes_recebidos] = '\0';
            printf("Recebido do servidor : %s\n", buffer);
            printf("Pressione enter para continuar.....");
            getchar();
        }
    }
    closesocket(socket_cliente);
    WSACleanup();

    return 0;
}

void exibir_menu(void)
{

    printf("\n");
    printf("=====================================\n");
    printf("        BANCO CONCORRENTE\n");
    printf("=====================================\n");
    printf("1 - Cadastrar conta\n");
    printf("2 - Depositar\n");
    printf("3 - Sacar\n");
    printf("4 - Consultar saldo\n");
    printf("5 - Listar contas\n");
    printf("6 - Transferir\n");
    printf("0 - Sair\n");
    printf("=====================================\n\n");
}

void capturar_entrada(char *buffer, size_t tamanho)
{

    printf("Escolha uma opcao: ");

    fgets(buffer, tamanho, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

int entrada_nome(char **nome)
{

    char buffer[500];

    printf("Digite seu nome para cadastro : ");

    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    *nome = alocar_nome(strlen(buffer) + 1);

    if (*nome == NULL)
    {

        printf("Erro na alocacao do ponteiro.\n");
        return -1;
    }

    strcpy(*nome, buffer);

    return 0;
}

char *alocar_nome(size_t tamanho)
{

    char *novo = malloc(tamanho + 1);

    if (!novo)
    {

        return NULL;
    }

    return novo;
}

int parser_campos(char *buffer, char **campos)
{

    char *delimitador = buffer;
    int c = 0;

    for (int i = 0; buffer[i] != '\0'; i++)
    {

        if (buffer[i] == ';')
        {

            buffer[i] = '\0';

            campos[c++] = delimitador;
            delimitador = &buffer[i + 1];
        }
    }

    campos[c] = delimitador;

    return c + 1;
}
