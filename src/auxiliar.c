#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estruturas.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_paciente.h"
#include "../include/validacoes.h"

void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // Consome todos os caracteres restantes no buffer
}

int validar_nome_paciente(char *nome_paciente, size_t tamanho)
{
    do
    {
        msg_11_nome_paciente();
        fgets(nome_paciente, tamanho, stdin);               // Lê o nome do paciente
        nome_paciente[strcspn(nome_paciente, "\n")] = '\0'; // Remove o caractere de nova linha

        if (strlen(nome_paciente) == 0) // Verifica se o nome está vazio
        {
            msg_02_opcao_invalida();
            continue; // Volta para a pergunta do nome
        }

        return 1; // Nome válido
    } while (1);
}

int validar_opcao_usuario()
{
    char opcao[10];

    do
    {
        // Lê a opção do usuário
        // limpar_buffer(); // Limpa o buffer antes de capturar a entrada
        msg_40_opcoes();

        if (fgets(opcao, sizeof(opcao), stdin) == NULL)
        {
            msg_02_opcao_invalida();
            continue;
        }
        opcao[strcspn(opcao, "\n")] = '\0'; // Remove o caractere de nova linha

        if (strcmp(opcao, "1") == 0) // Se a opção for "SIM"
        {
            return 1; // Confirmação
        }
        else if (strcmp(opcao, "2") == 0) // Se a opção for "NÃO"
        {
            return 2; // Rejeição
        }
        else if (strcmp(opcao, "3") == 0) // Se a opção for "SAIR"
        {
            return 3; // Sair
        }
        else // Se a entrada for inválida
        {
            msg_02_opcao_invalida();
        }
    } while (1);
}

int validar_cpf(char *cpf, size_t tamanho)
{
    do
    {
        msg_13_informar_cpf(); // Mensagem para solicitar o CPF
        // limpar_buffer();                // Limpa o buffer antes de capturar o CPF
        fgets(cpf, tamanho, stdin);     // Lê o CPF como string
        cpf[strcspn(cpf, "\n")] = '\0'; // Remove o caractere de nova linha

        // Verifica se o CPF possui exatamente 11 dígitos
        if (strlen(cpf) != 11)
        {
            msg_02_opcao_invalida();
            printf("O CPF deve conter exatamente 11 dígitos.\n");
            continue;
        }

        // Verifica se todos os caracteres são números
        int valido = 1;
        for (size_t i = 0; i < strlen(cpf); i++)
        {
            if (cpf[i] < '0' || cpf[i] > '9')
            {
                valido = 0;
                break;
            }
        }

        if (!valido)
        {
            msg_02_opcao_invalida();
            printf("O CPF deve conter apenas números.\n");
            continue;
        }

        return 1; // CPF válido
    } while (1);
}

int validar_telefone(char *telefone, size_t tamanho)
{
    do
    {
        msg_14_informar_telefone(); // Mensagem para solicitar o telefone
        // limpar_buffer();                          // Limpa o buffer antes de capturar o telefone
        fgets(telefone, tamanho, stdin);          // Lê o telefone como string
        telefone[strcspn(telefone, "\n")] = '\0'; // Remove o caractere de nova linha

        // Verifica se o telefone possui exatamente 11 dígitos
        if (strlen(telefone) != 11)
        {
            msg_02_opcao_invalida();
            printf("O telefone deve conter exatamente 11 dígitos.\n");
            continue;
        }

        // Verifica se todos os caracteres são números
        int valido = 1;
        for (size_t i = 0; i < strlen(telefone); i++)
        {
            if (telefone[i] < '0' || telefone[i] > '9')
            {
                valido = 0;
                break;
            }
        }

        if (!valido)
        {
            msg_02_opcao_invalida();
            printf("O telefone deve conter apenas números.\n");
            continue;
        }

        return 1; // Telefone válido
    } while (1);
}