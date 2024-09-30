#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 8192

void handle_request(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        close(client_socket);
        return;
    }
    buffer[bytes_received] = '\0';

    printf("Request:\n%s\n", buffer);

    // Extrair o endereço do servidor e a porta da requisição
    char method[10], host[100], path[100];
    sscanf(buffer, "%s http://%99[^/]%99[^\n]", method, host, path);

    // Conectar ao servidor de destino
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); // Porta HTTP
    inet_pton(AF_INET, host, &server_addr.sin_addr);

    connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Enviar a requisição ao servidor de destino
    send(server_socket, buffer, bytes_received, 0);

    // Receber a resposta do servidor
    while ((bytes_received = recv(server_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        send(client_socket, buffer, bytes_received, 0);
    }

    close(server_socket);
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);

    printf("Proxy server listening on port %d...\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
        handle_request(client_socket);
    }

    close(server_socket);
    return 0;
}
