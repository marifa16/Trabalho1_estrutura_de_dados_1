#include <stdio.h>
#include <string.h>
#include "../include/estruturas.h"
#include "../include/mensagens.h"
#include "../include/modulo_relatorios.h"
#include "../include/auxiliar.h"
#include "../include/files_manager.h"

Estado tratar_modulo_relatorios()
{
    Estado estado_atual = ESTADO_MODULO_RELATORIOS;
    int escolha_modulo;
    do
    {
        msg_39_mostrar_modulo_relatorios();
        escolha_modulo = ler_opcao_menu(1, 7);

        switch (escolha_modulo)
        {
        case 1: // Consultas de pacientes
        {
            char cpf[12];
            msg_13_informar_cpf();
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = '\0'; // Remove o caractere de nova linha

            int id_paciente = get_id(ARQ_PACIENTES, 2, cpf);
            if (id_paciente == -1)
            {
                msg_paciente_nao_encontrado(cpf);
                break;
            }

            // Exibe consultas relacionadas ao paciente
            char id_paciente_str[16];
            snprintf(id_paciente_str, sizeof(id_paciente_str), "%d", id_paciente);
            printf("Consultas do paciente (CPF: %s):\n", cpf);
            exibir_arquivo(ARQ_CONSULTAS, "paciente", id_paciente_str);

            limpar_buffer();
            break;
        }

        case 2: // Consultas de médicos
        {
            char crm_str[16];
            printf("Digite o CRM do médico: ");
            fgets(crm_str, sizeof(crm_str), stdin);
            crm_str[strcspn(crm_str, "\n")] = '\0';

            // Valida se o médico existe
            int id_medico = get_id(ARQ_MEDICOS, 2, crm_str);
            if (id_medico == -1)
            {
                msg_medico_nao_encontrado(crm_str);
                break;
            }

            // Exibe consultas relacionadas ao médico
            char id_medico_str[16];
            snprintf(id_medico_str, sizeof(id_medico_str), "%d", id_medico);
            printf("Consultas do médico (CRM: %s):\n", crm_str);
            exibir_arquivo(ARQ_CONSULTAS, "medico", id_medico_str);

            limpar_buffer();
            break;
        }

        case 3: // Consulta apenas pela especialidade
        {
            int opcao;
            char *especialidades[] = {
                "CLINICO_GERAL",
                "PEDIATRA",
                "CARDIOLOGISTA",
                "DERMATOLOGISTA",
                "PSIQUIATRA"};

            msg_42_especialidade_opcoes();
            printf("Digite o número da especialidade: ");
            opcao = ler_opcao_menu(0, 5); // 0 para sair, 1-5 para especialidades

            if (opcao == 0)
            {
                break; // Volta ao menu de relatórios
            }
            if (opcao < 1 || opcao > 5)
            {
                msg_02_opcao_invalida();
                break;
            }

            // Exibe consultas pela especialidade selecionada
            char *especialidade_selecionada = especialidades[opcao - 1];
            printf("Consultas da especialidade %s:\n", especialidade_selecionada);
            exibir_arquivo(ARQ_CONSULTAS, "especialidade", especialidade_selecionada);

            limpar_buffer();
            break;
        }

        case 4: // Consultas do dia atual
        {
            char data_atual_sistema[11]; // AAAA-MM-DD
            obter_data_atual(data_atual_sistema);

            char data_formatada_para_csv[11]; // DD/MM/AAAA
            // Converte AAAA-MM-DD para DD/MM/AAAA
            // data_atual_sistema: YYYY-MM-DD
            // data_formatada_para_csv: DD/MM/YYYY
            snprintf(data_formatada_para_csv, sizeof(data_formatada_para_csv), "%.2s/%.2s/%.4s",
                     data_atual_sistema + 8, // DD
                     data_atual_sistema + 5, // MM
                     data_atual_sistema);    // YYYY

            printf("Consultas agendadas para hoje (%s):\n", data_formatada_para_csv);
            exibir_arquivo(ARQ_CONSULTAS, "data", data_formatada_para_csv);

            limpar_buffer();
            break;
        }
        case 5: // Nova opção: Quantidade de consultas por especialidade
        {
            relatorio_contagem_consultas_por_especialidade(); // Chama a função do relatório
            limpar_buffer();
            break;
        }
        case 6: // Voltar ao menu principal
        {
            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;
        }
        case 7: // Encerrar o programa
        {
            estado_atual = ESTADO_SAIR;
            break;
        }
        default:
            msg_02_opcao_invalida();
            break;
        }
    } while (estado_atual == ESTADO_MODULO_RELATORIOS);

    return estado_atual;
}
