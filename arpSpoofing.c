#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>

#define MAC_LEN 6
#define IP_LEN 4

// Função para converter string de IP para array de bytes
void ip_str_to_byte(const char *ip_str, unsigned char *ip) {
    sscanf(ip_str, "%d.%d.%d.%d", (int *)&ip[0], (int *)&ip[1], (int *)&ip[2], (int *)&ip[3]);
}

// Função para converter string de MAC para array de bytes
void mac_str_to_byte(const char *mac_str, unsigned char *mac) {
    sscanf(mac_str, "%x:%x:%x:%x:%x:%x", (unsigned int *)&mac[0], (unsigned int *)&mac[1], (unsigned int *)&mac[2],
           (unsigned int *)&mac[3], (unsigned int *)&mac[4], (unsigned int *)&mac[5]);
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Uso: %s <interface> <ip_vitima> <mac_vitima> <ip_gateway>\n", argv[0]);
        return 1;
    }

    const char *iface = argv[1];
    const char *ip_vitima = argv[2];
    const char *mac_vitima = argv[3];
    const char *ip_gateway = argv[4];

    unsigned char mac_meu[MAC_LEN] = {0x00, 0x0c, 0x29, 0x48, 0x9e, 0x1d};  // Exemplo de MAC do atacante
    unsigned char mac_vit[MAC_LEN];
    unsigned char ip_vit[IP_LEN], ip_gw[IP_LEN];

    mac_str_to_byte(mac_vitima, mac_vit);
    ip_str_to_byte(ip_vitima, ip_vit);
    ip_str_to_byte(ip_gateway, ip_gw);

    // Aqui seria o código de montagem e envio de pacotes ARP falsos.
    // Este exemplo é para ilustração e deve ser adaptado e expandido.

    return 0;
}
