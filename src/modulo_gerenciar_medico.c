#include "../include/estruturas.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_medico.h"
#include "../include/validacoes.h"
#include "../include/files_manager.h"
#include "../include/auxiliar.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

const char *especialidade_string(Especialidade especialidade);

reg_medico *medicos = NULL;
int total_medicos = 0;

Estado tratar_modulo_medico()
{
    Estado estado_atual = ESTADO_GERENCIAR_MEDICO;
    int escolha_modulo;

    do
    {
        msg_38_mostrar_modulo_medico(); // Exibe o menu de gerenciamento de médicos

        // Limpa o buffer de entrada se o próximo caractere for '\n'
        int c = getchar();
        if (c != '\n' && c != EOF)
        {
            ungetc(c, stdin); // Devolve o caractere se não era '\n'
        }

        char escolha_modulo_str[10];
        fgets(escolha_modulo_str, sizeof(escolha_modulo_str), stdin); // Lê a entrada como string
        escolha_modulo_str[strcspn(escolha_modulo_str, "\n")] = '\0'; // Remove o caractere de nova linha

        // Converte a entrada para inteiro
        if (sscanf(escolha_modulo_str, "%d", &escolha_modulo) != 1)
        {
            msg_02_opcao_invalida();
            continue;
        }

        switch (escolha_modulo)
        {
        case 1: // Criar Médico
        {
            char nome_medico[100];              // Variável para armazenar o nome do médico
            int crm;                            // Variável para armazenar o CRM do médico
            char telefone[12];                  // Variável para armazenar o telefone do médico
            char telefone_str[15];              // Agora visível em todo o escopo
            Especialidade especialidade_medico; // Variável para controlar o loop de saída

            // Nome médico
            do
            {
                msg_23_nome_medico();
                limpar_buffer();
                fgets(nome_medico, sizeof(nome_medico), stdin); // Lê o nome do méddico como string
                nome_medico[strcspn(nome_medico, "\n")] = '\0'; // Remove o caractere de nova linha

                // Verifica se o nome não está vazio
                if (strlen(nome_medico) == 0)
                {
                    msg_02_opcao_invalida();
                    printf("O nome do médico não pode estar vazio.\n");
                    continue; // Volta a solicitar o nome
                }

                // Verifica se o nome contém números
                int contem_numero = 0;
                for (size_t i = 0; i < strlen(nome_medico); i++)
                {
                    if (isdigit(nome_medico[i])) // Verifica se o caractere é um número
                    {
                        contem_numero = 1; // Marca que o nome contém números
                        break;             // Sai do loop
                    }
                }
                if (contem_numero)
                {
                    msg_02_opcao_invalida();
                    printf("O nome do médico não pode conter números.\n");
                    continue; // Volta a solicitar o nome
                }

                msg_24_validar_nome_medico();
                printf("%s\n", nome_medico); // Exibe o nome do médico

                // Função Validar
                int opcao = validar_opcao_usuario();
                if (opcao == 1) // SIM
                {
                    break; // Sai do loop
                }
                else if (opcao == 2) // NÃO
                {
                    continue; // Volta para solicitar o nome novamente
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu de gerenciamento de médicos
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);

            // CRM
            do
            {
                msg_30_crm();
                char crm_str[10];                       // Variável para armazenar o CRM como string
                fgets(crm_str, sizeof(crm_str), stdin); // Lê o CRM como string
                crm_str[strcspn(crm_str, "\n")] = '\0'; // Remove o caractere de nova linha

                // Verifica se o CRM possui 6 digitos
                if (strlen(crm_str) != 6)
                {
                    msg_02_opcao_invalida();
                    printf("O CRM deve conter 6 dígitos.\n");
                    continue; // Volta para solicitar o CRM de novo
                }

                // Verifica se todos os caracteres são números
                int valido = 1;
                for (size_t i = 0; i < strlen(crm_str); i++)
                {
                    if (crm_str[i] < '0' || crm_str[i] > '9') // Verifica se o caractere é um dígito
                    {
                        valido = 0;
                        break;
                    }
                }
                if (!valido)
                {
                    msg_02_opcao_invalida();
                    printf("O CRM deve conter apenas números.\n");
                    continue; // Volta para solicitar o CRM de novo
                }

                crm = atoi(crm_str);                      // Converte o CRM para inteiro
                printf("O CRM informado foi: %d\n", crm); // Exibe o crm do médico

                // Função Validar
                int opcao = validar_opcao_usuario();
                if (opcao == 1) // SIM
                {
                    break; // Sai do loop
                }
                else if (opcao == 2) // NÃO
                {
                    continue; // Volta para solicitar o CRM
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu gerenciar médico
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);

            // Telefone médico
            do
            {
                msg_14_informar_telefone();
                fgets(telefone_str, sizeof(telefone_str), stdin);
                telefone_str[strcspn(telefone_str, "\n")] = '\0';

                // Verifica se o telefone tem 11 digitos
                if (strlen(telefone_str) != 11)
                {
                    msg_02_opcao_invalida();
                    printf("O telefone deve conter 11 digitos (DDD + NÚMERO).\n");
                    continue; // Volta para solicitar o telefone
                }

                // Verifica se os caracteres são números
                int valido = 1;
                for (size_t i = 0; i < strlen(telefone_str); i++)
                {
                    if (telefone_str[i] < '0' || telefone_str[i] > '9') // Verifica se o caractere é um dígito
                    {
                        valido = 0;
                        break;
                    }
                }
                if (!valido)
                {
                    msg_02_opcao_invalida();
                    printf("O telefone deve conter apenas números.\n");
                    continue; // Volta para solicitar o telefone
                }

                strcpy(telefone, telefone_str);
                printf("O telefone informado foi: %s\n", telefone);

                // Função Validar
                int opcao = validar_opcao_usuario();

                if (opcao == 1) // SIM
                {
                    break; // Sai do loop
                }
                else if (opcao == 2) // NÃO
                {
                    continue; // Volta para solicitar o telefone de novo
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu gerenciar médico
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);

            // Especialidade médica
            do
            {
                msg_41_especialidade_medico();

                // Exibe as opçõe de especialidades
                for (int i = 0; i < TOTAL_ESPECIALIDADES; i++)
                {
                    printf("[%d] - %s\n", i + 1, especialidade_string(i)); // Converte o enum para uma string
                }

                printf("Escolha uma especialidade (1-%d): ", TOTAL_ESPECIALIDADES);
                char escolha_especialidade_str[10];                                         // Variavél para armazenar a entrada
                fgets(escolha_especialidade_str, sizeof(escolha_especialidade_str), stdin); // Lê a entrada como string
                escolha_especialidade_str[strcspn(escolha_especialidade_str, "\n")] = '\0'; // Remove o caractere da nova linha

                // Converte a entrada para inteiro
                int escolha_especialidade;
                if (sscanf(escolha_especialidade_str, "%d", &escolha_especialidade) != 1)
                {
                    msg_02_opcao_invalida();
                    continue; // Volta para solicitar especialidade de novo
                }
                // Verifica se a escolha está detro do intervalo
                if (escolha_especialidade >= 1 && escolha_especialidade <= TOTAL_ESPECIALIDADES)
                {
                    especialidade_medico = escolha_especialidade - 1;
                    printf("A especialidade escolhida foi: %s\n", especialidade_string(especialidade_medico));

                    // Função Validar
                    int opcao = validar_opcao_usuario();

                    if (opcao == 1) // SIM
                    {
                        reg_medico *tmp = realloc(medicos, (total_medicos + 1) * sizeof(reg_medico));
                        if (tmp == NULL)
                        { /* erro */
                        }
                        medicos = tmp;
                        strcpy(medicos[total_medicos].nome, nome_medico);
                        medicos[total_medicos].crm = crm;
                        strcpy(medicos[total_medicos].telefone, telefone_str); // telefone_str está visível aqui!
                        medicos[total_medicos].especialidade_medico = especialidade_medico;
                        total_medicos++; // Incrementa o total de médicos

                        // Adiciona o médico ao arquivo CSV
                        char *valores[4];
                        char crm_str[16];
                        snprintf(crm_str, sizeof(crm_str), "%d", medicos[total_medicos - 1].crm);
                        valores[0] = medicos[total_medicos - 1].nome;
                        valores[1] = crm_str;
                        valores[2] = (char *)especialidade_string(medicos[total_medicos - 1].especialidade_medico);
                        valores[3] = medicos[total_medicos - 1].telefone; // telefone já é string!
                        add_row("data/registro_medicos.csv", 5, valores);

                        msg_29_sucesso_cadastro_medico();
                        return 1; // Retorna 1 para indicar que o médico foi cadastrado com sucesso
                    }
                    else if (opcao == 2) // Não
                    {
                        continue; // Volta a solicitar a especialidade
                    }
                    else if (opcao == 3) // SAIR
                    {
                        msg_05_retornando_menu();
                        return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu gerenciar médico
                    }
                    else
                    {
                        msg_02_opcao_invalida();
                    }
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);
        }

        case 2: // exibir
            validar_medico();
            return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu gereenciar médico

        case 3: // atualizar
            printf("Atualizar médico selecionado.\n");
            //
            return ESTADO_MENU_PRINCIPAL;

        case 4: // deletar
            printf("Deletar médico selecionado.\n");
            return ESTADO_MENU_PRINCIPAL;

        case 5: // voltar
            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;

        case 6: // sair
            msg_03_finalizacao();
            estado_atual = ESTADO_SAIR;
            break;

        default:
            msg_02_opcao_invalida();
            break;
        }
    } while (estado_atual == ESTADO_GERENCIAR_MEDICO);
    return estado_atual;
}

// Função para convertere especialidade enum para string
const char *especialidade_string(Especialidade especialidade)
{
    switch (especialidade)
    {
    case CLINICO_GERAL:
        return "Clínico Geral";
    case PEDIATRA:
        return "Pediatra";
    case CARDIOLOGISTA:
        return "Cardiologista";
    case DERMATOLOGISTA:
        return "Dermatologista";
    case PSIQUIATRA:
        return "Psiquiatra";
    default:
        return "Especialidade Desconhecida"; // Retorna uma string para especialidade desconhecida
    }
}
