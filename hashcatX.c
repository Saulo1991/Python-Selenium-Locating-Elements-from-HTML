#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>

#define MAX_LENGTH 6

void calculate_md5(const char *str, unsigned char *output) {
    MD5((unsigned char *)str, strlen(str), output);
}

void print_hash(unsigned char *hash) {
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");
}

int main() {
    const char *target_hash = "5d41402abc4b2a76b9719d911017c592"; // Hash MD5 de "hello"
    unsigned char hash[MD5_DIGEST_LENGTH];
    char attempt[MAX_LENGTH + 1];

    for (int i = 0; i < (1 << (MAX_LENGTH * 8)); i++) {
        snprintf(attempt, sizeof(attempt), "%06d", i);
        calculate_md5(attempt, hash);

        char hash_string[MD5_DIGEST_LENGTH * 2 + 1];
        for (int j = 0; j < MD5_DIGEST_LENGTH; j++) {
            sprintf(&hash_string[j * 2], "%02x", hash[j]);
        }
        hash_string[MD5_DIGEST_LENGTH * 2] = '\0';

        if (strcmp(hash_string, target_hash) == 0) {
            printf("Password found: %s\n", attempt);
            break;
        }
    }

    return 0;
}
