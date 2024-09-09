#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int count = 0;
    char number[100];  // Mudei de char para um array de char
    int intNumber;

    while (1) {
        printf("Type your number: ");
        scanf("%s", number);  // Agora lê como string
        count++;

        // Verifica se o campo está vazio ou apenas contém espaços
        if (strcmp(number, "") == 0 || strcmp(number, " ") == 0) {
            printf("Empty field\n");
            continue;
        }

        intNumber = atoi(number);  // Converte string para número

        if (intNumber < 0) {
            printf("End of program\n");
            break;
        }
    }

    printf("Total of numbers: %d\n", count);  // Corrigido para imprimir o contador

    return 0;
}
