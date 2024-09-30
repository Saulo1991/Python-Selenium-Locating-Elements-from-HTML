#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void init_openssl() {
    SSL_load_error_strings();
    OpenSSL_add_ssl_algorithms();
}

void cleanup_openssl() {
    EVP_cleanup();
}

void check_ssl(const char *hostname, const char *port) {
    SSL_CTX *ctx;
    SSL *ssl;
    int server;
    
    ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        fprintf(stderr, "Unable to create SSL context\n");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    ssl = SSL_new(ctx);
    BIO *bio = BIO_new_ssl_connect(ctx);
    BIO_set_conn_hostname(bio, hostname);
    BIO_set_conn_port(bio, port);

    if (BIO_do_connect(bio) <= 0) {
        fprintf(stderr, "Error connecting to %s:%s\n", hostname, port);
        ERR_print_errors_fp(stderr);
    } else {
        printf("Connected to %s:%s\n", hostname, port);
        
        const SSL_CIPHER *cipher = SSL_get_current_cipher(ssl);
        printf("Cipher: %s\n", SSL_CIPHER_get_name(cipher));
        printf("SSL Version: %s\n", SSL_get_version(ssl));
    }

    BIO_free_all(bio);
    SSL_CTX_free(ctx);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <hostname> <port>\n", argv[0]);
        return 1;
    }

    init_openssl();
    check_ssl(argv[1], argv[2]);
    cleanup_openssl();

    return 0;
}
