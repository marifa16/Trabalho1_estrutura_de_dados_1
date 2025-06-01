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
            limpar_buffer();
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = '\0';

            // Valida se o paciente existe
            int id_paciente = get_id(ARQ_PACIENTES, 2, cpf);
            if (id_paciente < 0)
            {
                msg_02_opcao_invalida();
                msg_paciente_nao_encontrado(cpf); // Função já existe em mensagens.c
                break;
            }

            // Exibe consultas relacionadas ao paciente
            char id_paciente_str[16];
            snprintf(id_paciente_str, sizeof(id_paciente_str), "%d", id_paciente);
            printf("Consultas do paciente (CPF: %s):\n", cpf);
            msg_cabecalho_tabela_consultas();
            exibir_arquivo(ARQ_CONSULTAS, "paciente", id_paciente_str);
            msg_rodape_tabela_consultas();
            break;
        }

        case 2: // Consultas de médicos
        {
            char crm_str[16];
            printf("Digite o CRM do médico: ");
            limpar_buffer();
            fgets(crm_str, sizeof(crm_str), stdin);
            crm_str[strcspn(crm_str, "\n")] = '\0';

            // Valida se o médico existe
            int id_medico = get_id(ARQ_MEDICOS, 2, crm_str);
            if (id_medico < 0)
            {
                msg_medico_nao_encontrado(crm_str); // Função já existe em mensagens.c
                break;
            }

            // Exibe consultas relacionadas ao médico
            char id_medico_str[16];
            snprintf(id_medico_str, sizeof(id_medico_str), "%d", id_medico);
            printf("Consultas do médico (CRM: %s):\n", crm_str);
            msg_cabecalho_tabela_consultas();
            exibir_arquivo(ARQ_CONSULTAS, "medico", id_medico_str);
            msg_rodape_tabela_consultas();
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
            msg_cabecalho_tabela_consultas();
            exibir_arquivo(ARQ_CONSULTAS, "especialidade", especialidade_selecionada);
            msg_rodape_tabela_consultas();
            break;
        }

        case 4: // Relatório de pacientes sem consultas
        {
            printf("Pacientes sem consultas agendadas:\n");
            exibir_arquivo(ARQ_PACIENTES, "sem_consultas", "");
            break;
        }

        case 5: // Relatório de médicos sem consultas
        {
            printf("Médicos sem consultas agendadas:\n");
            exibir_arquivo(ARQ_MEDICOS, "sem_consultas", "");
            break;
        }

        case 6: // Consultas no período
        {
            char data_inicio[11], data_fim[11];
            printf("Digite a data de início (AAAA-MM-DD): ");
            fgets(data_inicio, sizeof(data_inicio), stdin);
            data_inicio[strcspn(data_inicio, "\n")] = '\0';

            printf("Digite a data de fim (AAAA-MM-DD): ");
            fgets(data_fim, sizeof(data_fim), stdin);
            data_fim[strcspn(data_fim, "\n")] = '\0';

            // Valida as datas informadas
            if (!validar_data(data_inicio) || !validar_data(data_fim))
            {
                msg_02_opcao_invalida();
                break;
            }

            printf("Consultas agendadas entre %s e %s:\n", data_inicio, data_fim);
            char periodo[32];
            snprintf(periodo, sizeof(periodo), "%s;%s", data_inicio, data_fim);
            exibir_arquivo(ARQ_CONSULTAS, "periodo", periodo);
            msg_rodape_tabela_consultas();
            break;
        }

        case 7: // Voltar
            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;

        default:
            msg_02_opcao_invalida();
            break;
        }

    } while (estado_atual == ESTADO_MODULO_RELATORIOS);

    return estado_atual;
}
