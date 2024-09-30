#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <regex.h>

#define BUFFER_SIZE 1024

struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t WriteCallback(void *contents, size_t size, size_t nmemb, struct MemoryStruct *userp) {
    size_t total_size = size * nmemb;
    userp->memory = realloc(userp->memory, userp->size + total_size + 1);
    if (userp->memory == NULL) {
        printf("Não foi possível alocar memória!\n");
        return 0;
    }
    memcpy(&(userp->memory[userp->size]), contents, total_size);
    userp->size += total_size;
    userp->memory[userp->size] = '\0';
    return total_size;
}

void extract_emails(const char *html) {
    regex_t regex;
    regcomp(&regex, "[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}", REG_EXTENDED);
    
    regmatch_t matches[1];
    const char *p = html;

    while (regexec(&regex, p, 1, matches, 0) == 0) {
        int start = matches[0].rm_so;
        int end = matches[0].rm_eo;
        
        char email[end - start + 1];
        strncpy(email, p + start, end - start);
        email[end - start] = '\0';

        printf("Email encontrado: %s\n", email);
        
        p += end; // Continue buscando após o e-mail encontrado
    }

    regfree(&regex);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <domínio>\n", argv[0]);
        return 1;
    }

    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        char url[256];
        snprintf(url, sizeof(url), "https://www.google.com/search?q=%s", argv[1]);
        
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");
        
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() falhou: %s\n", curl_easy_strerror(res));
        }

        extract_emails(chunk.memory);
        
        curl_easy_cleanup(curl);
        free(chunk.memory);
    }

    curl_global_cleanup();
    return 0;
}
