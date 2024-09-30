#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include "toralize.h"

#define reqsize 128
#define ressize 128
#define PROXYPORT 8080
#define PROXY "192.168.1.1"
#define USERNAME "user"

void init_winsock() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Falha ao inicializar o Winsock. Código de erro: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
}

Req *request(const char *dstip, const int dstport) {
    Req *req;

    req = malloc(reqsize);
    
    req->vn = 4;
    req->cd = 1;
    req->dstport = htons(dstport);
    req->dstip = inet_addr(dstip);
    strncpy(req->userid, USERNAME, 8);

    return req;
}

int main(int argc, char *argv[]) {
    char *host;
    int port, s;
    struct sockaddr_in sock;
    Req *req;
    Res *res;
    char buf[ressize];
    char success;
    char tmp[512];

    init_winsock();

    if (argc < 3) {
        fprintf(stderr, "Uso: %s <host> <port>\n", argv[0]);
        return -1;
    }

    host = argv[1];
    port = atoi(argv[2]);

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("Erro ao criar socket");
        return -1;
    }

    res = (Res *)buf;

    sock.sin_family = AF_INET;
    sock.sin_port = htons(PROXYPORT);
    sock.sin_addr.s_addr = inet_addr(PROXY);

    if (connect(s, (struct sockaddr *)&sock, sizeof(sock)) != 0) {
        perror("Erro ao conectar");
        return -1;
    }

    printf("Conectado ao proxy\n");
    req = request(host, port);
    send(s, (char *)req, reqsize, 0);

    memset(buf, 0, ressize);
    if (recv(s, buf, ressize, 0) < 1) {
        perror("Erro ao ler resposta");
        free(req);
        closesocket(s);
        return -1;
    }

    res = (Res *)buf;
    success = (res->cd == 90);
    if (!success) {
        fprintf(stderr, "Não foi possível atravessar o proxy, código de erro: %d\n", res->cd);
        closesocket(s);
        free(req);
        return -1;
    }

    printf("Conectado com sucesso através do proxy para %s:%d\n", host, port);

    memset(tmp, 0, 512);
    snprintf(tmp, 511,
        "HEAD / HTTP/1.0\r\n"
        "Host: www.example.com\r\n"
        "\r\n");
    send(s, tmp, strlen(tmp), 0);

    memset(tmp, 0, 512);
    recv(s, tmp, 511, 0);
    printf("'%s'\n", tmp);

    closesocket(s);
    free(req);

    WSACleanup();

    return 0;
}


//Run: 
//gcc -o my_proxy_program main.c -lws2_32

//than run:
//my_proxy_program <host> <port>