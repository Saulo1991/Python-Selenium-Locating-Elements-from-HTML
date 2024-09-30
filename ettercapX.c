#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libnet.h>

#define ETHERTYPE_IP 0x0800

void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    printf("Packet captured: Length: %d\n", header->len);
    
    // Simplesmente imprime os primeiros 16 bytes do pacote
    for (int i = 0; i < 16 && i < header->len; i++) {
        printf("%02x ", packet[i]);
    }
    printf("\n");
}

void inject_packet(const char *device) {
    libnet_t *l;
    char errbuf[LIBNET_ERRBUF_SIZE];

    l = libnet_init(LIBNET_RAW4, device, errbuf);
    if (l == NULL) {
        fprintf(stderr, "Error initializing libnet: %s\n", errbuf);
        exit(EXIT_FAILURE);
    }

    u_int32_t dst_ip = libnet_name2addr4(l, "192.168.1.1", LIBNET_RESOLVE); // IP de destino
    u_int32_t src_ip = libnet_name2addr4(l, "192.168.1.2", LIBNET_RESOLVE); // IP de origem

    // Construindo o pacote IP
    libnet_build_ipv4(
        LIBNET_IPV4_H + LIBNET_TCP_H, // comprimento do cabeçalho
        0,                              // Tipo de serviço
        242,                            // ID
        0,                              // Fragments
        64,                             // TTL
        IPPROTO_TCP,                   // Protocolo
        0,                              // Checksum (0 para auto)
        src_ip,                        // IP de origem
        dst_ip,                        // IP de destino
        NULL,                          // Payload
        0,                              // Tamanho do payload
        l,                              // Handle libnet
        0                               // ID do pacote
    );

    // Enviar o pacote
    if (libnet_write(l) == -1) {
        fprintf(stderr, "Error sending packet: %s\n", libnet_geterror(l));
    }

    libnet_destroy(l);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <interface>\n", argv[0]);
        return 1;
    }

    char *dev = argv[1];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Could not open device %s: %s\n", dev, errbuf);
        return 1;
    }

    // Captura de pacotes
    pcap_loop(handle, 10, packet_handler, NULL);

    // Injetar pacote
    inject_packet(dev);

    pcap_close(handle);
    return 0;
}
