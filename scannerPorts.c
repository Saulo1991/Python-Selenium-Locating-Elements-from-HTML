#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in server;
    int port;
    
    if (argc < 3) {
        printf("Uso: %s <IP> <porta>\n", argv[0]);
        return 1;
    }

    char *ip = argv[1];
    port = atoi(argv[2]);

    // Criando o socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Falha ao criar socket\n");
        return 1;
    }

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Tentativa de conexão
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Porta %d fechada\n", port);
    } else {
        printf("Porta %d aberta\n", port);
    }

    close(sock);
    return 0;
}
