#include "../include/estruturas.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_medico.h"
#include "../include/validacoes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Struct Médico
typedef struct
{
    char nome[100];
    int crm;
    char especialidade[100];
    int telefone;
} Medico;

Medico *medicos = NULL;
int total_medicos = 0;

Estado tratar_modulo_medico()
{
    Estado estado_atual = ESTADO_GERENCIAR_MEDICO;
    int escolha_modulo;

    do
    {
        msg_38_mostrar_modulo_medico(); // Exibe o menu de gerenciamento de médicos
        scanf("%d", &escolha_modulo);

        switch (escolha_modulo)
        {
        case 1: // Criar Médico
        {
            char nome_medico[100]; // Variável para armazenar o nome do médico
            char opcao[10];        // Variavel para armazenar a opção sim/não/sair
            const char *especialidades[] = {
                "Clínico Geral", "Pediatra", "Cardiologista", "Dermatologista", "Psiquiatra"}; // Array de especialidades médicas
            int total_especialidades = sizeof(especialidades) / sizeof(especialidades[0]);     // Total de especialidades
            char especialidade_medico[100];                                                    // Variável para armazenar a especialidade do médico
            int crm;                                                                           // Variável para armazenar o CRM do médico
            int telefone_medico;                                                               // Variável para armazenar o telefone do médico
            int sair = 0;                                                                      // Variável para controlar o loop de saída

            // Nome médico
            while (!sair)
            {
                msg_22_criar_medico();
                msg_23_nome_medico();
                scanf("%s", nome_medico); // Lê o nome do médico
                msg_24_validar_nome_medico();
                printf("%s\n", nome_medico);  // Exibe o nome do médico
                printf("SIM | NÃO | SAIR\n"); // Exibe opções para o usuário (sim -> continuar, não -> voltar para msg_23_nome_medico, sair -> volar para o menu principal)
                scanf("%s", opcao);           // Lê a opção do usuário

                if (strcmp(opcao, "SIM") == 0) // Se a opção for ""
                {
                    break; // Sai do loop se a opção for "SIM", continua para a próxima pergunta
                }
                else if (strcmp(opcao, "NÃO") == 0) // Se a opção for "NÃO"
                {
                    continue; // Volta para msg_23_nome_medico
                }
                else if (strcmp(opcao, "SAIR") == 0) // Se a opcao for "SAIR"
                {
                    msg_05_retornando_menu();
                    sair = 1;                       // Sai do loop
                    return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu de gerenciar médico
                }
                else // Se a opção for inválida
                {
                    msg_02_opcao_invalida();
                }
            }

            // CRM
            while (1)
            {
                msg_30_crm();
                scanf("%d", &crm);                        // Lê o CRM do médico
                printf("O CRM informado foi: %d\n", crm); // Exibe o CRM do médico
                printf("O CRM está correto?\n");          // Pergunta se o CRM está correto
                printf("SIM | NÃO | SAIR\n");             // Exibe opções para o usuário (sim -> continuar, não -> voltar para msg_30_crm, sair -> voltar para o menu principal)
                scanf("%s", opcao);                       // Lê a opção do usuário

                if (strcmp(opcao, "SIM") == 0) // Se a opção for "SIM", continua para a próxima pergunta
                {
                    break; // Sai do loop se a opção for "SIM"
                }
                else if (strcmp(opcao, "NÃO") == 0) // Se a opção for "NÃO"
                {
                    continue; // Volta para msg_30_crm
                }
                else if (strcmp(opcao, "SAIR") == 0) // Se a opção
                {
                    msg_05_retornando_menu();
                    sair = 1;                       // Sai do loop
                    return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu de gerenciar médico
                }
                else // Se a opção for inválida
                {
                    msg_02_opcao_invalida();
                }
            }

            // Telefone médico
            while (1)
            {
                msg_14_informar_telefone();
                scanf("%d", &telefone_medico);                             // Lê o telefone do médico
                printf("O telefone informado foi: %d\n", telefone_medico); // Exibe o telefone do médico
                printf("O telefone está correto?\n");                      // Pergunta se o telefone está correto
                printf("SIM | NÃO | SAIR\n");                              // Exibe opções para o usuário (sim -> continuar, não -> voltar para msg_14_informar_telefone, sair -> voltar para o menu principal)
                scanf("%s", opcao);                                        // Lê a opção do usuário

                if (strcmp(opcao, "SIM") == 0) // Se a opção for "SIM", continua para a próxima pergunta
                {
                    break; // Sai do loop se a opção for "SIM"
                }
                else if (strcmp(opcao, "NÃO") == 0) // Se a opção for "NÃO"
                {
                    continue; // Volta para msg_14_informar_telefone
                }
                else if (strcmp(opcao, "SAIR") == 0) // Se a opção for "SAIR"
                {
                    msg_05_retornando_menu();
                    sair = 1;                       // Sai do loop
                    return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu de gerenciar médico
                    break;                          // Sai do loop
                }
                else // Se a opção for inválida
                {
                    msg_02_opcao_invalida();
                }
            }

            // Especialidade médica
            while (1)
            {
                printf("Escolha uma especialidade:\n"); // Exibe opções de especialidades médicas

                for (int i = 0; i < total_especialidades; i++) // Percorre o array de especialidades
                {
                    printf("[%d] - %s\n", i + 1, especialidades[i]); // Exibe a especialidade
                }
                printf("Escolha uma especialidade (1-%d): ", total_especialidades); // Pergunta para o usuário escolher uma especialidade
                int escolha_especialidade;
                scanf("%d", &escolha_especialidade); // Lê a opção do usuário

                if (escolha_especialidade >= 1 && escolha_especialidade <= total_especialidades) // Se a opção estiver dentro do intervalo de especialidades
                {
                    strcpy(especialidade_medico, especialidades[escolha_especialidade - 1]); // Copia a especialidade escolhida para a variável especialidade_medico
                    printf("A especialidade escolhida foi: %s\n", especialidade_medico);     // Exibe a especialidade escolhida
                    printf("A especialidade está correta?\n");                               // Pergunta se a especialidade está correta
                    printf("SIM | NÃO | SAIR\n");                                            // Exibe opções para o usuário (sim -> continuar, não -> voltar para a escolha de especialidade, sair -> voltar para o menu principal)
                    scanf("%s", opcao);                                                      // Lê a opção do usuário

                    if (strcmp(opcao, "SIM") == 0) // Se a opção for "SIM", continua para a próxima pergunta
                    {
                        msg_29_sucesso_cadastro_medico();

                        Medico novo;
                        strcpy(novo.nome, nome_medico);
                        novo.crm = crm;
                        strcpy(novo.especialidade, especialidade_medico);
                        novo.telefone = telefone_medico;

                        Medico *tmp = realloc(medicos, (total_medicos + 1) * sizeof(Medico));
                        if (tmp == NULL)
                        {
                            printf("Erro ao alocar mémoria para o médico.\n");
                            exit(1);
                        }
                        medicos = tmp;
                        medicos[total_medicos++] = novo;
                        break; // Sai do loop se a opção for "SIM"
                    }
                    else if (strcmp(opcao, "NÃO") == 0) // Se a opção for "NÃO"
                    {
                        continue; // Volta para a escolha de especialidade
                    }
                    else if (strcmp(opcao, "SAIR") == 0) // Se a opção for "SAIR"
                    {
                        msg_05_retornando_menu();
                        sair = 1;                       // Sai do loop
                        return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu de gerenciar médico
                    }
                    else
                    {
                        msg_02_opcao_invalida();
                    }
                }
                break; // Sai do loop se a opção estiver dentro do intervalo de especialidades
            }
            return ESTADO_MENU_PRINCIPAL; // Retorna ao menu principal
        }

        case 2: // exibir
        {
            validar_medico(); // Chama a função de validação do médico
        }
            return ESTADO_GERENCIAR_MEDICO;

        case 3: // atualizar
            printf("Atualizar médico selecionado.\n");
            //
            return ESTADO_MENU_PRINCIPAL;

        case 4: // deletar
        {
            int crm;
            int encontrado = 0;
            int i;
            char resposta[10];

            msg_30_crm();
            scanf("%d", &crm);

            // Procura médico no vetor
            for (i = 0; i < total_medicos; i++)
            {
                if (medicos[i].crm == crm)
                {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado)
            {
                printf("Médico não cadastrado.\n");
                return ESTADO_GERENCIAR_MEDICO;
            }
            while (1)
            {
                msg_28_validar_medico();
                printf("1 - SIM\n2 - NÃO\n3 - SAIR\n"); // Exibe opções de resposta (sim -> deleta médico, não -> volta para msg_30_crm, sair -> volta para o menu principal)
                scanf("%s", resposta);

                if (strcmp(resposta, "1") == 0) // SIM, deleta o médico
                {
                    // Remove o médico do vetor
                    for (int j = i; j < total_medicos - 1; j++)
                    {
                        medicos[j] = medicos[j + 1];
                    }
                    total_medicos--;
                    printf("Médico deletado com sucesso!\n");
                    return ESTADO_MENU_PRINCIPAL;
                }
                else if (strcmp(resposta, "2") == 0 || strcmp(resposta, "3") == 0) // As resposta 2 e 3, ambas voltam para o menu principal
                {
                    return ESTADO_MENU_PRINCIPAL;
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            }
        }

        case 5: // voltar
            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;

        case 6: // sair
        {
            msg_03_finalizacao();
        }
            estado_atual = ESTADO_SAIR;
            break;

        default: // inválido
            msg_02_opcao_invalida();
            break;
        }
    } while (estado_atual == ESTADO_GERENCIAR_MEDICO);

    return estado_atual;
}