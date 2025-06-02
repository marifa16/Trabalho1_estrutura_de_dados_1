#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/estruturas.h"
#include "../include/modulo_consulta.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_medico.h"
#include "../include/auxiliar.h"
#include "../include/modulo_gerenciar_paciente.h"
#include "../include/files_manager.h"

Estado tratar_modulo_consulta()
{
    Estado estado_atual = ESTADO_MENU_CONSULTA;
    int escolha = 0;

    do
    {
        msg_01_agendar_consulta();
        escolha = ler_opcao_menu(1, 4);

        if (escolha == -1)
        {
            msg_05_retornando_menu();
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

            if (strlen(cpf_paciente) != 11)
            {
                msg_cpf_invalido();
                break;
            }

            int id_paciente = get_id(ARQ_PACIENTES, 2, cpf_paciente);
            if (id_paciente < 0)
            {
                printf("Paciente com CPF %s não encontrado! Não é possível agendar consulta.\n", cpf_paciente);
                break;
            }

            carregar_medicos_do_arquivo();

            msg_42_exiber_medicos_disponiveis();
            if (total_medicos == 0)
            {
                printf("Nenhum médico cadastrado no sistema.\n");
                break;
            }
            // Substituído por uma função mais robusta se disponível, como exibir_lista_medicos()
            for (int i = 0; i < total_medicos; i++)
            {
                printf("[%d] - %s (CRM: %s, Especialidade: %s)\n",
                       medicos[i].id_medico,
                       medicos[i].nome,
                       medicos[i].crm,
                       especialidade_string(medicos[i].especialidade_medico));
            }
            printf("===========================\n");

            int escolha_id_medico = 0;
            do
            {
                printf("Digite o ID do médico para agendar (ou 0 para sair): ");
                char escolha_medico_str[10];
                limpar_buffer(); // Limpa antes de ler ID do médico
                fgets(escolha_medico_str, sizeof(escolha_medico_str), stdin);
                escolha_medico_str[strcspn(escolha_medico_str, "\n")] = '\0';

                if (sscanf(escolha_medico_str, "%d", &escolha_id_medico) != 1)
                {
                    msg_02_opcao_invalida();
                    continue;
                }
                if (escolha_id_medico == 0)
                {
                    msg_05_retornando_menu();
                    break;
                }

                int medico_valido = 0;
                for (int i = 0; i < total_medicos; ++i)
                {
                    if (medicos[i].id_medico == escolha_id_medico)
                    {
                        medico_valido = 1;
                        break;
                    }
                }
                if (!medico_valido)
                {
                    msg_02_opcao_invalida();
                    printf("ID de médico não encontrado.\n");
                    continue;
                }
                break;
            //limpar_buffer();

            } while (1);

            if (escolha_id_medico == 0)
                break;

            struct tm data_consulta = {0};
            int dia, mes, ano = 2025;
            do
            {
                printf("Digite o mês desejado (1-12) para %d:\n", ano);
                char mes_str[10];
                //limpar_buffer(); // Limpa antes de ler o mês
                fgets(mes_str, sizeof(mes_str), stdin);
                mes_str[strcspn(mes_str, "\n")] = '\0';
                if (sscanf(mes_str, "%d", &mes) != 1 || !validar_mes(mes))
                {
                    msg_02_opcao_invalida();
                    continue;
                }

                printf("Digite o dia desejado para %02d/%d:\n", mes, ano);
                char dia_str[10];
                //limpar_buffer(); // Limpa antes de ler o dia
                fgets(dia_str, sizeof(dia_str), stdin);
                dia_str[strcspn(dia_str, "\n")] = '\0';
                if (sscanf(dia_str, "%d", &dia) != 1 || !validar_dia2(dia, mes, ano))
                {
                    msg_02_opcao_invalida();
                    continue;
                }
                data_consulta.tm_year = ano - 1900;
                data_consulta.tm_mon = mes - 1;
                data_consulta.tm_mday = dia;
                time_t t = mktime(&data_consulta);
                if (t == -1)
                {
                    msg_02_opcao_invalida();
                    printf("Data inválida. Por favor, insira uma data válida.\n");
                    continue;
                }
                struct tm *data_verificada = localtime(&t);
                if (data_verificada->tm_wday == 0)
                {
                    printf("Não há atendimentos aos domingos. Escolha outra data.\n");
                    continue;
                }
                break;
            } while (1);

            int horario_ocupado[9] = {0};
            FILE *arq_cons = fopen(ARQ_CONSULTAS, "r");
            if (arq_cons)
            {
                char linha[512];
                fgets(linha, sizeof(linha), arq_cons);
                while (fgets(linha, sizeof(linha), arq_cons))
                {
                    int id_cons_csv, id_pac_csv, id_med_csv;
                    char data_hora_csv[32], status_csv[16];
                    if (sscanf(linha, "%d,%d,%d,%31[^,],%15[^\n]", &id_cons_csv, &id_pac_csv, &id_med_csv, data_hora_csv, status_csv) == 5)
                    {
                        if (id_med_csv == escolha_id_medico && strcmp(status_csv, "AGENDADA") == 0)
                        {
                            int d_csv, m_csv, a_csv, h_csv, min_csv;
                            if (sscanf(data_hora_csv, "%d/%d/%d %d:%d", &d_csv, &m_csv, &a_csv, &h_csv, &min_csv) == 5)
                            {
                                if (d_csv == dia && m_csv == mes && a_csv == ano)
                                {
                                    if (h_csv >= 8 && h_csv <= 16)
                                        horario_ocupado[h_csv - 8] = 1;
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

            int escolha_horario_idx = 0; // Renomeado para evitar confusão com 'escolha_horario' usado como hora final
            int hora_final_consulta = -1;
            do
            {
                printf("Escolha o índice do horário (1-%d, ou 0 para sair): ", algum_livre > 0 ? 9 : 0); // Ajusta o máx se necessário
                char horario_str[10];
                //limpar_buffer(); // Limpa antes de ler o índice do horário
                fgets(horario_str, sizeof(horario_str), stdin);
                horario_str[strcspn(horario_str, "\n")] = '\0';

                if (sscanf(horario_str, "%d", &escolha_horario_idx) != 1)
                {
                    msg_02_opcao_invalida();
                    continue;
                }
                if (escolha_horario_idx == 0)
                {
                    msg_05_retornando_menu();
                    hora_final_consulta = 0; // Sinaliza para sair
                    break;
                }
                if (escolha_horario_idx < 1 || escolha_horario_idx > 9 || horario_ocupado[escolha_horario_idx - 1])
                {
                    msg_02_opcao_invalida();
                    printf("Horário inválido ou já ocupado.\n");
                    continue;
                }
                hora_final_consulta = 8 + escolha_horario_idx - 1;
                break;
            } while (1);

            if (hora_final_consulta == 0 || hora_final_consulta == -1)
                break;

            char data_hora[32];
            snprintf(data_hora, sizeof(data_hora), "%02d/%02d/%d %02d:00", dia, mes, ano, hora_final_consulta);

            char id_paciente_str[16], id_medico_str[16], status[16];
            snprintf(id_paciente_str, sizeof(id_paciente_str), "%d", id_paciente);
            snprintf(id_medico_str, sizeof(id_medico_str), "%d", escolha_id_medico);
            strcpy(status, "AGENDADA");

            char *valores_consulta[4];
            valores_consulta[0] = id_paciente_str;
            valores_consulta[1] = id_medico_str;
            valores_consulta[2] = data_hora;
            valores_consulta[3] = status;

            // Ajuste para add_row: select_col deve ser o número de colunas de DADOS + 1 (para o ID gerado)
            // Se add_row espera 4 valores de dados, e gera o ID, select_col deve ser 5.
            if (add_row(ARQ_CONSULTAS, 5, valores_consulta))
            {
                msg_08_sucesso_agendamento();
                char nome_medico_agendado[100] = "N/D";
                for (int i = 0; i < total_medicos; ++i)
                {
                    if (medicos[i].id_medico == escolha_id_medico)
                    {
                        strncpy(nome_medico_agendado, medicos[i].nome, sizeof(nome_medico_agendado) - 1);
                        nome_medico_agendado[sizeof(nome_medico_agendado) - 1] = '\0';
                        break;
                    }
                }
                printf("Consulta marcada para %s com o médico %s.\n", data_hora, nome_medico_agendado);
            }
            else
            {
                printf("Erro ao salvar a consulta.\n");
            }
            limpar_buffer(); // Limpa o buffer no final do case
            break;
        }

        case 2: // Cancelar Consulta
        {
            int tipo_usuario = 0;
            do
            {
                printf("===========================\n");
                printf("Você está acessando como:\n[1] - Médico\n[2] - Paciente\n[3] - Sair\n");
                tipo_usuario = ler_opcao_menu(1, 3);
                if (tipo_usuario == -1)
                {
                    msg_05_retornando_menu();
                    limpar_buffer();
                    break;
                }
                break;
            } while (tipo_usuario == -1); // Repete se ler_opcao_menu falhou

            if (tipo_usuario == 3 || tipo_usuario == -1)
            {
                if (tipo_usuario == 3)
                    msg_05_retornando_menu();

                limpar_buffer(); // Limpa antes de sair do case
                break;
            }

            int id_usuario_sistema = -1;
            char id_usuario_str[16];

            if (tipo_usuario == 1)
            {
                char crm[16];
                msg_30_crm();
                fgets(crm, sizeof(crm), stdin);
                crm[strcspn(crm, "\n")] = '\0';
                id_usuario_sistema = get_id(ARQ_MEDICOS, 2, crm);
                if (id_usuario_sistema < 0)
                {
                    msg_medico_nao_encontrado(crm);
                    break;
                }
                snprintf(id_usuario_str, sizeof(id_usuario_str), "%d", id_usuario_sistema);
                printf("Consultas do médico (CRM: %s):\n", crm);
                exibir_arquivo(ARQ_CONSULTAS, "medico", id_usuario_str);
            }
            else if (tipo_usuario == 2)
            {
                char cpf[12];
                msg_13_informar_cpf();
                fgets(cpf, sizeof(cpf), stdin);
                cpf[strcspn(cpf, "\n")] = '\0';
                id_usuario_sistema = get_id(ARQ_PACIENTES, 2, cpf);
                if (id_usuario_sistema < 0)
                {
                    msg_paciente_nao_encontrado(cpf);
                    break;
                }
                snprintf(id_usuario_str, sizeof(id_usuario_str), "%d", id_usuario_sistema);
                printf("Consultas do paciente (CPF: %s):\n", cpf);
                exibir_arquivo(ARQ_CONSULTAS, "paciente", id_usuario_str);
            }

            if (id_usuario_sistema < 0)
            { // Se não encontrou usuário ou saiu antes
                break;
            }

            int id_consulta_cancelar = -1;
            int linha_para_atualizar = -1;
            do
            {
                printf("Digite o ID da consulta que deseja cancelar (ou 0 para sair): ");
                char id_consulta_str[16];

                fgets(id_consulta_str, sizeof(id_consulta_str), stdin);
                id_consulta_str[strcspn(id_consulta_str, "\n")] = '\0';

                if (strlen(id_consulta_str) == 0)
                { // Se o usuário apenas pressionar Enter
                    msg_02_opcao_invalida();
                    continue;
                }
                if (sscanf(id_consulta_str, "%d", &id_consulta_cancelar) != 1)
                {
                    msg_02_opcao_invalida();
                    continue;
                }
                if (id_consulta_cancelar == 0)
                {
                    msg_05_retornando_menu();
                    break;
                }

                FILE *arq = fopen(ARQ_CONSULTAS, "r");
                if (!arq)
                {
                    msg_erro_abrir_arquivo_nome(ARQ_CONSULTAS);
                    break;
                }
                char linha_csv[512];
                fgets(linha_csv, sizeof(linha_csv), arq);

                int achou_consulta_valida = 0;
                int contador_linha_dados = 0;
                char csv_id_consulta_atual[16], csv_id_pac_atual[16], csv_id_med_atual[16], csv_status_atual[16];

                while (fgets(linha_csv, sizeof(linha_csv), arq))
                {
                    // Zera as strings para evitar lixo de leituras anteriores
                    memset(csv_id_consulta_atual, 0, sizeof(csv_id_consulta_atual));
                    memset(csv_id_pac_atual, 0, sizeof(csv_id_pac_atual));
                    memset(csv_id_med_atual, 0, sizeof(csv_id_med_atual));
                    memset(csv_status_atual, 0, sizeof(csv_status_atual));

                    if (sscanf(linha_csv, "%15[^,],%15[^,],%15[^,],%*[^,],%15[^\n]",
                               csv_id_consulta_atual, csv_id_pac_atual, csv_id_med_atual, csv_status_atual) != 4)
                    {
                        // Se não conseguiu ler os 4 primeiros campos, pode ser uma linha mal formatada ou em branco no final.
                        // O status é o quinto, então o sscanf acima está pegando o status.
                        // Para pegar o status corretamente:
                        sscanf(linha_csv, "%*[^,],%*[^,],%*[^,],%*[^,],%15[^\n]", csv_status_atual); // Re-ler só o status se necessário
                    }
                    // Re-ler a linha completa para pegar todos os 5 campos para verificação
                    if (sscanf(linha_csv, "%15[^,],%15[^,],%15[^,],%*[^,],%15[^\n]",
                               csv_id_consulta_atual, csv_id_pac_atual, csv_id_med_atual, csv_status_atual) != 4)
                    {
                        // Se o sscanf falhar em parsear 4 campos (id_consulta, id_paciente, id_medico, status), a linha está malformada.
                        // O formato é id_consulta,id_paciente,id_medico,data_hora,status
                        // Então precisamos de 5 especificadores de formato para sscanf.
                        if (sscanf(linha_csv, "%15[^,],%15[^,],%15[^,],%*31[^,],%15[^\n]", // Ignora data_hora
                                   csv_id_consulta_atual, csv_id_pac_atual, csv_id_med_atual, csv_status_atual) != 4)
                        {
                            continue; // Pula linha mal formada
                        }
                    }

                    if (atoi(csv_id_consulta_atual) == id_consulta_cancelar)
                    {
                        if ((tipo_usuario == 1 && strcmp(csv_id_med_atual, id_usuario_str) == 0) ||
                            (tipo_usuario == 2 && strcmp(csv_id_pac_atual, id_usuario_str) == 0))
                        {
                            if (strcmp(csv_status_atual, "CANCELADA") == 0)
                            {
                                printf("Esta consulta (ID: %d) já está cancelada.\n", id_consulta_cancelar);
                                achou_consulta_valida = -1;
                            }
                            else
                            {
                                achou_consulta_valida = 1;
                            }
                            linha_para_atualizar = contador_linha_dados;
                            break;
                        }
                    }
                    contador_linha_dados++;
                }
                fclose(arq);

                if (achou_consulta_valida == 0)
                {
                    printf("ID da consulta inválido ou não pertence a você. Tente novamente.\n");
                    if (tipo_usuario == 1)
                        exibir_arquivo(ARQ_CONSULTAS, "medico", id_usuario_str);
                    else
                        exibir_arquivo(ARQ_CONSULTAS, "paciente", id_usuario_str);
                    continue;
                }
                if (achou_consulta_valida == -1)
                {
                    continue;
                }

                printf("Deseja realmente cancelar a consulta ID %d?\n[1] - SIM\n[2] - NÃO\n[3] - SAIR\n", id_consulta_cancelar);
                int resp_confirmacao = ler_opcao_menu(1, 3);

                if (resp_confirmacao == 1)
                {
                    FILE *arq_leitura_att = fopen(ARQ_CONSULTAS, "r");
                    if (!arq_leitura_att)
                    {
                        msg_erro_abrir_arquivo_nome(ARQ_CONSULTAS);
                        break;
                    }
                    char linha_original_att[512];
                    char val_id_cons[16], val_id_pac[16], val_id_med[16], val_data_hora[32], val_status_antigo[16];

                    fgets(linha_original_att, sizeof(linha_original_att), arq_leitura_att);
                    for (int k = 0; k <= linha_para_atualizar; ++k)
                    {
                        if (fgets(linha_original_att, sizeof(linha_original_att), arq_leitura_att) == NULL)
                        {
                            strcpy(val_id_cons, "ERRO");
                            break;
                        }
                    }
                    fclose(arq_leitura_att);

                    if (strcmp(val_id_cons, "ERRO") != 0 && sscanf(linha_original_att, "%[^,],%[^,],%[^,],%[^,],%s", val_id_cons, val_id_pac, val_id_med, val_data_hora, val_status_antigo) == 5)
                    {
                        char *valores_atualizados[5];
                        valores_atualizados[0] = val_id_cons;
                        valores_atualizados[1] = val_id_pac;
                        valores_atualizados[2] = val_id_med;
                        valores_atualizados[3] = val_data_hora;
                        valores_atualizados[4] = "CANCELADA";

                        // att_row espera (nome_arquivo, linha_a_atualizar (0-indexed), num_colunas_total, array_valores)
                        // O array_valores deve conter todos os campos, incluindo o ID.
                        if (att_row(ARQ_CONSULTAS, linha_para_atualizar, 5, valores_atualizados))
                        {
                            printf("Consulta ID %d cancelada com sucesso!\n", id_consulta_cancelar);
                        }
                        else
                        {
                            printf("Erro ao atualizar o status da consulta ID %d.\n", id_consulta_cancelar);
                        }
                    }
                    else
                    {
                        printf("Erro ao ler os dados da consulta para cancelamento.\n");
                    }
                    break;
                }
                else if (resp_confirmacao == 2)
                {
                    printf("Cancelamento da consulta ID %d abortado.\n", id_consulta_cancelar);
                    if (tipo_usuario == 1)
                        exibir_arquivo(ARQ_CONSULTAS, "medico", id_usuario_str);
                    else
                        exibir_arquivo(ARQ_CONSULTAS, "paciente", id_usuario_str);
                    continue;
                }
                else if (resp_confirmacao == 3 || resp_confirmacao == -1)
                {
                    msg_05_retornando_menu();
                    break;
                }
            } while (id_consulta_cancelar != 0);
            break;
        }

        case 3:
            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;

        case 4:
            estado_atual = ESTADO_SAIR;
            break;

        default:
            break;
        }
    } while (estado_atual == ESTADO_MENU_CONSULTA);

    return estado_atual;
}
