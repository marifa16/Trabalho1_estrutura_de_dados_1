#include <stdio.h>
#include "../include/estruturas.h"
#include "../include/mensagens.h"
#include "../include/modulo_relatorios.h"
#include <string.h>
#include "../include/auxiliar.h"
#include "../include/files_manager.h"

Estado tratar_modulo_relatorios()
{
    Estado estado_atual = ESTADO_MODULO_RELATORIOS;
    int escolha_modulo;

    do
    {
        msg_39_mostrar_modulo_relatorios(); // Exibe o menu de relatórios
        scanf("%d", &escolha_modulo);

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
            int id_paciente = get_id("data/registro_pacientes.csv", 2, cpf);
            if (id_paciente < 0)
            {
                msg_02_opcao_invalida();
                printf("Paciente com CPF %s não encontrado!\n", cpf);
                break;
            }

            // Exibe consultas relacionadas ao paciente
            char id_paciente_str[16];
            snprintf(id_paciente_str, sizeof(id_paciente_str), "%d", id_paciente);
            printf("Consultas do paciente (CPF: %s):\n", cpf);
            exibir_arquivo("data/registro_consultas.csv", "paciente", id_paciente_str);
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
            int id_medico = get_id("data/registro_medicos.csv", 2, crm_str);
            if (id_medico < 0)
            {
                printf("Médico com CRM %s não encontrado!\n", crm_str);
                break;
            }

            // Exibe consultas relacionadas ao médico
            char id_medico_str[16];
            snprintf(id_medico_str, sizeof(id_medico_str), "%d", id_medico);
            printf("Consultas do médico (CRM: %s):\n", crm_str);
            exibir_arquivo("data/registro_consultas.csv", "medico", id_medico_str);
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
            scanf("%d", &opcao);

            if (opcao == 0)
            {
                break; // Volta ao menu de relatórios
            }
            if (opcao < 1 || opcao > 5)
            {
                msg_02_opcao_invalida();
                break;
            }
            char *especialidade_escolhida = especialidades[opcao - 1];

            // Exibe consultas apenas para médicos com a especialidade escolhida
            FILE *arq = fopen("data/registro_consultas.csv", "r");
            if (!arq)
            {
                printf("Erro ao abrir o arquivo de consultas.\n");
                break;
            }
            char linha[512];
            fgets(linha, sizeof(linha), arq); // pula cabeçalho

            // Cabeçalho da tabela
            printf("+-----+---------------------+---------------------+---------------------+------------+\n");
            printf("| ID  | Paciente            | Medico              | Data/Hora           | Status     |\n");
            printf("+-----+---------------------+---------------------+---------------------+------------+\n");

            while (fgets(linha, sizeof(linha), arq))
            {
                int id, id_pac, id_med;
                char data_hora[32], status[16];
                if (sscanf(linha, "%d,%d,%d,%31[^,],%15[^\n]", &id, &id_pac, &id_med, data_hora, status) == 5)
                {
                    if (get_medico_especial(id_med, especialidade_escolhida))
                    {
                        char nome_paciente[122] = "";
                        char nome_medico[102] = "";
                        buscar_nome_paciente_por_id(id_pac, nome_paciente, sizeof(nome_paciente));
                        buscar_nome_medico_por_id(id_med, nome_medico, sizeof(nome_medico));
                        printf("| %-4d | %-19s | %-19s | %-19s | %-10s |\n",
                               id, nome_paciente, nome_medico, data_hora, status);
                    }
                }
            }
            printf("+-----+---------------------+---------------------+---------------------+------------+\n");
            fclose(arq);
            break;
        }

        case 4: // Consultas do dia
        {
            printf("Consultas do dia atual:\n");
            exibir_arquivo("data/registro_consultas.csv", "dia", NULL);
            break;
        }
        case 5: // contagem por especialidade
        {
            relatorio_contagem_consultas_por_especialidade();
            break;
        }
        case 6: // Voltar ao menu principal
            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;

        case 7: // Sair do programa
            estado_atual = ESTADO_SAIR;
            break;

        default:
            msg_02_opcao_invalida();
            break;
        }
    } while (estado_atual == ESTADO_MODULO_RELATORIOS);

    return estado_atual;
}
