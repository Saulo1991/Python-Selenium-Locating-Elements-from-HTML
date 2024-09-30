#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

//Ferramenta para quebra de senhas (John the Ripper)

void md5_hash(const char *password, unsigned char output[MD5_DIGEST_LENGTH]) {
    MD5((unsigned char *)password, strlen(password), output);
}

int compare_hash(const char *password, const char *hash) {
    unsigned char output[MD5_DIGEST_LENGTH];
    char hex_output[MD5_DIGEST_LENGTH * 2 + 1];
    md5_hash(password, output);
    
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(&hex_output[i * 2], "%02x", output[i]);
    }

    return strcmp(hex_output, hash) == 0;
}

void brute_force(const char *hash, const char *base, int length, int max_length) {
    if (length == max_length) return;
    
    for (int i = 0; i < 26; i++) {
        char password[max_length + 1];
        snprintf(password, length + 2, "%s%c", base, 'a' + i);
        
        if (compare_hash(password, hash)) {
            printf("Password found: %s\n", password);
            return;
        }

        brute_force(hash, password, length + 1, max_length);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <hash_md5> <max_length>\n", argv[0]);
        return 1;
    }

    const char *hash = argv[1];
    int max_length = atoi(argv[2]);

    brute_force(hash, "", 0, max_length);
    return 0;
}
