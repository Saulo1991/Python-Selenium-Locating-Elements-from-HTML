#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define TIMEOUT 1000

void scan_port(const char *ip, int port) {
    int sock;
    struct sockaddr_in target;
    struct timeval tv;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket");
        return;
    }

    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_pton(AF_INET, ip, &target.sin_addr);

    tv.tv_sec = 0;
    tv.tv_usec = TIMEOUT * 1000;

    if (connect(sock, (struct sockaddr *)&target, sizeof(target)) < 0) {
        printf("Port %d is closed\n", port);
    } else {
        printf("Port %d is open\n", port);
    }

    shutdown(sock, 2);
    close(sock);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <IP> <Números de portas>\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    int start_port = atoi(argv[2]);
    int end_port = start_port + 100; // Varre as próximas 100 portas

    for (int port = start_port; port < end_port; port++) {
        scan_port(ip, port);
    }

    return 0;
}
