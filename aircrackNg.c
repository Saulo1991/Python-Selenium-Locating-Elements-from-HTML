#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>

void packet_handler(u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {
    printf("Packet captured: Length: %d\n", header->len);
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

    pcap_loop(handle, 10, packet_handler, NULL);

    pcap_close(handle);
    return 0;
}
