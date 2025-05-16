#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

int main() {

    setlocale(LC_ALL, "PORTUGUESE"); // Caractere em português
    // Defina a saída do console para UTF-8
    // system("chcp 65001 > nul");

    // Print a messagem de boas-vindas
    printf("Bem vindo ao sistema de gerenciamento de estoque.\n");

    return 0;
}