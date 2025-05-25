#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estados.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_paciente.h"
#include "../include/validacoes.h"

// Struct paciente
typedef struct
{
    char nome[100];
    int cpf;
    int telefone_paciente;
} Paciente;

Paciente *pacientes = NULL; // Ponteiro para o vetor de pacientes
int total_pacientes = 0;    // Contador de pacientes cadastrados

Estado tratar_modulo_paciente()
{
    Estado estado_atual = ESTADO_GERENCIAR_PACIENTE;
    int escolha_modulo;

    do
    {
        msg_37_mostrar_modulo_paciente(); // Exibe o menu de gerenciamento de pacientes
        scanf("%d", &escolha_modulo);

        switch (escolha_modulo)
        {
        case 1: // Criar Paciente
        {
            char nome_paciente[100]; // Variável para armazenar o nome do paciente
            char opcao[10];          // Variavel para armazenar a opção sim/não/sair
            int cpf;                 // Variável para armazenar o CPF do paciente
            int telefone_paciente;   // Variável para armazenar o telefone do paciente

            int sair = 0; // Variável para controlar o loop de saída
            while (!sair)
            {
                // Nome paciente
                while (1)
                {
                    msg_10_criar_paciente();
                    msg_11_nome_paciente();
                    scanf("%s", nome_paciente); // Lê o nome do paciente
                    msg_12_validar_nome_paciente();
                    printf("%s\n", nome_paciente); // Exibe o nome do paciente
                    printf("SIM | NÃO | SAIR\n");  // Exibe opções de resposta (sim -> ontinua, não -> volta para msg_11, sair -> volta para o menu paciente)
                    scanf("%s", opcao);            // Lê a opção do usuário

                    if (strcmp(opcao, "SIM") == 0) // Se a opção for "SIM"
                    {
                        break; // Sai do loop se a opção for "SIM" e continua pata próxima pergunta
                    }
                    else if (strcmp(opcao, "NÃO") == 0) // Se a opção for "NÃO"
                    {
                        continue; // Volta para a pergunta do nome
                    }
                    else if (strcmp(opcao, "SAIR") == 0) // Se a opção for "SAIR"
                    {
                        msg_05_retornando_menu();
                        sair = 1;                                 // Sai do loop
                        estado_atual = ESTADO_GERENCIAR_PACIENTE; // Retorna ao menu de gerenciar paciente
                        break;
                    }
                    else // Se a opção for inválida
                    {
                        msg_02_opcao_invalida();
                    }

                    // CPF
                    while (1)
                    {
                        msg_13_informar_cpf();
                        scanf("%d", &cpf);                        // Lê o CPF do paciente
                        printf("O CPF informado foi: %d\n", cpf); // Exibe o CPF do paciente
                        printf("O CPF está correto?\n");          // Pergunta se o CPF está correto
                        printf("SIM | NÃO | SAIR\n");             // Exibe opções de resposta (sim -> ontinua, não -> volta para msg_13, sair -> volta para o menu paciente)
                        scanf("%s", opcao);                       // Lê a opção do usuário

                        if (strcmp(opcao, "SIM") == 0) // Se a opção for "SIM"
                        {
                            break; // Sai do loop se a opção for "SIM" e continua para próxima pergunta
                        }
                        else if (strcmp(opcao, "NÃO") == 0) // Se a opção for "NÃO"
                        {
                            continue; // Volta para a pergunta do CPF
                        }
                        else if (strcmp(opcao, "SAIR") == 0) // Se a opção for "SAIR"
                        {
                            msg_05_retornando_menu();
                            sair = 1;                                 // Sai do loop
                            estado_atual = ESTADO_GERENCIAR_PACIENTE; // Retorna ao menu gerenciar pacientes
                            break;
                        }
                        else // Se a opção for inválida
                        {
                            msg_02_opcao_invalida();
                        }
                    }

                    // Telefone
                    while (1)
                    {
                        msg_14_informar_telefone();
                        scanf("%d", &telefone_paciente);                             // Lê o telefone do paciente
                        printf("O telefone informado foi: %d\n", telefone_paciente); // Exibe o telofene do paciente
                        printf("O telefone está correto?\n");                        // Pergunta se o telefone está correto
                        printf("SIM | NÃO | SAIR\n");                                // Exibe opções de resposta (sim -> ontinua, não -> volta para msg_14, sair -> volta para o menu paciente)
                        scanf("%s", opcao);                                          // Lê a opção do usuário

                        if (strcmp(opcao, "SIM") == 0) // Se a opção for "SIM"
                        {
                            msg_15_sucesso_cadastro();
                            // Salvar paciente no vetor de pacientes
                            Paciente novo;
                            strcpy(novo.nome, nome_paciente);                                             // Copia o nome do paciente para a estrutura
                            novo.cpf = cpf;                                                               // Atribui o CPF à estrutura
                            novo.telefone_paciente = telefone_paciente;                                   // Atribui o telefone à estrutura
                            Paciente *tmp = realloc(pacientes, (total_pacientes + 1) * sizeof(Paciente)); // Realoca memória para o vetor de pacientes
                            if (tmp == NULL)                                                              // Se a realocaçãp falhar
                            {
                                printf("Erro ao alocar memória para o paciente.\n");
                                exit(1); // Encerra o progama
                            }
                            pacientes = tmp;                     // Atualiza o ponteiro de pacientes
                            pacientes[total_pacientes++] = novo; // Adiciona o novo paciente ao vetor
                            break;                               // Sai do loop se a opção for "SIM"
                        }
                        else if (strcmp(opcao, "NÃO") == 0) // Se a opção for "NÃO"
                        {
                            continue; // Volta para a pergunta do telefone
                        }
                        else if (strcmp(opcao, "SAIR") == 0) // Se a opção for "SAIR"
                        {
                            msg_05_retornando_menu();
                            sair = 1; // Sai do loop
                            estado_atual = ESTADO_GERENCIAR_PACIENTE;
                            break;
                        }
                        else // Se a opção for inválida
                        {
                            msg_02_opcao_invalida();
                        }
                    }
                    break; // Sai do loop principal após completar todas as perguntas
                }
            }
            break;
        }

        case 2:                 // Exibir Paciente
            validar_paciente(); // Chama a função de validação de paciente
            break;

        case 3: // Atualizar Paciente
            printf("Atualizar paciente selecionado.\n");
            break;

        case 4: // Deletar Paciente
        {
            int cpf;
            int encontrado = 0;
            int i;
            char resposta[10];

            msg_13_informar_cpf();
            scanf("%d", &cpf); // Lê o CPF do paciente

            // Procura o paciente no vetor
            for (i = 0; i < total_pacientes; i++)
            {
                if (pacientes[i].cpf == cpf)
                {
                    encontrado = 1;
                    break; // Sai do loop
                }
            }

            if (!encontrado)
            {
                printf("Paciente não cadastrado.\n");
                break;
            }
            while (1)
            {
                msg_21_validar_paciente();
                printf("1 - SIM\n2 - NÃO\n3 - SAIR\n"); // Exibe opções de resposta (sim -> deleta o paciente, não -> volta para msg_13_informar_cpf, sair -> volta para o menu principal)
                scanf("%s", resposta);

                if (strcmp(resposta, "1") == 0) // Se a opção dor 1 - SIM, delta o paciente
                {
                    // Remove o pacinete do vetor deslocamento
                    for (int j = i; j < total_pacientes - 1; j++)
                    {
                        pacientes[j] = pacientes[j + 1];
                    }
                    total_pacientes--;
                    printf("Deletado com sucesso!\n");
                    estado_atual = ESTADO_MENU_PRINCIPAL;
                    break;
                }
                else if (strcmp(resposta, "2") == 0 || strcmp(resposta, "3") == 0) // As resposta 2 e 3, ambas voltam para o menu principal
                {
                    estado_atual = ESTADO_MENU_PRINCIPAL;
                    break;
                }
                else
                { // Se resposta inválida, mostra mensagem e repete a pergunta
                    msg_02_opcao_invalida();
                }
            }
            break;
        }

        case 5: // Voltar ao menu principal
            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;

        case 6: // Sair do programa
            estado_atual = ESTADO_SAIR;
            break;

        default:
            msg_02_opcao_invalida();
            break;
        }
    } while (estado_atual == ESTADO_GERENCIAR_PACIENTE);

    return estado_atual;
}