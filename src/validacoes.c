#include <stdio.h>
#include <string.h>
#include "../include/validacoes.h"
#include "../include/paciente.h"

// Acesso ao vetor de pacientes
extern Paciente *paciente;  // Ponteiro para o vetor de pacientes
extern int total_pacientes; // Contador de pacientes cadastrados

int validar_paciente()
{
    // Implementação da validação de paciente
    int cpf;
    char opcao[10];
    int encontrado = 0; // Variável para verificar se o CPF foi encontrado

    msg_31_cpf();
    scanf("%d", &cpf); // Lê o CPF do usuário

    for (int i = 0; i < total_pacientes; i++) // Percorre o vetor de pacientes
    {
        if (paciente[i].cpf == cpf) // Se o CPF do paciente for igual ao CPF informado
        {
            encontrado = 1; // Marca que o CPF foi encontrado
            printf("Paciente cadastrado.\n");
            printf("Nome: %s\n", paciente[i].paciente);     // Exibe o nome do paciente
            printf("CPF: %d\n", paciente[i].cpf);           // Exibe o CPF do paciente
            printf("Telefone: %d\n", paciente[i].telefone); // Exibe o telefone do paciente

            printf("Digite SAIR para voltar ao menu principal.\n"); // Exibe mensagem para voltar ao menu
            scanf("%s", opcao);                                     // Lê a opção do usuário
            return 1;                                               // Retorna 1 se o CPF foi encontrado
        }
    }
    if (!encontrado) // Se o CPF não foi encontrado
    {
        printf("Paciente não cadastrado.\n");                   // Exibe mensagem de erro
        printf("Digite SAIR para voltar ao menu principal.\n"); // Exibe mensagem para voltar ao menu peincipal
        scanf("%s", opcao);                                     // Lê a opção do usuário
    }
    return 0; // Retorna 0 se o CPF não foi encontrado
}

int validar_medico()
{
    // Implementação da validação de médico
    return 1; // Exemplo
}

int validar_mais_op()
{
    // Implementação de outras validações
    return 1; // Exemplo
}