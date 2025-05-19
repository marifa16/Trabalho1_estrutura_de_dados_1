#include <stdio.h>
#include "../include/estados.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_paciente.h"
#include "../include/paciente.h"

Paciente *paciente = NULL; // Ponteiro para o vetor de pacientes
int total_pacientes = 0; // Contador de pacientes cadastrados

Estado tratar_modulo_paciente(int entrada)
{
    char nome_paciente[100]; // Variável para armazenar o nome do paciente
    char opcao[10]; // Variavel para armazenar a opção sim/não/sair
    int cpf; // Variável para armazenar o CPF do paciente
    int telefone; // Variável para armazenar o telefone do paciente

    switch (entrada)
    {
    case 1: // Criar Paciente
    {
        int sair = 0; // Variável para controlar o loop de saída
        while (!sair) {
            // Nome 
            while (1) {
                msg_10_criar_paciente();
                msg_11_nome_paciente();
                scanf("%s", nome_paciente); // Lê o nome do paciente
                msg_12_validar_nome_paciente();
                printf("%s\n", nome_paciente); // Exibe o nome do paciente
                printf("SIM | NÃO | SAIR\n"); // Exibe opções de resposta (sim -> ontinua, não -> volta para msg_11, sair -> volta para o menu paciente)
                scanf("%s", opcao); // Lê a opção do usuário

                if (strcmp(opcao, "SIM") == 0) // Se a opção for "SIM"
                {
                    break; // Sai do loop se a opção for "SIM" e continua pata próxima pergunta
                } else if (strcmp(opcao, "NÃO") == 0) // Se a opção for "NÃO" 
                {
                    continue; // Volta para a pergunta do nome
                } else if (strcmp(opcao, "SAIR") == 0) // Se a opção for "SAIR" 
                {
                    msg_05_retornando_menu();
                    sair = 1; // Sai do loop
                    return ESTADO_GERENCIAR_PACIENTE; // Retorna ao menu de gerenciar paciente
                } else // Se a opção for inválida
                {
                    msg_02_opcao_invalida();
                }

                // CPF 
                while (1) {
                    msg_13_informar_cpf(); 
                    scanf("%d", &cpf); // Lê o CPF do paciente 
                    printf("O CPF informado foi: %d\n", cpf); // Exibe o CPF do paciente
                    printf("O CPF está correto?\n"); // Pergunta se o CPF está correto
                    printf("SIM | NÃO | SAIR\n"); // Exibe opções de resposta (sim -> ontinua, não -> volta para msg_13, sair -> volta para o menu paciente)
                    scanf("%s", opcao); // Lê a opção do usuário

                    if (strcmp(opcao, "SIM") == 0) // Se a opção for "SIM"
                    {
                        break; // Sai do loop se a opção for "SIM" e continua para próxima pergunta
                    } else if (strcmp(opcao, "NÃO") == 0) // Se a opção for "NÃO"
                    {
                        continue; // Volta para a pergunta do CPF
                    } else if (strcmp(opcao, "SAIR") == 0) // Se a opção for "SAIR"
                    {
                        msg_05_retornando_menu();
                        sair = 1; // Sai do loop 
                        return ESTADO_GERENCIAR_PACIENTE; // Retorna ao menu gerenciar pacientes
                } else // Se a opção for inválida
                    {
                        msg_02_opcao_invalida(); 
                    }
                }                   
                
                // Telefone
                while (1) {
                    msg_14_informar_telefone();
                    scanf("%d", &telefone); // Lê o telefone do paciente 
                    printf("O telefone informado foi: %d\n", telefone); // Exibe o telofene do paciente
                    printf("O telefone está correto?\n"); // Pergunta se o telefone está correto
                    printf("SIM | NÃO | SAIR\n"); // Exibe opções de resposta (sim -> ontinua, não -> volta para msg_14, sair -> volta para o menu paciente)
                    scanf("%s", opcao); // Lê a opção do usuário

                    if (strcmp(opcao, "SIM") == 0) // Se a opção for "SIM"
                    {
                        msg_15_sucesso_cadastro();
                        // Salvar paciente no vetor de pacientes
                        strcpy(novo.paciente, nome_paciente); // Copia o nome do paciente para a estrutura
                        novo.cpf = cpf; // Atribui o CPF à estrutura 
                        novo.telefone = telefone; // Atribui o telefone à estrutura
                        Paciente *tpm = realloc(paciente, (total_pacientes + 1) * sizeof(Pacientes)); // Realoca memória para o vetor de pacientes
                        if (tmp = NULL) // Se a realocaçãp falhar
                        {
                            printf("Erro ao alocar memória para o paciente.\n");
                            exit(1); // Encerra o progama
                        }
                        paciente = tmp; // Atualiza o ponteiro de pacientes
                        paciente[total_pacientes++] = novo; // Adiciona o novo paciente ao vetor
                        break; // Sai do loop se a opção for "SIM"
                } else if (ctrcmp(opcao, "NÃO") == 0) // Se a opção for "NÃO"
                {
                    continue; // Volta para a pergunta do telefone
                } else if(strcmp(opcao, "SAIR") == 0) // Se a opção for "SAIR"
                {
                    msg_05_retornando_menu();
                    sair = 1; // Sai do loop
                    return ESTADO_GERENCIAR_PACIENTE;
                } else // Se a opção for inválida
                {
                    msg_02_opcao_invalida();
                }
            }
            break; // Sai do loop principal após completar todas as perguntas 
        }
    }
        return ESTADO_GERENCIAR_PACIENTE;
    }

    case 2: // Exibir Paciente
        printf("Exibir paciente selecionado.\n");
        return ESTADO_GERENCIAR_PACIENTE;

    case 3: // Atualizar Paciente
        printf("Atualizar paciente selecionado.\n");
        return ESTADO_GERENCIAR_PACIENTE;

    case 4: // Deletar Paciente
        printf("Deletar paciente selecionado.\n");
        return ESTADO_GERENCIAR_PACIENTE;

    case 5: // Voltar ao menu principal
        return ESTADO_MENU_PRINCIPAL;

    case 6: // Sair do programa
        return ESTADO_SAIR;

    default: // Opção inválida
        msg_02_opcao_invalida();
        return ESTADO_GERENCIAR_PACIENTE;
    }
}