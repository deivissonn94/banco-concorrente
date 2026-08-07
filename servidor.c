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

#include <ctype.h>
#include "arquivo.h"

#define MAX_CAMPOS 5

int valida_numero(char *buffer);

int valida_float(char *valor);

void capturar_entrada(char *buffer, size_t tamanho);

int envia_lista(const Lista *lista, int socket_cliente);

int parser_campos(char *buffer, char **campos);

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

        printf("recv retornou %d\n", bytes_recebidos);

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
            buffer[bytes_recebidos] = '\0';
            printf("Recebido: %s\n", buffer);

            char *campos[MAX_CAMPOS] = {0};

            int quantidade = parser_campos(buffer, campos);

            printf("opcao : %s\n", campos[0]);
            printf("Nome : %s\n", campos[1]);
            printf("Saldo : %s\n", campos[2]);
            printf("Conta : %s\n\n", campos[3]);

            int opcao = atoi(campos[0]);

            char *mensagem = NULL;

             char buffer_envio[1024] = {0};


            switch (opcao)
            {
            case 1:
            {
                if(quantidade != 2 || campos[1] == NULL || strlen(campos[1]) == 0 ){

                    mensagem = "Parametros enviados estao errados , ou incompletos.\n";

                    break;
                }
                if (inserindo_conta(&lista, &arquivo, campos[1], strlen(campos[1])) == -1)
                {

                    mensagem = "Erro ao cadastrar conta.\n";
                }
                else
                {

                    mensagem = "Cadastro realizado com sucesso.\n";
                }

                break;
            }

            case 2:
                {   

                    if( quantidade != 3 ||
                        campos[1] == NULL ||
                        strlen(campos[1]) == 0 ||
                        (valida_numero(campos[1]) == 0) ||
                        campos[2] == NULL ||
                        strlen(campos[2]) == 0 ||
                        (valida_float(campos[2]) == 0))
                        {

                            mensagem = "Parametros enviados estao errados , ou incompletos.\n";

                            break;


                    }

                    int num_conta = atoi(campos[1]);
                    float valor = atof(campos[2]);

                    if(depositar(&lista,num_conta,valor) == 0){

                        mensagem = "Desposito realizado com sucesso .\n\n";
                    }else{

                        mensagem = "Falha ao realizar o deposito .\n\n";
                    }

                    break;
                }
                

                
                
            case 3:
                {   

                    if( quantidade != 3 ||
                        campos[1] == NULL ||
                        strlen(campos[1]) == 0 ||
                        (valida_numero(campos[1]) == 0) ||
                        campos[2] == NULL ||
                        strlen(campos[2]) == 0 ||
                        (valida_float(campos[2]) == 0))
                        {

                            mensagem = "Parametros enviados estao errados , ou incompletos.\n";

                            break;


                    }

                    int num_conta = atoi(campos[1]);
                    float valor = atof(campos[2]);

                    int resultado = sacar(&lista,num_conta,valor);

                    if(resultado == 0){

                        mensagem = "Saque realizado com sucesso .\n\n";
                    }else if(resultado == -2){

                        mensagem = "Conta nao possui saldo para efetuar o saque .\n\n";
                    }else{

                        mensagem = "Falha ao realizar o deposito .\n\n";

                    }

                    break;
                }
                

            case 4:
            {

                if(quantidade != 2 || campos[1] == NULL || strlen(campos[1]) == 0  || (valida_numero(campos[1])==0) ) {

                    mensagem = "ERRO;Parametros enviados estao errados , ou incompletos.\n";

                    break;

                }

               

                int numero_conta = atoi(campos[1]);
                Conta *buscada = buscar_conta(&lista, numero_conta);

                if (!buscada)
                {

                    mensagem = "ERRO;Numero da conta nao entcontrado";
                }
                else
                {

                    snprintf(buffer_envio, sizeof(buffer_envio), "OK;%d;%s;%.2f\n",
                             buscada->numero,
                             buscada->nome,
                             buscada->saldo);

                    mensagem = buffer_envio;
                }

                break;
            }
            case 5:
                envia_lista(&lista, socket_cliente);
                break;

            case 6:
            {    

                

                mensagem = "Parametros enviados estao errados , ou incompletos.\n";

                if( quantidade !=4 ) {

                    break;
                }

                int valida = 1;

                for (size_t i = 1; i < 4; i++)
                {
                    if(campos[i] == NULL){

                        valida = 0;
                        break;
                    }

                    if(i == 3){

                        if(valida_float(campos[i]) == 0) valida = 0;
                        
                    }else{

                        if(valida_numero(campos[i]) == 0){

                            valida = 0;
                            break;
                        }
                    }
                }

                if(!valida){

                    break;
                }

                int n_conta_origem = atoi(campos[1]);
                int n_conta_destino = atoi(campos[2]);
                float valor = atof(campos[3]);

                int resultado = transferencia(&lista,n_conta_origem,n_conta_destino,valor);

                if(resultado == 0){

                    mensagem = "Transferencia realizada com sucesso.\n\n";
                    break;
                }

                if(resultado == -1){

                    mensagem = "Erro ao procurar contas.\n\n";
                }else if(resultado == -3){

                    mensagem ="Valor digitado nao valido.\n\n";

                }else if(resultado == -4){

                    mensagem = "As contas informadas sao as mesmas.\n\n";
                }else if(resultado == -2){

                    mensagem ="Conta origem nao possui saldo.\n\n";
                }
                
                break;

            }
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

            if (opcao != 5 )
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

    return c +1;
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

int valida_numero(char *buffer){


    for (size_t i = 0;buffer[i] != 0; i++)
    {
        if(isdigit(buffer[i]) == 0){

            return 0;
        }
    }

    return 1;
    
}

int valida_float(char *valor){

    int contador = 0;

    for (size_t i = 0; valor[i] != '\0'; i++)
    {

        if(i == 0 && valor[i] == '.'){

            return 0;
        }

        if(i != 0 && contador == 0 && valor[i] == '.'){

            contador++;
            continue;
        }

        if(valor[i] == '.' && contador>0 ){

            return 0;
        }

        if(isdigit(valor[i]) == 0){

            return 0;
        }
        
    }

    int posicao = strlen(valor) -1;

    if(valor[posicao] == '.'){

        return 0;
    }

    return 1;
    
}

