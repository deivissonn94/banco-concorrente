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

int main()
{

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
    while (1)
    {

        char buffer[1024];

        int socket_cliente = accept(socket_servidor, NULL, NULL);

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

            perror("Erro ao receber bytes :");
        }
        else if (bytes_recebidos == 0)
        {

            printf("Conexão encerrada.\n");
        }
        else
        {

            buffer[bytes_recebidos] = '\0';
            printf("Buffer : %s\n", buffer);
        }

        close(socket_cliente);
    }

    close(socket_servidor);

    return 0;
}