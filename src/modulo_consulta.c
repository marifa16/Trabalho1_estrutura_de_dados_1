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
#include "../include/files_manager.h"

// Vetor global
reg_consulta *consultas = NULL; // Ponteiro para vetor consultas
int total_consultas = 0;        // Contador

const char *especialidade_string(Especialidade especialidade);

void carregar_medicos_do_arquivo(void);

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

            // Valida paciente
            int id_paciente = get_id("data/registro_pacientes.csv", 2, cpf_paciente);
            if (id_paciente < 0)
            {
                printf("Paciente com CPF %s não encontrado! Não é possível agendar consulta.\n", cpf_paciente);
                break;
            }

            // Carrega médicos do arquivo antes de listar
            carregar_medicos_do_arquivo();

            // Lista médicos
            msg_42_exiber_medicos_disponiveis();
            if (total_medicos == 0)
            {
                printf("Nenhum médico cadastrado no sistema.\n");
                break;
            }
            for (int i = 0; i < total_medicos; i++)
            {
                printf("[%d] - %s (CRM: %s, Especialidade: %s)\n",
                       i + 1,
                       medicos[i].nome,
                       medicos[i].crm,
                       especialidade_string(medicos[i].especialidade_medico));
            }

            // Escolher médico
            int escolha_medico = 0;
            do
            {
                msg_06_escolher_medico();
                printf("[0] - Sair\n");
                char escolha_medico_str[10];
                limpar_buffer();
                fgets(escolha_medico_str, sizeof(escolha_medico_str), stdin);
                escolha_medico_str[strcspn(escolha_medico_str, "\n")] = '\0';

                // DEBUG: Mostra o que foi lido
                // printf("DEBUG: escolha_medico_str = '%s'\n", escolha_medico_str);

                // Valida se é um número e se está no intervalo de médicos cadastrados
                if (sscanf(escolha_medico_str, "%d", &escolha_medico) != 1 ||
                    escolha_medico < 0 || escolha_medico > total_medicos)
                {
                    msg_02_opcao_invalida();
                    continue;
                }
                if (escolha_medico == 0)
                {
                    msg_05_retornando_menu();
                    return estado_atual; // Ou outro fluxo para sair do agendamento
                }
                break;
            } while (1);

            int id_medico = medicos[escolha_medico - 1].id_medico;

            // Escolher data
            struct tm data_consulta = {0};
            int dia, mes, ano = 2025;
            do
            {
                printf("Digite o mês desejado (1-12):\n");
                char mes_str[10];
                fgets(mes_str, sizeof(mes_str), stdin);
                mes_str[strcspn(mes_str, "\n")] = '\0';
                if (sscanf(mes_str, "%d", &mes) != 1 || mes < 1 || mes > 12)
                {
                    msg_02_opcao_invalida();
                    continue;
                }
                printf("Digite o dia desejado:\n");
                char dia_str[10];
                fgets(dia_str, sizeof(dia_str), stdin);
                dia_str[strcspn(dia_str, "\n")] = '\0';
                if (sscanf(dia_str, "%d", &dia) != 1 || dia < 1 || dia > 31)
                {
                    msg_02_opcao_invalida();
                    continue;
                }
                data_consulta.tm_year = ano - 1900;
                data_consulta.tm_mon = mes - 1;
                data_consulta.tm_mday = dia;
                if (mktime(&data_consulta) == -1)
                {
                    msg_02_opcao_invalida();
                    printf("Data inválida. Por favor, insira uma data válida.\n");
                    continue;
                }
                if (data_consulta.tm_wday == 0)
                {
                    printf("Não há atendimentos aos domingos. Escolha outra data.\n");
                    continue;
                }
                break;
            } while (1);

            // Lista horários disponíveis (8h às 16h)
            int horario_ocupado[9] = {0};
            FILE *arq_cons = fopen("data/registro_consultas.csv", "r");
            if (arq_cons)
            {
                char linha[512];
                fgets(linha, sizeof(linha), arq_cons); // pula cabeçalho
                while (fgets(linha, sizeof(linha), arq_cons))
                {
                    int id_cons, id_pac, id_med;
                    char data_hora[32], status[16];
                    if (sscanf(linha, "%d,%d,%d,%31[^,],%15[^\n]", &id_cons, &id_pac, &id_med, data_hora, status) == 5)
                    {
                        if (id_med == id_medico)
                        {
                            int d, m, a, h, min;
                            if (sscanf(data_hora, "%d/%d/%d %d:%d", &d, &m, &a, &h, &min) == 5)
                            {
                                if (d == dia && m == mes && a == ano)
                                {
                                    if (h >= 8 && h <= 16)
                                        horario_ocupado[h - 8] = 1;
                                }
                            }
                        }
                    }
                }
                fclose(arq_cons);
            }

            printf("Horários disponíveis para %02d/%02d/%d:\n", dia, mes, ano);
            int algum_livre = 0;
            for (int i = 0; i < 9; i++)
            {
                if (!horario_ocupado[i])
                {
                    printf("[%d] - %02d:00\n", i + 1, 8 + i);
                    algum_livre = 1;
                }
            }
            if (!algum_livre)
            {
                printf("Não há horários disponíveis para este médico nesta data.\n");
                break;
            }

            // Escolher horário
            int escolha_horario = 0;
            do
            {
                printf("Escolha o horário (1-9): ");
                char horario_str[10];
                fgets(horario_str, sizeof(horario_str), stdin);
                horario_str[strcspn(horario_str, "\n")] = '\0';
                if (sscanf(horario_str, "%d", &escolha_horario) != 1 || escolha_horario < 1 || escolha_horario > 9 || horario_ocupado[escolha_horario - 1])
                {
                    msg_02_opcao_invalida();
                    continue;
                }
                break;
            } while (1);

            // Monta data/hora da consulta
            char data_hora[32];
            snprintf(data_hora, sizeof(data_hora), "%02d/%02d/%d %02d:00", dia, mes, ano, 8 + escolha_horario - 1);

            // Adiciona consulta no arquivo
            char id_paciente_str[16], id_medico_str[16], status[16];
            snprintf(id_paciente_str, sizeof(id_paciente_str), "%d", id_paciente);
            snprintf(id_medico_str, sizeof(id_medico_str), "%d", id_medico);
            strcpy(status, "AGENDADA");

            char *valores[4];
            valores[0] = id_paciente_str;
            valores[1] = id_medico_str;
            valores[2] = data_hora;
            valores[3] = status;

            add_row("data/registro_consultas.csv", 5, valores);

            msg_08_sucesso_agendamento();
            printf("Consulta marcada para %02d/%02d/%d às %02d:00 com o médico %s.\n",
                   dia, mes, ano, 8 + escolha_horario - 1, medicos[escolha_medico - 1].nome);
            break;
        }

        case 2: // Cancelar Consulta
        {
            int tipo_usuario = 0;
            do
            {
                printf("===========================\n");
                printf("Você está acessando como:\n[1] - Médico\n[2] - Paciente\n[3] - Sair\n");
                char tipo_str[10];
                fgets(tipo_str, sizeof(tipo_str), stdin);
                tipo_str[strcspn(tipo_str, "\n")] = '\0';
                if (sscanf(tipo_str, "%d", &tipo_usuario) != 1 || tipo_usuario < 1 || tipo_usuario > 3)
                {
                    msg_02_opcao_invalida();
                    continue;
                }
                break;
            } while (1);

            if (tipo_usuario == 3)
            {
                msg_05_retornando_menu();
                return ESTADO_MENU_CONSULTA;
            }

            int id_usuario = -1;
            char id_usuario_str[16];
            if (tipo_usuario == 1)
            {
                // Médico
                char crm[16];
                msg_30_crm();
                fgets(crm, sizeof(crm), stdin);
                crm[strcspn(crm, "\n")] = '\0';
                id_usuario = get_id("data/registro_medicos.csv", 2, crm);
                if (id_usuario < 0)
                {
                    msg_medico_nao_encontrado(crm);
                    return ESTADO_MENU_CONSULTA;
                }
                snprintf(id_usuario_str, sizeof(id_usuario_str), "%d", id_usuario);
                printf("Consultas do médico (CRM: %s):\n", crm);
                exibir_arquivo("data/registro_consultas.csv", "medico", id_usuario_str);
            }
            else if (tipo_usuario == 2)
            {
                // Paciente
                char cpf[12];
                msg_13_informar_cpf();
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';
                id_usuario = get_id("data/registro_pacientes.csv", 2, cpf);
                if (id_usuario < 0)
                {
                    msg_paciente_nao_encontrado(cpf);
                    return ESTADO_MENU_CONSULTA;
                }
                snprintf(id_usuario_str, sizeof(id_usuario_str), "%d", id_usuario);
                printf("Consultas do paciente (CPF: %s):\n", cpf);
                exibir_arquivo("data/registro_consultas.csv", "paciente", id_usuario_str);
            }

            // Pergunta qual consulta será desmarcada
            int id_consulta = -1;
            do
            {
                printf("Digite o ID da consulta que deseja cancelar (ou 0 para sair): ");
                char id_consulta_str[16];
                fgets(id_consulta_str, sizeof(id_consulta_str), stdin);
                id_consulta_str[strcspn(id_consulta_str, "\n")] = '\0';
                if (sscanf(id_consulta_str, "%d", &id_consulta) != 1)
                {
                    msg_02_opcao_invalida();
                    continue;
                }
                if (id_consulta == 0)
                {
                    msg_05_retornando_menu();
                    return ESTADO_MENU_CONSULTA;
                }

                // Verifica se a consulta existe e pertence ao usuário
                FILE *arq = fopen("data/registro_consultas.csv", "r");
                if (!arq)
                {
                    msg_erro_abrir_arquivo();
                    return ESTADO_MENU_CONSULTA;
                }
                char linha[512];
                fgets(linha, sizeof(linha), arq); // pula cabeçalho
                int achou = 0;
                int linha_atual = 0;
                while (fgets(linha, sizeof(linha), arq))
                {
                    int id, id_pac, id_med;
                    char data_hora[32], status[16];
                    if (sscanf(linha, "%d,%d,%d,%31[^,],%15[^\n]", &id, &id_pac, &id_med, data_hora, status) == 5)
                    {
                        if (id == id_consulta)
                        {
                            if ((tipo_usuario == 1 && id_med == id_usuario) ||
                                (tipo_usuario == 2 && id_pac == id_usuario))
                            {
                                achou = 1;
                                break;
                            }
                        }
                    }
                    linha_atual++;
                }
                fclose(arq);

                if (!achou)
                {
                    printf("Consulta não encontrada ou não pertence ao usuário informado.\n");
                    exibir_arquivo("data/registro_consultas.csv", tipo_usuario == 1 ? "medico" : "paciente", id_usuario_str);
                    continue;
                }

                // Confirmação do cancelamento
                printf("Deseja realmente cancelar esta consulta?\n[1] - SIM\n[2] - NÃO\n[3] - SAIR\n");
                char resp[10];
                fgets(resp, sizeof(resp), stdin);
                int resp_int = atoi(resp);
                if (resp_int == 1)
                {
                    // Atualiza status para CANCELADA
                    // Lê a linha da consulta para montar os valores
                    FILE *arq2 = fopen("data/registro_consultas.csv", "r");
                    if (!arq2)
                    {
                        msg_erro_abrir_arquivo();
                        return ESTADO_MENU_CONSULTA;
                    }
                    fgets(linha, sizeof(linha), arq2); // pula cabeçalho
                    int linha_idx = 0;
                    char linha_csv[512];
                    while (fgets(linha_csv, sizeof(linha_csv), arq2))
                    {
                        if (linha_idx == linha_atual)
                            break;
                        linha_idx++;
                    }
                    fclose(arq2);

                    // Extrai os campos da linha
                    char id_str[8], id_pac_str[8], id_med_str[8], data_hora[32], status[16];
                    char *token = strtok(linha_csv, ",\n");
                    int i = 0;
                    while (token && i < 5)
                    {
                        switch (i)
                        {
                        case 0:
                            strncpy(id_str, token, sizeof(id_str));
                            id_str[sizeof(id_str) - 1] = '\0';
                            break;
                        case 1:
                            strncpy(id_pac_str, token, sizeof(id_pac_str));
                            id_pac_str[sizeof(id_pac_str) - 1] = '\0';
                            break;
                        case 2:
                            strncpy(id_med_str, token, sizeof(id_med_str));
                            id_med_str[sizeof(id_med_str) - 1] = '\0';
                            break;
                        case 3:
                            strncpy(data_hora, token, sizeof(data_hora));
                            data_hora[sizeof(data_hora) - 1] = '\0';
                            break;
                        case 4:
                            strncpy(status, token, sizeof(status));
                            status[sizeof(status) - 1] = '\0';
                            break;
                        }
                        token = strtok(NULL, ",\n");
                        i++;
                    }
                    strcpy(status, "CANCELADA");
                    char *valores[5];
                    valores[0] = id_str;
                    valores[1] = id_pac_str;
                    valores[2] = id_med_str;
                    valores[3] = data_hora;
                    valores[4] = status;
                    att_row("data/registro_consultas.csv", linha_atual, 5, valores);
                    printf("Consulta cancelada com sucesso!\n");
                    break;
                }
                else if (resp_int == 2)
                {
                    exibir_arquivo("data/registro_consultas.csv", tipo_usuario == 1 ? "medico" : "paciente", id_usuario_str);
                    continue;
                }
                else if (resp_int == 3)
                {
                    msg_05_retornando_menu();
                    return ESTADO_MENU_CONSULTA;
                }
                else
                {
                    msg_02_opcao_invalida();
                    continue;
                }
            } while (1);

            break;
        }

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

