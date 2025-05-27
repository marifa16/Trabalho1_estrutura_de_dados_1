#include <stdio.h>
#include <string.h>
#include "../include/estruturas.h"
#include "../include/validacoes.h"
#include "../include/mensagens.h"

// Acesso ao vetor de pacientes
extern reg_paciente *pacientes; // Ponteiro para o vetor de pacientes
extern int total_pacientes;     // Contador de pacientes cadastrados
extern reg_medico *medicos;
extern int total_medicos;

// Declaração da função espcialidade_string
const char *especialidade_sttring(Especialidade especialidade);

int validar_paciente()
{
    char cpf[12];   // CPF como string (11 dígitos + '\0')
    char opcao[10]; // Variável para armazenar a opção sim/não/sair

    msg_31_cpf();
    fgets(cpf, sizeof(cpf), stdin); // Lê o CPF do usuário como string
    cpf[strcspn(cpf, "\n")] = '\0'; // Remove o caractere de nova linha

    for (int i = 0; i < total_pacientes; i++) // Percorre o vetor de pacientes
    {
        if (strcmp(pacientes[i].cpf, cpf) == 0) // Compara strings
        {
            printf("=====================\n");
            printf("Paciente cadastrado.\n");                // Exibe mensagem de sucesso
            printf("Nome: %s\n", pacientes[i].nome);         // Exibe o nome do paciente
            printf("CPF: %s\n", pacientes[i].cpf);           // Exibe o CPF do paciente
            printf("Telefone: %s\n", pacientes[i].telefone); // Exibe o telefone do paciente
            printf("=====================\n");
            return 1; // Retorna 1 se o CPF foi encontrado
        }
    }
    printf("Paciente não cadastrado.\n");         // Exibe mensagem de erro
    printf("Digite SAIR para voltar ao menu.\n"); // Informa ao usuário como voltar ao menu principal
    fgets(opcao, sizeof(opcao), stdin);           // Lê a opção do usuário
    opcao[strcspn(opcao, "\n")] = '\0';           // Remove o caractere de nova linha
    if (strcmp(opcao, "SAIR") == 0)               // Se a opção for "SAIR"
    {
        printf("Voltando ao menu...\n"); // Exibe mensagem de retorno
        return 0;                        // Retorna 0 para indicar que o usuário deseja voltar ao menu
    }
    msg_02_opcao_invalida();
    return 0; // Retorna 0 se o CPF não foi encontrado
}

int validar_medico()
{
    // Implementação validar médico
    int crm;
    char opcao[10];

    msg_30_crm();
    scanf("%d", &crm); // Lê a o CRM do usuário

    for (int i = 0; i < total_medicos; i++) // Percorre o vetor médico
    {
        if (medicos[i].crm == crm) // Seo CRM do médico cadastraddo for igual ao CRM informado
        {
            printf("=====================\n");
            printf("Médico cadastrado.\n");                          // Exibe mensagem de sucesso
            printf("Nome: %s\n", medicos[i].nome);                   // Exibe o nome do médico
            printf("CRM: %d\n", medicos[i].crm);                     // Exibe o CRM do médico
            printf("Especialidade: %s\n", especialidade_string(medicos[i].especialidade_medico)); // Exibe a especialidade do médico
            printf("Telefone: %d\n", medicos[i].telefone_medico);           // Exibe o telefone do médico
            printf("=====================\n");
            return 1; // Retorna 1 se o CPF foi encontrado
        }
    }
    printf("Medico não cadastrado.\n");           // Exibe mensagem de erro
    printf("Digite SAIR para voltar ao menu.\n"); // Informa ao usuário como voltar ao menu principal
    scanf("%s", opcao);                           // Lê a opção do usuário
    if (strcmp(opcao, "SAIR") == 0)               // Se a opção for "SAIR"
    {
        return 0; // Retorna 0 para indicar que o usuário deseja voltar ao menu
    }
    msg_02_opcao_invalida();
    return 0; // Retorna 0 se o CRM não foi encontrado
}

int validar_mais_op()
{
    char opcao[10];

    while (1)
    {
        msg_32_mudar_algo();
        scanf("%s", opcao);

        if (strcmp(opcao, "Sim") == 0)
        {
            return 1; // Retorna para continuar medando
        }
        else if (strcmp(opcao, "Não") == 0)
        {
            return 0; // Retorna para parar
        }
        else
        {
            msg_02_opcao_invalida();
        }
    }
}