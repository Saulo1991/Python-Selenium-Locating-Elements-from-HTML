#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

//Enumerador de diretórios e subdomínios baseado em força bruta

#define BUFFER_SIZE 1024

void check_url(const char *ip, const char *path) {
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
    server.sin_port = htons(80);
    inet_pton(AF_INET, ip, &server.sin_addr);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connect");
        close(sock);
        return;
    }

    snprintf(request, sizeof(request), "GET /%s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", path, ip);
    send(sock, request, strlen(request), 0);

    int bytes_received = recv(sock, response, sizeof(response) - 1, 0);
    response[bytes_received] = '\0';

    if (strstr(response, "200 OK") != NULL) {
        printf("Found: /%s\n", path);
    } else if (strstr(response, "403 Forbidden") != NULL) {
        printf("Forbidden: /%s\n", path);
    } else if (strstr(response, "404 Not Found") != NULL) {
        // Do nothing for 404
    }

    close(sock);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <IP do servidor> <caminho do dicionário>\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    const char *dict_path = argv[2];
    FILE *dict_file = fopen(dict_path, "r");

    if (!dict_file) {
        perror("fopen");
        return 1;
    }

    char line[BUFFER_SIZE];
    while (fgets(line, sizeof(line), dict_file)) {
        line[strcspn(line, "\n")] = 0;
        check_url(ip, line);
    }

    fclose(dict_file);
    return 0;
}
