#include <stdio.h> // printf(), scanf(), fgets()

#include <stdlib.h> // exit(), malloc(), free()

#include <string.h> // memset(), strlen(), strcmp(), memcpy()

#include <winsock2.h> // WSAStartup(), WSACleanup(), socket(), connect(),
                      // send(), recv(), closesocket()

#include <ws2tcpip.h> // inet_pton(), inet_ntop(), getaddrinfo()

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
    
    if(socket_cliente == INVALID_SOCKET){

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

    char mensagem_teste[] = "Mensagem enviada";

    int bytes_enviados = send(socket_cliente, mensagem_teste, strlen(mensagem_teste), 0);

    if (bytes_enviados == SOCKET_ERROR)
    {

        printf("Erro ao enviar a mensagem. Codigo: %d\n", WSAGetLastError());
        closesocket(socket_cliente);
        WSACleanup();
        return -1;
    }
    else
    {

        printf("Mensagem enviada com sucesso!\n");
    }

    closesocket(socket_cliente);
    WSACleanup();

    return 0;
}