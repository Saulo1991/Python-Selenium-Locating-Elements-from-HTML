#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//Interface interativa do Metasploit Framework para exploração de vulnerabilidades.

#define BUFFER_SIZE 1024

void execute_command(const char *command) {
    char buffer[BUFFER_SIZE];
    FILE *fp;

    fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen");
        return;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    pclose(fp);
}

int main() {
    char command[BUFFER_SIZE];

    printf("Welcome to SimpleShell, type 'exit' to quit.\n");

    while (1) {
        printf(">>> ");
        if (fgets(command, sizeof(command), stdin) == NULL) {
            break; // EOF
        }

        command[strcspn(command, "\n")] = 0; // Remove newline

        if (strcmp(command, "exit") == 0) {
            break;
        }

        execute_command(command);
    }

    return 0;
}
