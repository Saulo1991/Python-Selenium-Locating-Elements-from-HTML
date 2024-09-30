#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 2048

void query_whois(const char *domain) {
    int sock;
    struct sockaddr_in server;
    char request[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket");
        return;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(43); // Porta WHOIS
    inet_pton(AF_INET, "8.8.8.8", &server.sin_addr); // Usando o servidor WHOIS público do Google

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connect");
        close(sock);
        return;
    }

    snprintf(request, sizeof(request), "%s\r\n", domain);
    send(sock, request, strlen(request), 0);

    int bytes_received = recv(sock, response, sizeof(response) - 1, 0);
    response[bytes_received] = '\0';

    printf("WHOIS Response for %s:\n%s\n", domain, response);

    close(sock);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <domínio>\n", argv[0]);
        return 1;
    }

    query_whois(argv[1]);

    return 0;
}
