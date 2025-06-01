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
    char nome_medico[100];              // Vari√°vel para armazenar o nome do m√©dico
    char crm[16];                       // Vari√°vel para armazenar o CRM do m√©dico
    char telefone[16];                  // Vari√°vel para armazenar o telefone do m√©dico
    Especialidade especialidade_medico; // Vari√°vel para controlar o loop de sa√≠da

    do
    {
        msg_38_mostrar_modulo_medico(); // Exibe o menu de gerenciamento de m√©dicos

        // Limpa o buffer de entrada se o pr√≥ximo caractere for '\n'
        int c = getchar();
        if (c != '\n' && c != EOF)
        {
            ungetc(c, stdin); // Devolve o caractere se n√£o era '\n'
        }

        char escolha_modulo_str[10];
        fgets(escolha_modulo_str, sizeof(escolha_modulo_str), stdin); // L√™ a entrada como string
        escolha_modulo_str[strcspn(escolha_modulo_str, "\n")] = '\0'; // Remove o caractere de nova linha

        // Converte a entrada para inteiro
        if (sscanf(escolha_modulo_str, "%d", &escolha_modulo) != 1)
        {
            msg_02_opcao_invalida();
            continue;
        }

        switch (escolha_modulo)
        {
        case 1: // Criar MÈdico
        {
            msg_22_criar_medico(); // Mensagem de boas-vindas ao cadastro

            // Nome m√©dico
            do
            {
                msg_23_nome_medico();
                fgets(nome_medico, sizeof(nome_medico), stdin); // L√™ o nome do m√©ddico como string
                nome_medico[strcspn(nome_medico, "\n")] = '\0'; // Remove o caractere de nova linha

                // Verifica se o nome n√£o est√° vazio
                if (strlen(nome_medico) == 0)
                {
                    msg_02_opcao_invalida();
                    printf("O nome do m√©dico n√£o pode estar vazio.\n");
                    continue; // Volta a solicitar o nome
                }

                // Verifica se o nome cont√©m n√∫meros
                int contem_numero = 0;
                for (size_t i = 0; i < strlen(nome_medico); i++)
                {
                    if (isdigit(nome_medico[i])) // Verifica se o caractere √© um n√∫mero
                    {
                        contem_numero = 1; // Marca que o nome cont√©m n√∫meros
                        break;             // Sai do loop
                    }
                }
                if (contem_numero)
                {
                    msg_02_opcao_invalida();
                    printf("O nome do m√©dico n√£o pode conter n√∫meros.\n");
                    continue; // Volta a solicitar o nome
                }

                msg_24_validar_nome_medico();
                printf("%s\n", nome_medico); // Exibe o nome do m√©dico

                msg_40_opcoes();
                // FunÁ„o Validar
                int opcao = ler_opcao_menu(1, 3); // 1=SIM, 2=N√ÉO, 3=SAIR
                if (opcao == 1)                   // SIM
                {
                    break; // Sai do loop
                }
                else if (opcao == 2) // N√ÉO
                {
                    continue; // Volta para solicitar o nome novamente
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu de gerenciamento de m√©dicos
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
                fgets(crm, sizeof(crm), stdin); // L√™ o CRM como string
                crm[strcspn(crm, "\n")] = '\0'; // Remove o caractere de nova linha

                // Verifica se o CRM possui 6 digitos
                if (strlen(crm) != 6)
                {
                    msg_02_opcao_invalida();
                    printf("O CRM deve conter 6 d√≠gitos.\n");
                    continue; // Volta para solicitar o CRM de novo
                }

                // Verifica se todos os caracteres s√£o n√∫meros
                int valido = 1;
                for (size_t i = 0; i < strlen(crm); i++)
                {
                    if (crm[i] < '0' || crm[i] > '9')
                    {
                        valido = 0;
                        break;
                    }
                }
                if (!valido)
                {
                    msg_02_opcao_invalida();
                    printf("O CRM deve conter apenas n√∫meros.\n");
                    continue; // Volta para solicitar o CRM de novo
                }

                printf("O CRM informado foi: %s\n", crm); // Exibe o crm do mÈdico

                msg_40_opcoes();
                // FunÁ„o Validar
                int opcao = ler_opcao_menu(1, 3); // 1=SIM, 2=N√O, 3=SAIR
                if (opcao == 1)                   // SIM
                {
                    break; // Sai do loop
                }
                else if (opcao == 2) // N√ÉO
                {
                    continue; // Volta para solicitar o CRM
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu gerenciar mÈdico
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);

            // Telefone m√©dico
            do
            {
                msg_14_informar_telefone();
                char telefone_str[15];                            // Vari·vel que armazena o telefone como string
                fgets(telefone_str, sizeof(telefone_str), stdin); // L√™ o telefone como string
                telefone_str[strcspn(telefone_str, "\n")] = '\0'; // Remove o caractere de linha

                // Verifica se o telefone tem 11 digitos
                if (strlen(telefone_str) != 11)
                {
                    msg_02_opcao_invalida();
                    printf("O telefone deve conter 11 digitos (DDD + N√öMERO).\n");
                    continue; // Volta para solicitar o telefone
                }

                // Verifica se os caracteres s„o n˙meros
                int valido = 1;
                for (size_t i = 0; i < strlen(telefone_str); i++)
                {
                    if (telefone_str[i] < '0' || telefone_str[i] > '9') // Verifica se o caractere √© um d√≠gito
                    {
                        valido = 0;
                        break;
                    }
                }
                if (!valido)
                {
                    msg_02_opcao_invalida();
                    printf("O telefone deve conter apenas n√∫meros.\n");
                    continue; // Volta para solicitar o telefone
                }

                printf("O telefone informado foi: %s\n", telefone_str); // Exibe o telefone do mÈdico

                msg_40_opcoes();
                // FunÁ„oo Validar
                int opcao = ler_opcao_menu(1, 3);

                if (opcao == 1) // SIM
                {
                    strcpy(telefone, telefone_str); // <-- COPIA O TELEFONE VALIDADO
                    break;
                }
                else if (opcao == 2) // N√O
                {
                    continue; // Volta para solicitar o telefone de novo
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu gerenciar mÈdico
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);

            // Especialidade mÈdica
            do
            {
                msg_41_especialidade_medico();

                // Exibe as opÁıes de especialidades
                for (int i = 0; i < TOTAL_ESPECIALIDADES; i++)
                {
                    printf("[%d] - %s\n", i + 1, especialidade_string(i));
                }

                printf("Escolha uma especialidade (1-%d): ", TOTAL_ESPECIALIDADES);
                char escolha_especialidade_str[10];
                fgets(escolha_especialidade_str, sizeof(escolha_especialidade_str), stdin);
                escolha_especialidade_str[strcspn(escolha_especialidade_str, "\n")] = '\0';

                int escolha_especialidade;
                if (sscanf(escolha_especialidade_str, "%d", &escolha_especialidade) != 1)
                {
                    msg_02_opcao_invalida();
                    continue;
                }
                if (escolha_especialidade >= 1 && escolha_especialidade <= TOTAL_ESPECIALIDADES)
                {
                    especialidade_medico = escolha_especialidade - 1;
                    printf("A especialidade escolhida foi: %s\n", especialidade_string(especialidade_medico));

                    msg_40_opcoes();
                    int opcao = ler_opcao_menu(1, 3);

                    if (opcao == 1) // SIM
                    {
                        break; // Sai do do-while e segue para o cadastro
                    }
                    else if (opcao == 2) // N√O
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
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);

            // Agora, fora do do-while, faÁa o cadastro:
            medicos = realloc(medicos, (total_medicos + 1) * sizeof(reg_medico));
            if (medicos == NULL)
            {
                printf("Erro ao alocar memÛria para o mÈdico.\n");
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
            valores[2] = (char *)especialidade_string(especialidade_medico);
            valores[3] = telefone;

            add_row("data/registro_medicos.csv", 5, valores); // 5 colunas (id + 4 campos)
            msg_29_sucesso_cadastro_medico();
            break;
        }

        case 2: // Exibir MÈdico
        {
            char crm_busca[16];
            printf("Digite o CRM do m√©dico para exibir: ");
            fgets(crm_busca, sizeof(crm_busca), stdin);
            crm_busca[strcspn(crm_busca, "\n")] = '\0';

            int id = get_id("data/registro_medicos.csv", 2, crm_busca);
            if (id < 0)
            {
                printf("M√©dico com CRM %s n√£o encontrado!\n", crm_busca);
            }
            else
            {
                read_row("data/registro_medicos.csv", id); // L√™ a linha do m√©dico
            }
            break;
        }

        case 3: // Atualizar MÈdico
        {
            char crm_busca[16];
            printf("Digite o CRM do mÈdico para atualizar: ");
            fgets(crm_busca, sizeof(crm_busca), stdin);
            crm_busca[strcspn(crm_busca, "\n")] = '\0';

            int linha = buscar_linha("data/registro_medicos.csv", 2, crm_busca);
            if (linha < 0)
            {
                printf("MÈdico com CRM %s n„o encontrado!\n", crm_busca);
                return ESTADO_GERENCIAR_MEDICO;
            }

            FILE *arquivo = fopen("data/registro_medicos.csv", "r");
            if (!arquivo)
            {
                printf("Erro ao abrir o arquivo!\n");
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

                printf("Deseja atualizar mais alguma informaÁ„o?\n[1] - SIM\n[2] - N√O\n[3] - SAIR\n");
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

            // Atualiza a linha do mÈdico
            char *valores[5];
            valores[0] = id;
            valores[1] = nome;
            valores[2] = crm;
            valores[3] = especialidade;
            valores[4] = telefone;
            att_row("data/registro_medicos.csv", linha, 5, valores);
            printf("MÈdico atualizado com sucesso!\n");
            break;
        }

        case 4: // deletar
        {
            char crm_busca[16];
            printf("Digite o CRM do m√©dico para deletar: ");
            fgets(crm_busca, sizeof(crm_busca), stdin);
            crm_busca[strcspn(crm_busca, "\n")] = '\0';

            int linha = buscar_linha("data/registro_medicos.csv", 2, crm_busca);
            if (linha < 0)
            {
                printf("Medico com CRM: %s, n√£o encontrado!\n", crm_busca);
                break;
            }

            if (del_row("data/registro_medicos.csv", linha))
                printf("Medico deletado com sucesso!\n");
            else
                printf("Erro ao deletar m√©dico!\n");
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

// Fun√ß√£o para convertere especialidade enum para string
const char *especialidade_string(Especialidade especialidade)
{
    switch (especialidade)
    {
    case CLINICO_GERAL:
        return "Cl√≠nico Geral";
    case PEDIATRA:
        return "Pedriatra";
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
