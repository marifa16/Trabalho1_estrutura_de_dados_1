#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estruturas.h"
#include "../include/modulo_consulta.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_medico.h"
#include "../include/validacoes.h"
#include "../include/auxiliar.h"
#include "../include/modulo_gerenciar_paciente.h"

// Vetor global
reg_consulta *consultas = NULL; // Ponteiro para vetor consultas
int total_consultas = 0;        // Contador

Estado tratar_modulo_consulta()
{
    Estado estado_atual = ESTADO_MENU_CONSULTA; // Inicializa o estado como ESTADO_MENU_CONSULTA
    int escolha = 0;
    char escolha_str[10]; // Variável para armazenar a entrada do usuário

    do
    {
        // Exibe o menu de consultas
        msg_01_agendar_consulta();
        fgets(escolha_str, sizeof(escolha_str), stdin); // Lê a entrada do usuário como string
        escolha_str[strcspn(escolha_str, "\n")] = '\0'; // Remove o caractere de nova linha

        // Converte a entrada para inteiro
        if (sscanf(escolha_str, "%d", &escolha) != 1)
        {
            msg_02_opcao_invalida();
            continue;
        }

        switch (escolha)
        {
        case 1: // Agendar Consulta
        {
            char cpf_paciente[12];

            msg_31_cpf();
            fgets(cpf_paciente, sizeof(cpf_paciente), stdin);
            cpf_paciente[strcspn(cpf_paciente, "\n")] = '\0';

            // Verifica se o CPF existe no arquivo de pacientes
            int linha_paciente = buscar_linha("data/registro_pacientes.csv", 2, cpf_paciente);
            if (linha_paciente < 0)
            {
                printf("Paciente com CPF %s não encontrado! Não é possível agendar consulta.\n", cpf_paciente);
                return ESTADO_MENU_CONSULTA;
            }

            int crm_medico, mes;

            // Exibe médicos disponíveis
            msg_42_exiber_medicos_disponiveis();
            for (int i = 0; i < total_medicos; i++)
            {
                printf("[%d] - %s (CRM: %d, Especialidade: %s)\n", i + 1, medicos[i].nome, medicos[i].crm, especialidade_string(medicos[i].especialidade_medico));
            }

            // Escolha do médico
            int escolha_medico = 0;      // Variável para armazenar a escolha do médico
            char escolha_medico_str[10]; // escolha do médico como string
            do
            {
                msg_06_escolher_medico();
                fgets(escolha_medico_str, sizeof(escolha_medico_str), stdin); // Lê a entrada do usuário como string
                escolha_medico_str[strcspn(escolha_medico_str, "\n")] = '\0'; // Remove o caractere de nova linha

                // Converte a entrada para inteiro
                if (sscanf(escolha_medico_str, "%d", &escolha_medico) != 1 || escolha_medico < 1 || escolha_medico > total_medicos)
                {
                    msg_02_opcao_invalida();
                    continue; // Volta para solicitar a escolha do médico
                }

                crm_medico = medicos[escolha_medico - 1].crm; // Obtém o CRM do médico escolhido
                printf("Você escolheu: %s\nCRM: %d\nEspecialidade: %s\n", medicos[escolha_medico - 1].nome, crm_medico, especialidade_string(medicos[escolha_medico - 1].especialidade_medico));

                // Função Validar
                int opcao = validar_opcao_usuario();
                if (opcao == 1) // SIM
                {
                    break; // Sai do loop
                }
                else if (opcao == 2) // NÃO
                {
                    continue; // Volta para solicitar a escolha do médico
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    return ESTADO_MENU_PRINCIPAL; // Retorna ao menu principal
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);

            // Escolher data
            struct tm data_consulta = {0};
            data_consulta.tm_year = 2025 - 1900; // Define o ano (2025)

            do
            {
                char dia_str[10], mes_str[10];

                // Solicita o mês
                printf("Digite o mês desejado (1-12):\n");
                fgets(mes_str, sizeof(mes_str), stdin); // Lê a entrada do usuário como string
                mes_str[strcspn(mes_str, "\n")] = '\0'; // Remove o caractere de nova linha
                // Converte a entrada para inteiro
                if (sscanf(mes_str, "%d", &mes) != 1 || mes < 1 || mes > 12)
                {
                    msg_02_opcao_invalida();
                    continue; // Volta para solicitar o mês
                }
                data_consulta.tm_mon = mes - 1; // Define o mês (0-11)

                // Solicita o dia
                printf("Digite o dia desejado:\n");
                fgets(dia_str, sizeof(dia_str), stdin); // Lê a entrada como string
                dia_str[strcspn(dia_str, "\n")] = '\0'; // Remove o caractere de nova linha
                // Converte a ebtrada para inteiro
                if (sscanf(dia_str, "%d", &data_consulta.tm_mday) != 1 || data_consulta.tm_mday < 1)
                {
                    msg_02_opcao_invalida();
                    continue;
                }

                // Valida a data
                if (mktime(&data_consulta) == -1)
                {
                    msg_02_opcao_invalida();
                    printf("Data inválida. Por favor, insira uma data válida.\n");
                    continue; // Volta a solicitar a data
                }

                // Verifica se é domingo
                if (data_consulta.tm_wday == 0)
                {
                    printf("Não há atendimentos aos domingos. Escolha outra data.\n");
                    continue; // Volta para solicitar a data
                }

                // Exibe a data escolhida
                printf("Data escolhida: %02d/%02d/%d\n", data_consulta.tm_mday, data_consulta.tm_mon + 1, data_consulta.tm_year + 1900);

                // Função Validar
                int opcao = validar_opcao_usuario();
                if (opcao == 1) // SIM
                {
                    break; // Sai do loop
                }
                else if (opcao == 2) // NÃO
                {
                    continue; // Volta para solicitar a data novamente
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    return ESTADO_MENU_CONSULTA; // Retorna ao menu de consulta
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);

            // Exibe horários livres do médico selecionado
            int horario_escolhido = -1;

            do
            {
                printf("Horários disponimes para o médico %s no dia %02d/%02d/%d:\n", medicos[escolha_medico - 1].nome, data_consulta.tm_mday, data_consulta.tm_mon + 1, data_consulta.tm_year + 1900);

                int horario_livre = 0;
                for (int i = 0; i < 9; i++) // Horários das 8h às 16h
                {
                    medicos[total_medicos].horarios[i].hora = 8 + i;         // Define as horas (8h, 9h, ..., 16h)
                    medicos[total_medicos].horarios[i].ocupado = 0;          // Marca todos os horários como livres
                    strcpy(medicos[total_medicos].horarios[i].paciente, ""); // Inicializa o nome do paciente como vazio
                    strcpy(medicos[total_medicos].horarios[i].medico, "");   // Inicializa o nome do médico como vazio
                }
                if (!horario_livre)
                {
                    printf("Não há horário disponível para esse médico neste dia. Escolha outro médico ou data.\n");
                    return ESTADO_MENU_CONSULTA; // Retorna ao menu consulta
                }

                // Solicita o horário
                printf("Escolha um horário:\n");
                char horario_str[10];
                fgets(horario_str, sizeof(horario_str), stdin); // Lê o horário como string
                horario_str[strcspn(horario_str, "\n")] = '\0'; // Remove o caractere de nova linha

                // Converte para inteiro
                if (sscanf(horario_str, "%d", &horario_escolhido) != 1 || horario_escolhido < 8 || horario_escolhido > 16)
                {
                    msg_02_opcao_invalida();
                    continue; // Volta para solicitar o horário
                }

                // Verifica se o horário está ocupado
                int indice = horario_escolhido - 8; // Calcula o índice no vetor
                if (medicos[escolha_medico - 1].horarios[indice].ocupado)
                {
                    printf("O horário %02d:00 já está ocupado. Escolha outro horário.\n", horario_escolhido);
                    continue;
                }

                // Exibe o horário escolhido
                printf("Horário escolhido: %02d:00\n", horario_escolhido);

                // Função Validar
                int opcao = validar_opcao_usuario();
                if (opcao == 1) // SIM
                {
                    // Marca o horário
                    medicos[escolha_medico - 1].horarios[indice].ocupado = 1;
                    strcpy(medicos[escolha_medico - 1].horarios[indice].paciente, "Nome do Paciente"); // Armazena o nome do paciente que agendou a consulta naquele horário

                    msg_08_sucesso_agendamento();
                    printf("Marcada para %02d/%02d/%d às %02d:00 com o médico %s.\n", data_consulta.tm_mday, data_consulta.tm_mon + 1, data_consulta.tm_year + 1900, horario_escolhido, medicos[escolha_medico - 1].nome);
                    return ESTADO_MENU_CONSULTA;
                }
                else if (opcao == 2) // NÃO
                {
                    continue; // Volta a solicitar o horario
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    return ESTADO_MENU_CONSULTA;
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);
        }

        case 2: // Cancelar
            printf("Cancelamento de consulta selecionado.\n");
            estado_atual = ESTADO_MENU_CONSULTA;
            break;

        case 3: // Retornar ao menu principal
            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;

        case 4: // Sair do programa
            estado_atual = ESTADO_SAIR;
            break;

        default:
            msg_02_opcao_invalida();
            break;
        }
    } while (estado_atual == ESTADO_MENU_CONSULTA);

    return estado_atual;
}
