#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void check_vulnerability(const char *url, const char *payload) {
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
    inet_pton(AF_INET, url, &server.sin_addr);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connect");
        close(sock);
        return;
    }

    snprintf(request, sizeof(request), "GET /vulnerable.php?id=%s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", payload, url);
    send(sock, request, strlen(request), 0);

    int bytes_received = recv(sock, response, sizeof(response) - 1, 0);
    response[bytes_received] = '\0';

    if (strstr(response, "SQL syntax") != NULL) {
        printf("Vulnerable to SQL Injection with payload: %s\n", payload);
    }

    close(sock);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <IP> <caminho do arquivo de payloads>\n", argv[0]);
        return 1;
    }

    const char *url = argv[1];
    const char *payload_file = argv[2];
    FILE *file = fopen(payload_file, "r");

    if (!file) {
        perror("fopen");
        return 1;
    }

    char payload[BUFFER_SIZE];
    while (fgets(payload, sizeof(payload), file)) {
        payload[strcspn(payload, "\n")] = 0;
        check_vulnerability(url, payload);
    }

    fclose(file);
    return 0;
}
