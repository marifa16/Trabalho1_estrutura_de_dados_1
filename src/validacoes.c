#include <stdio.h>
#include <string.h>
#include "../include/validacoes.h"

typedef struct {
    char nome[100];
    int crm;
    char especialidade[100];
    int telefone;
} Medico;

typedef struct {
    char nome[100];
    int cpf;
    int telefone_paciente;
} Paciente;

// Acesso ao vetor de pacientes
extern Paciente *pacientes;  // Ponteiro para o vetor de pacientes
extern int total_pacientes; // Contador de pacientes cadastrados
extern Medico *medicos;
extern int total_medicos;

int validar_paciente()
{
    // Implementação da validação de paciente
    int cpf;
    char opcao[10]; // Variável para armazenar a opção sim/não/sair

    msg_31_cpf();
    scanf("%d", &cpf); // Lê o CPF do usuário

    for (int i = 0; i < total_pacientes; i++) // Percorre o vetor de pacientes
    {
        if (pacientes[i].cpf == cpf) // Se o CPF do paciente cadastrado for igual ao CPF informado
        {
            printf("=====================\n");
            printf("Paciente cadastrado.\n"); // Exibe mensagem de sucesso
            printf("Nome: %s\n", pacientes[i].nome); // Exibe o nome do paciente
            printf("CPF: %d\n", pacientes[i].cpf); // Exibe o CPF do paciente
            printf("Telefone: %d\n", pacientes[i].telefone_paciente); // Exibe o telefone do paciente
             printf("=====================\n");
            return 1;// Retorna 1 se o CPF foi encontrado
        }
    }
    printf("Paciente não cadastrado.\n"); // Exibe mensagem de erro
    printf("Digite SAIR para voltar ao menu.\n"); // Informa ao usuário como voltar ao menu principal
    scanf("%s", opcao); // Lê a opção do usuário
    if (strcmp(opcao, "SAIR") == 0) // Se a opção for "SAIR"
    {
        printf("Voltando ao menu...\n"); // Exibe mensagem de retorno
        return 0; // Retorna 0 para indicar que o usuário deseja voltar ao menu
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
            printf("Médico cadastrado.\n"); // Exibe mensagem de sucesso
            printf("Nome: %s\n", medicos[i].nome); // Exibe o nome do médico
            printf("CRM: %d\n", medicos[i].crm); // Exibe o CRM do médico
            printf("Especialidade: %s\n", medicos[i].especialidade); // Exibe a especialidade do médico
            printf("Telefone: %d\n", medicos[i].telefone); // Exibe o telefone do médico
            printf("=====================\n");
            return 1;// Retorna 1 se o CPF foi encontrado
        }
    }
    printf("Medico não cadastrado.\n"); // Exibe mensagem de erro
    printf("Digite SAIR para voltar ao menu.\n"); // Informa ao usuário como voltar ao menu principal
    scanf("%s", opcao); // Lê a opção do usuário
    if (strcmp(opcao, "SAIR") == 0) // Se a opção for "SAIR"
    {
        return 0; // Retorna 0 para indicar que o usuário deseja voltar ao menu
    }
    msg_02_opcao_invalida(); 
    return 0; // Retorna 0 se o CRM não foi encontrado
}

int validar_mais_op()
{
    char opcao[10];

    while(1) {
        msg_32_mudar_algo();
        scanf("%s", opcao);

        if(strcmp(opcao, "Sim") == 0) {
            return 1; // Retorna para continuar medando
        } else if(strcmp(opcao, "Não") == 0) {
            return 0; // Retorna para parar
        } else {
            msg_02_opcao_invalida();
        }
    }
}