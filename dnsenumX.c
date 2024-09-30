#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <resolv.h>

void query_dns(const char *domain) {
    unsigned char answer[NS_PACKETSZ];
    ns_msg handle;
    ns_rr rr;
    int n;
    
    n = res_query(domain, C_IN, T_A, answer, sizeof(answer));
    if (n < 0) {
        perror("res_query");
        return;
    }
    
    if (ns_initparse(answer, n, &handle) < 0) {
        perror("ns_initparse");
        return;
    }
    
    int num_rr = ns_msg_count(handle, ns_s_an);
    for (int i = 0; i < num_rr; i++) {
        if (ns_parserr(&handle, ns_s_an, i, &rr) < 0) {
            perror("ns_parserr");
            return;
        }
        
        char name[NS_MAXDNAME];
        ns_name_uncompress(answer, answer + n, ns_rr_name(rr), name, sizeof(name));
        struct in_addr addr;
        memcpy(&addr, ns_rr_rdata(rr), sizeof(addr));
        printf("A record: %s -> %s\n", name, inet_ntoa(addr));
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <domínio>\n", argv[0]);
        return 1;
    }

    query_dns(argv[1]);
    return 0;
}