void carregar_medicos_do_arquivo()
{
    FILE *arquivo = fopen("data/registro_medicos.csv", "r");
    if (!arquivo)
        return;

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho

    // Libera vetor antigo, se houver
    if (medicos)
    {
        free(medicos);
        medicos = NULL;
        total_medicos = 0;
    }

    while (fgets(linha, sizeof(linha), arquivo))
    {
        reg_medico temp;
        int id;
        char especialidade[100];
        if (sscanf(linha, "%d,%99[^,],%15[^,],%99[^,],%15[^,\n]",
                   &id, temp.nome, temp.crm, especialidade, temp.telefone) == 5)
        {
            temp.id_medico = id;
            strncpy(temp.especialidade, especialidade, sizeof(temp.especialidade));
            temp.especialidade[sizeof(temp.especialidade) - 1] = '\0';
            // Descobre o enum da especialidade
            for (int i = 0; i < TOTAL_ESPECIALIDADES; i++)
            {
                if (strcmp(especialidade, especialidade_string(i)) == 0)
                {
                    temp.especialidade_medico = i;
                    break;
                }
            }
            medicos = realloc(medicos, (total_medicos + 1) * sizeof(reg_medico));
            medicos[total_medicos++] = temp;
        }
    }
    fclose(arquivo);
}
