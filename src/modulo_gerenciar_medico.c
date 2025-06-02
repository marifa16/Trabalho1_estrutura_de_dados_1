#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/estruturas.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_medico.h"
#include "../include/files_manager.h"
#include "../include/auxiliar.h"

reg_medico *medicos = NULL; // Vetor global para médicos em memória
int total_medicos = 0;      // Contador de médicos em memória

Estado tratar_modulo_medico()
{
    Estado estado_atual = ESTADO_GERENCIAR_MEDICO;
    int escolha_modulo;

    do
    {
        msg_38_mostrar_modulo_medico();
        escolha_modulo = ler_opcao_menu(1, 6);

        switch (escolha_modulo)
        {
        case 1: // Criar medico
        {
            char nome_medico[100];
            char crm[16];
            char telefone[16];
            Especialidade especialidade_medico_enum;

            msg_22_criar_medico();

            // Nome médico
            do
            {
                msg_23_nome_medico();
                // validar_nome_padrao deve usar fgets e remover \n
                if (!validar_nome_padrao(nome_medico, sizeof(nome_medico)))
                {
                    continue;
                }
                msg_24_validar_nome_medico();
                printf("%s\n", nome_medico);
                msg_40_opcoes();
                int opcao = ler_opcao_menu(1, 3);
                if (opcao == 1)
                    break;
                if (opcao == 2)
                    continue;
                if (opcao == 3 || opcao == -1)
                {
                    msg_05_retornando_menu();
                    goto end_case1;
                } // Usar goto para sair do case
            } while (1);

            // CRM
            do
            {
                msg_30_crm();
                // validar_crm deve usar fgets e remover \n
                if (!validar_crm(crm, sizeof(crm)))
                {
                    // msg_02_opcao_invalida(); // validar_crm já deve emitir msg de erro específica
                    continue;
                }
                // Verificar se CRM já existe
                if (contem_valor(ARQ_MEDICOS, 2, crm))
                { // Coluna 2 é CRM
                    printf("ERRO: CRM (%s) já cadastrado.\n", crm);
                    continue;
                }
                printf("O CRM informado foi: %s\n", crm);
                msg_40_opcoes();
                int opcao = ler_opcao_menu(1, 3);
                if (opcao == 1)
                    break;
                if (opcao == 2)
                    continue;
                if (opcao == 3 || opcao == -1)
                {
                    msg_05_retornando_menu();
                    goto end_case1;
                }
            } while (1);

            // Telefone médico
            do
            {
                msg_14_informar_telefone();
                // validar_telefone_padrao deve usar fgets e remover \n
                if (!validar_telefone_padrao(telefone, sizeof(telefone)))
                {
                    // msg_02_opcao_invalida(); // validar_telefone_padrao já deve emitir msg
                    continue;
                }
                printf("O telefone informado foi: %s\n", telefone);
                msg_40_opcoes();
                int opcao = ler_opcao_menu(1, 3);
                if (opcao == 1)
                    break;
                if (opcao == 2)
                    continue;
                if (opcao == 3 || opcao == -1)
                {
                    msg_05_retornando_menu();
                    goto end_case1;
                }
            } while (1);

            // Especialidade medica
            do
            {
                msg_41_especialidade_medico();
                for (int i = 0; i < TOTAL_ESPECIALIDADES; i++)
                {
                    printf("[%d] - %s\n", i + 1, especialidade_string(i));
                }
                printf("Escolha uma especialidade (1-%d): ", TOTAL_ESPECIALIDADES);
                int escolha_especialidade = ler_opcao_menu(1, TOTAL_ESPECIALIDADES);
                if (escolha_especialidade == -1)
                {
                    msg_05_retornando_menu();
                    goto end_case1;
                }

                especialidade_medico_enum = (Especialidade)(escolha_especialidade - 1);
                printf("Especialidade escolhida: %s\n", especialidade_string(especialidade_medico_enum));
                msg_40_opcoes();
                int opcao = ler_opcao_menu(1, 3);
                if (opcao == 1)
                    break;
                if (opcao == 2)
                    continue;
                if (opcao == 3 || opcao == -1)
                {
                    msg_05_retornando_menu();
                    goto end_case1;
                }
            } while (1);

            // Adicionar ao arquivo CSV
            char *valores_csv[4]; // nome, crm, especialidade_str, telefone
            valores_csv[0] = nome_medico;
            valores_csv[1] = crm;
            valores_csv[2] = (char *)especialidade_string(especialidade_medico_enum);
            valores_csv[3] = telefone;

            if (add_row(ARQ_MEDICOS, 5, valores_csv) > 0) // 5 = id + 4 campos
            {
                msg_29_sucesso_cadastro_medico();
                // Opcional: Recarregar médicos em memória se você os usa globalmente para outras operações
                // carregar_medicos_do_arquivo();
            }
            else
            {
                printf("Erro ao salvar médico no arquivo.\n");
            }
        end_case1:; // Rótulo para goto
            break;
        }

        case 2: // Exibir medico
        {
            char crm_busca[16];
            printf("Digite o CRM do médico para exibir: ");
            fgets(crm_busca, sizeof(crm_busca), stdin);
            crm_busca[strcspn(crm_busca, "\n")] = '\0';

            if (strlen(crm_busca) == 0)
            {
                msg_02_opcao_invalida();
                printf("CRM não pode ser vazio.\n");
                break;
            }

            int id_medico_exibir = get_id(ARQ_MEDICOS, 2, crm_busca); // Coluna 2 é CRM
            if (id_medico_exibir < 0)
            {
                msg_medico_nao_encontrado(crm_busca);
            }
            else
            {
                printf("Detalhes do Médico (ID: %d):\n", id_medico_exibir);
                FILE *f_med = fopen(ARQ_MEDICOS, "r");
                if (f_med)
                {
                    char linha_med[512];
                    char id_csv[16], nome_csv[100], crm_csv[16], esp_csv[100], tel_csv[16];
                    fgets(linha_med, sizeof(linha_med), f_med); // Pula cabeçalho
                    int encontrado_exibir = 0;
                    while (fgets(linha_med, sizeof(linha_med), f_med))
                    {
                        sscanf(linha_med, "%[^,],%[^,],%[^,],%[^,],%[^\n]", id_csv, nome_csv, crm_csv, esp_csv, tel_csv);
                        if (strcmp(crm_csv, crm_busca) == 0)
                        {
                            printf("+-----+----------------------+---------+----------------------+-------------+\n");
                            printf("| ID  | Nome                 | CRM     | Especialidade        | Telefone    |\n");
                            printf("+-----+----------------------+---------+----------------------+-------------+\n");
                            printf("| %-3s | %-20s | %-7s | %-20s | %-11s |\n", id_csv, nome_csv, crm_csv, esp_csv, tel_csv);
                            printf("+-----+----------------------+---------+----------------------+-------------+\n");
                            encontrado_exibir = 1;
                            break;
                        }
                    }
                    fclose(f_med);
                    if (!encontrado_exibir)
                        msg_medico_nao_encontrado(crm_busca); // Segurança
                }
                else
                {
                    msg_erro_abrir_arquivo_nome(ARQ_MEDICOS);
                }
            }
            break;
        }

        case 3: // Atualizar medico
        {
            char crm_busca[16];
            printf("Digite o CRM do médico para atualizar: ");
            fgets(crm_busca, sizeof(crm_busca), stdin);
            crm_busca[strcspn(crm_busca, "\n")] = '\0';

            if (strlen(crm_busca) == 0)
            {
                msg_02_opcao_invalida();
                printf("CRM não pode ser vazio.\n");
                break;
            }

            int linha_medico_atualizar = buscar_linha(ARQ_MEDICOS, 2, crm_busca); // Coluna 2 é CRM
            if (linha_medico_atualizar < 0)
            {
                msg_medico_nao_encontrado(crm_busca);
                break;
            }
            // Ler dados atuais do médico do arquivo
            FILE *arquivo_leitura = fopen(ARQ_MEDICOS, "r");
            if (!arquivo_leitura)
            {
                msg_erro_abrir_arquivo_nome(ARQ_MEDICOS);
                break;
            }

            char linha_csv[512];
            char id_atual_str[16], nome_atual[100], crm_original[16], esp_atual_str[100], tel_atual[16];

            fgets(linha_csv, sizeof(linha_csv), arquivo_leitura); // Pula cabeçalho
            for (int i = 0; i <= linha_medico_atualizar; i++)
            {
                if (fgets(linha_csv, sizeof(linha_csv), arquivo_leitura) == NULL)
                {
                    // Erro, não deveria acontecer se buscar_linha retornou uma linha válida
                    fclose(arquivo_leitura);
                    printf("Erro ao ler dados do médico para atualização.\n");
                    goto end_case3;
                }
            }
            fclose(arquivo_leitura);

            // Tokenizar a linha lida
            char linha_copia[512];
            strncpy(linha_copia, linha_csv, sizeof(linha_copia) - 1);
            linha_copia[sizeof(linha_copia) - 1] = '\0';

            char *token;
            token = strtok(linha_copia, ",\n");
            if (token)
                strncpy(id_atual_str, token, sizeof(id_atual_str) - 1);
            else
                strcpy(id_atual_str, "");
            token = strtok(NULL, ",\n");
            if (token)
                strncpy(nome_atual, token, sizeof(nome_atual) - 1);
            else
                strcpy(nome_atual, "");
            token = strtok(NULL, ",\n");
            if (token)
                strncpy(crm_original, token, sizeof(crm_original) - 1);
            else
                strcpy(crm_original, "");
            token = strtok(NULL, ",\n");
            if (token)
                strncpy(esp_atual_str, token, sizeof(esp_atual_str) - 1);
            else
                strcpy(esp_atual_str, "");
            token = strtok(NULL, ",\n");
            if (token)
                strncpy(tel_atual, token, sizeof(tel_atual) - 1);
            else
                strcpy(tel_atual, "");
            id_atual_str[sizeof(id_atual_str) - 1] = '\0';
            nome_atual[sizeof(nome_atual) - 1] = '\0';
            crm_original[sizeof(crm_original) - 1] = '\0';
            esp_atual_str[sizeof(esp_atual_str) - 1] = '\0';
            tel_atual[sizeof(tel_atual) - 1] = '\0';

            // Buffers para novos dados, inicializados com os atuais
            char novo_nome[100];
            strncpy(novo_nome, nome_atual, sizeof(novo_nome));
            char novo_crm[16];
            strncpy(novo_crm, crm_original, sizeof(novo_crm));
            char novo_telefone[16];
            strncpy(novo_telefone, tel_atual, sizeof(novo_telefone));
            char nova_especialidade_str[100];
            strncpy(nova_especialidade_str, esp_atual_str, sizeof(nova_especialidade_str));

            int continuar_atualizacao_menu = 1;
            while (continuar_atualizacao_menu)
            {
                msg_25_atualizar_medico(); // Menu: 1-Nome, 2-CRM, 3-Telefone, 4-Especialidade
                printf("[5] - Salvar e Sair\n");
                printf("[6] - Cancelar e Sair\n");
                printf("===========================\n");
                int escolha_campo = ler_opcao_menu(1, 6);

                switch (escolha_campo)
                {
                case 1: // Nome
                    printf("Nome atual: %s\n", novo_nome);
                    do
                    {
                        msg_17_atualizar_nome(); // "Insira o novo nome:"
                        if (!validar_nome_padrao(novo_nome, sizeof(novo_nome)))
                            continue;
                        printf("Novo nome proposto: %s\n", novo_nome);
                        msg_40_opcoes();
                        int op_confirma = ler_opcao_menu(1, 3);
                        if (op_confirma == 1)
                            break;
                        if (op_confirma == 2)
                        {
                            strncpy(novo_nome, nome_atual, sizeof(novo_nome));
                            continue;
                        }
                        if (op_confirma == 3 || op_confirma == -1)
                        {
                            strncpy(novo_nome, nome_atual, sizeof(novo_nome));
                            msg_05_retornando_menu();
                            break;
                        }
                    } while (1);
                    break;
                case 2: // CRM
                    printf("CRM atual: %s\n", novo_crm);
                    do
                    {
                        msg_26_novo_crm();
                        if (!validar_crm(novo_crm, sizeof(novo_crm)))
                            continue;
                        if (strcmp(novo_crm, crm_original) != 0 && contem_valor(ARQ_MEDICOS, 2, novo_crm))
                        {
                            printf("ERRO: Novo CRM (%s) já cadastrado.\n", novo_crm);
                            strncpy(novo_crm, crm_original, sizeof(novo_crm));
                            continue;
                        }
                        printf("Novo CRM proposto: %s\n", novo_crm);
                        msg_40_opcoes();
                        int op_confirma = ler_opcao_menu(1, 3);
                        if (op_confirma == 1)
                            break;
                        if (op_confirma == 2)
                        {
                            strncpy(novo_crm, crm_original, sizeof(novo_crm));
                            continue;
                        }
                        if (op_confirma == 3 || op_confirma == -1)
                        {
                            strncpy(novo_crm, crm_original, sizeof(novo_crm));
                            msg_05_retornando_menu();
                            break;
                        }
                    } while (1);
                    break;
                case 3: // Telefone
                    printf("Telefone atual: %s\n", novo_telefone);
                    do
                    {
                        msg_14_informar_telefone(); // Reutilizando msg de paciente
                        if (!validar_telefone_padrao(novo_telefone, sizeof(novo_telefone)))
                            continue;
                        printf("Novo telefone proposto: %s\n", novo_telefone);
                        msg_40_opcoes();
                        int op_confirma = ler_opcao_menu(1, 3);
                        if (op_confirma == 1)
                            break;
                        if (op_confirma == 2)
                        {
                            strncpy(novo_telefone, tel_atual, sizeof(novo_telefone));
                            continue;
                        }
                        if (op_confirma == 3 || op_confirma == -1)
                        {
                            strncpy(novo_telefone, tel_atual, sizeof(novo_telefone));
                            msg_05_retornando_menu();
                            break;
                        }
                    } while (1);
                    break;
                case 4: // Especialidade
                    printf("Especialidade atual: %s\n", nova_especialidade_str);
                    do
                    {
                        msg_41_especialidade_medico();
                        for (int k_esp = 0; k_esp < TOTAL_ESPECIALIDADES; k_esp++)
                        {
                            printf("[%d] - %s\n", k_esp + 1, especialidade_string(k_esp));
                        }
                        printf("Escolha uma especialidade (1-%d): ", TOTAL_ESPECIALIDADES);
                        int escolha_nova_esp_idx = ler_opcao_menu(1, TOTAL_ESPECIALIDADES);
                        if (escolha_nova_esp_idx == -1)
                        {
                            msg_05_retornando_menu();
                            break;
                        }

                        const char *temp_nova_esp_str = especialidade_string((Especialidade)(escolha_nova_esp_idx - 1));
                        printf("Nova especialidade proposta: %s\n", temp_nova_esp_str);
                        msg_40_opcoes();
                        int op_confirma = ler_opcao_menu(1, 3);
                        if (op_confirma == 1)
                        {
                            strncpy(nova_especialidade_str, temp_nova_esp_str, sizeof(nova_especialidade_str) - 1);
                            nova_especialidade_str[sizeof(nova_especialidade_str) - 1] = '\0';
                            break;
                        }
                        if (op_confirma == 2)
                            continue;
                        if (op_confirma == 3 || op_confirma == -1)
                        {
                            msg_05_retornando_menu();
                            break;
                        }
                    } while (1);
                    break;
                case 5: // Salvar e Sair
                {
                    char *valores_att[5]; // id, nome, crm, especialidade, telefone
                    valores_att[0] = id_atual_str;
                    valores_att[1] = novo_nome;
                    valores_att[2] = novo_crm;
                    valores_att[3] = nova_especialidade_str;
                    valores_att[4] = novo_telefone;
                    if (att_row(ARQ_MEDICOS, linha_medico_atualizar, 5, valores_att))
                    {
                        printf("Médico atualizado com sucesso!\n");
                    }
                    else
                    {
                        printf("Erro ao atualizar médico.\n");
                    }
                    continuar_atualizacao_menu = 0;
                }
                break;
                case 6: // Cancelar e Sair
                    printf("Atualização cancelada. Nenhuma alteração foi salva.\n");
                    continuar_atualizacao_menu = 0;
                    break;
                default:
                    msg_02_opcao_invalida();
                    break;
                }
            }
        end_case3:;
            break;
        }

        case 4: // Deletar medico
        {
            char crm_busca[16];
            printf("Digite o CRM do médico para deletar: ");
            fgets(crm_busca, sizeof(crm_busca), stdin);
            crm_busca[strcspn(crm_busca, "\n")] = '\0';

            if (strlen(crm_busca) == 0)
            {
                msg_02_opcao_invalida();
                printf("CRM não pode ser vazio.\n");
                break;
            }

            int linha_del = buscar_linha(ARQ_MEDICOS, 2, crm_busca); // Coluna 2 é CRM
            if (linha_del < 0)
            {
                msg_medico_nao_encontrado(crm_busca);
            }
            else
            {
                // Adicionar confirmação antes de deletar
                printf("Tem certeza que deseja deletar o médico com CRM %s?\n", crm_busca);
                msg_40_opcoes();
                int op_confirma_del = ler_opcao_menu(1, 3);
                if (op_confirma_del == 1)
                {
                    if (del_row(ARQ_MEDICOS, linha_del))
                        printf("\nMédico deletado com sucesso!\n");
                    else
                        printf("\nErro ao deletar médico!\n");
                }
                else if (op_confirma_del == 2)
                {
                    printf("Deleção cancelada.\n");
                }
                else
                { // 3 ou -1
                    msg_05_retornando_menu();
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
            break;
        }
    } while (estado_atual == ESTADO_GERENCIAR_MEDICO);

    return estado_atual;
}
