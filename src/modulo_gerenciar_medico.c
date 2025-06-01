#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/estruturas.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_medico.h"
#include "../include/files_manager.h"
#include "../include/auxiliar.h"

const char *especialidade_string(Especialidade especialidade);

reg_medico *medicos = NULL;
int total_medicos = 0;

Estado tratar_modulo_medico()
{
    Estado estado_atual = ESTADO_GERENCIAR_MEDICO;
    int escolha_modulo;
    char nome_medico[100];              // Variável para armazenar o nome do médico
    char crm[16];                       // Variável para armazenar o CRM do médico
    char telefone[16];                  // Variável para armazenar o telefone do médico
    Especialidade especialidade_medico; // Variável para controlar o loop de saída

    do
    {
        // Menu principal do módulo médico
        msg_38_mostrar_modulo_medico();
        escolha_modulo = ler_opcao_menu(1, 6); // 1 a 6 conforme seu menu

        switch (escolha_modulo)
        {
        case 1: // Criar medico
        {
            msg_22_criar_medico(); // Mensagem de boas-vindas ao cadastro

            // Nome médico
            do
            {
                msg_23_nome_medico();
                if (!validar_nome_padrao(nome_medico, sizeof(nome_medico)))
                    continue;
                msg_24_validar_nome_medico();
                printf("%s\n", nome_medico);

                msg_40_opcoes();
                // Fun��o Validar
                int opcao = ler_opcao_menu(1, 3); // 1=SIM, 2=NÃO, 3=SAIR
                if (opcao == 1)                   // SIM
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
                if (!validar_crm(crm, sizeof(crm)))
                    continue;

                printf("O CRM informado foi: %s\n", crm); // Exibe o crm domedico

                msg_40_opcoes();
                // Fun��o Validar
                int opcao = ler_opcao_menu(1, 3); // 1=SIM, 2=N�O, 3=SAIR
                if (opcao == 1)                   // SIM
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
                    return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu gerenciarmedico
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
                if (!validar_telefone_padrao(telefone, sizeof(telefone)))
                    continue;
                printf("O telefone informado foi: %s\n", telefone);

                msg_40_opcoes();
                // Funcao Validar
                int opcao = ler_opcao_menu(1, 3);

                if (opcao == 1) // SIM
                {
                    break;
                }
                else if (opcao == 2) // NAO
                {
                    continue; // Volta para solicitar o telefone de novo
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu gerenciarmedico
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);

            // Especialidade m�dica
            do
            {
                msg_41_especialidade_medico();

                // Exibe as opcoes de especialidades
                for (int i = 0; i < TOTAL_ESPECIALIDADES; i++)
                {
                    printf("[%d] - %s\n", i + 1, especialidade_string(i));
                }

                printf("Escolha uma especialidade (1-%d): ", TOTAL_ESPECIALIDADES);
                int escolha_especialidade = ler_opcao_menu(1, TOTAL_ESPECIALIDADES);
                especialidade_medico = escolha_especialidade - 1; // Atribui o valor do enum corretamente

                msg_40_opcoes();
                int opcao = ler_opcao_menu(1, 3);

                if (opcao == 1) // SIM
                {
                    break; // Sai do do-while e segue para o cadastro
                }
                else if (opcao == 2) // NAO
                {
                    continue; // Volta a perguntar a especialidade
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    return ESTADO_GERENCIAR_MEDICO;
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);

            // Agora, fora do do-while, faca o cadastro:
            medicos = realloc(medicos, (total_medicos + 1) * sizeof(reg_medico));
            if (medicos == NULL)
            {
                printf("Erro ao alocar memoria para o medico.\n");
                return (1);
            }
            strcpy(medicos[total_medicos].nome, nome_medico);
            strcpy(medicos[total_medicos].crm, crm);
            strcpy(medicos[total_medicos].telefone, telefone);
            medicos[total_medicos].especialidade_medico = especialidade_medico;
            total_medicos++;

            char *valores[4];
            valores[0] = nome_medico;
            valores[1] = crm;
            valores[2] = (char *)especialidade_string(especialidade_medico); // apenas aqui, se necessário
            valores[3] = telefone;

            add_row(ARQ_MEDICOS, 5, valores); // 5 colunas (id + 4 campos)
            msg_29_sucesso_cadastro_medico();
            break;
        }

        case 2: // Exibir medico
        {
            char crm_busca[16];
            printf("Digite o CRM do médico para exibir: ");
            fgets(crm_busca, sizeof(crm_busca), stdin);
            crm_busca[strcspn(crm_busca, "\n")] = '\0';

            int id = get_id(ARQ_MEDICOS, 2, crm_busca);
            if (id < 0)
            {
                msg_medico_nao_encontrado(crm_busca);
            }
            else
            {
                read_row(ARQ_MEDICOS, id); // Lê a linha do médico
            }
            break;
        }

        case 3: // Atualizar medico
        {
            char crm_busca[16];
            printf("Digite o CRM domedico para atualizar: ");
            fgets(crm_busca, sizeof(crm_busca), stdin);
            crm_busca[strcspn(crm_busca, "\n")] = '\0';

            int linha = buscar_linha(ARQ_MEDICOS, 2, crm_busca);
            if (linha < 0)
            {
                printf("M�dico com CRM %s n�o encontrado!\n", crm_busca);
                return ESTADO_GERENCIAR_MEDICO;
            }

            FILE *arquivo = fopen(ARQ_MEDICOS, "r");
            if (!arquivo)
            {
                msg_erro_abrir_arquivo();
                break;
            }
            char linha_csv[512];
            fgets(linha_csv, sizeof(linha_csv), arquivo);
            for (int i = 0; i <= linha; i++)
                fgets(linha_csv, sizeof(linha_csv), arquivo);
            fclose(arquivo);

            // Extrai os campos da linha
            char id[8], nome[100], crm[16], especialidade[32], telefone[16];
            char *token = strtok(linha_csv, ",\n");
            int i = 0;
            while (token && i < 5)
            {
                switch (i)
                {
                case 0:
                    strncpy(id, token, sizeof(id));
                    id[sizeof(id) - 1] = '\0';
                    break;
                case 1:
                    strncpy(nome, token, sizeof(nome));
                    nome[sizeof(nome) - 1] = '\0';
                    break;
                case 2:
                    strncpy(crm, token, sizeof(crm));
                    crm[sizeof(crm) - 1] = '\0';
                    break;
                case 3:
                    strncpy(especialidade, token, sizeof(especialidade));
                    especialidade[sizeof(especialidade) - 1] = '\0';
                    break;
                case 4:
                    strncpy(telefone, token, sizeof(telefone));
                    telefone[sizeof(telefone) - 1] = '\0';
                    break;
                }
                token = strtok(NULL, ",\n");
                i++;
            }

            int continuar = 1;
            while (continuar)
            {
                msg_25_atualizar_medico();
                int opcao = ler_opcao_menu(1, 4);

                switch (opcao)
                {
                case 1: // Nome
                    msg_17_atualizar_nome();
                    fgets(nome, sizeof(nome), stdin);
                    nome[strcspn(nome, "\n")] = '\0';
                    break;
                case 2: // CRM
                    msg_26_novo_crm();
                    fgets(crm, sizeof(crm), stdin);
                    crm[strcspn(crm, "\n")] = '\0';
                    break;
                case 3: // Telefone
                    msg_20_novo_telefone();
                    fgets(telefone, sizeof(telefone), stdin);
                    telefone[strcspn(telefone, "\n")] = '\0';
                    break;
                case 4: // Especialidade
                    msg_27_nova_especialidade();
                    for (int i = 0; i < TOTAL_ESPECIALIDADES; i++)
                        printf("[%d] - %s\n", i + 1, especialidade_string(i));
                    printf("Escolha uma especialidade (1-%d): ", TOTAL_ESPECIALIDADES);
                    char escolha_especialidade_str[10];
                    fgets(escolha_especialidade_str, sizeof(escolha_especialidade_str), stdin);
                    int escolha_especialidade;
                    if (sscanf(escolha_especialidade_str, "%d", &escolha_especialidade) == 1 &&
                        escolha_especialidade >= 1 && escolha_especialidade <= TOTAL_ESPECIALIDADES)
                    {
                        strncpy(especialidade, especialidade_string(escolha_especialidade - 1), sizeof(especialidade));
                        especialidade[sizeof(especialidade) - 1] = '\0';
                    }
                    else
                    {
                        msg_02_opcao_invalida();
                    }
                    break;
                }

                printf("Deseja atualizar mais alguma informacao?\n[1] - SIM\n[2] - NAO\n[3] - SAIR\n");
                char resp[10];
                fgets(resp, sizeof(resp), stdin);
                int resp_int = atoi(resp);
                if (resp_int == 2)
                    continuar = 0;
                else if (resp_int == 3)
                {
                    msg_05_retornando_menu();
                    return ESTADO_GERENCIAR_MEDICO;
                }
            }

            // Atualiza a linha domedico
            char *valores[5];
            valores[0] = id;
            valores[1] = nome;
            valores[2] = crm;
            valores[3] = especialidade;
            valores[4] = telefone;
            att_row(ARQ_MEDICOS, linha, 5, valores);
            printf("Medico atualizado com sucesso!\n");
            break;
        }

        case 4: // deletar
        {
            char crm_busca[16];
            printf("Digite o CRM do médico para deletar: ");
            fgets(crm_busca, sizeof(crm_busca), stdin);
            crm_busca[strcspn(crm_busca, "\n")] = '\0';

            int linha = buscar_linha(ARQ_MEDICOS, 2, crm_busca);
            if (linha < 0)
            {
                printf("Medico com CRM: %s, não encontrado!\n", crm_busca);
                break;
            }

            if (del_row(ARQ_MEDICOS, linha))
                msg_paciente_deletado(); // Ou crie msg_medico_deletado()
            else
                msg_erro_deletar_paciente(); // Ou crie msg_erro_deletar_medico()
            break;
        }

        case 5: // Voltar ao menu principal
        {
            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;
        }

        case 6: // Sair do programa
        {
            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;
        }

        default:
            msg_02_opcao_invalida();
            break;
        }
    } while (estado_atual == ESTADO_GERENCIAR_MEDICO);
    return estado_atual;
}
