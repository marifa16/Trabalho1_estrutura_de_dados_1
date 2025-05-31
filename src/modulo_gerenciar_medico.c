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
            Especialidade especialidade_medico; // Variável para controlar o loop de saída
            char telefone_str[15];              // Variável para armazenar o telefone do médico como string

            // Nome médico
            do
            {
                msg_23_nome_medico();
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
                fgets(telefone_str, sizeof(telefone_str), stdin); // Lê o telefone como string
                telefone_str[strcspn(telefone_str, "\n")] = '\0'; // Remove o caractere de linha

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

                printf("O telefone informado foi: %s\n", telefone_str); // Exibe o telefone do médico

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
                        break; // Sai do loop
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

            medicos = realloc(medicos, (total_medicos + 1) * sizeof(reg_medico));
            if (medicos == NULL) // Verifica se a realocação foi sucecedida
            {
                printf("Erro ao alocar memória para o médico.\n");
                return (1);
            }

            // Salva dados do médicos
            strcpy(medicos[total_medicos].nome, nome_medico);
            medicos[total_medicos].crm = crm;
            strcpy(medicos[total_medicos].telefone, telefone_str);
            medicos[total_medicos].especialidade_medico = especialidade_medico;
            total_medicos++; // Incrementa o total de médicos

            char *valores[4];
            valores[0] = nome_medico;
            char buf_crm[16];
            snprintf(buf_crm, sizeof(buf_crm), "%d", crm);
            valores[1] = buf_crm;
            char buf_especialidade[32];
            strcpy(buf_especialidade, especialidade_string(especialidade_medico));
            valores[2] = buf_especialidade;
            valores[3] = telefone_str;

            add_row("data/registro_medicos.csv", 5, valores); // 5 colunas (id + 4 campos)
            msg_29_sucesso_cadastro_medico();
            break;
        }

        case 2: // Exibir Médico
        {
            char crm_busca[16];
            printf("Digite o CRM do médico para exibir: ");
            fgets(crm_busca, sizeof(crm_busca), stdin);
            crm_busca[strcspn(crm_busca, "\n")] = '\0';

            int id = get_id("data/registro_medicos.csv", 2, crm_busca);
            if (id < 0)
            {
                printf("Médico com CRM %s não encontrado!\n", crm_busca);
            }
            else
            {
                read_row("data/registro_medicos.csv", id); // Lê a linha do médico
            }
            break;
        }

        case 3: // Atualizar Médico
        {
            char crm_busca[16];
            printf("Digite o CRM do médico para atualizar: ");
            fgets(crm_busca, sizeof(crm_busca), stdin);
            crm_busca[strcspn(crm_busca, "\n")] = '\0';

            // Busca linha do Médico
            int linha = buscar_linha("data/registro_medicos.csv", 2, crm_busca);
            if (linha < 0)
            {
                printf("Medico com CRM %s, não encontrado!\n", crm_busca);
                return ESTADO_GERENCIAR_MEDICO; // Retorna ao menu de gerenciamento de médicos
            }

            // Ler a linha atual do médico
            FILE *arquivo = fopen("data/registro_medicos.csv", "r");
            if (!arquivo)
            {
                printf("Erro ao abrir o arquivo!\n");
                break; // Sai do case 3
            }
            char linha_csv[512];
            fgets(linha_csv, sizeof(linha_csv), arquivo);
            for (int i = 0; i <= linha; i++)
                fgets(linha_csv, sizeof(linha_csv), arquivo);
            fclose(arquivo);

            // Buffers locais para cada campo
            char id[8], nome[100], crm[16], especialidade[32], telefone[16];
            char *tokens[5];
            char *token = strtok(linha_csv, "\n");
            int i = 0;
            while (token && i < 5)
            {
                switch (i)
                 {
                    case 0: strncpy(id, token, sizeof(id)); id[sizeof(id)-1] = '\0'; tokens[0] = id; break;
                    case 1: strncpy(nome, token, sizeof(nome)); nome[sizeof(nome)-1] = '\0'; tokens[1] = nome; break;
                    case 2: strncpy(crm, token, sizeof(crm)); crm[sizeof(crm)-1] = '\0'; tokens[2] = crm; break;
                    case 3: strncpy(especialidade, token, sizeof(especialidade)); especialidade[sizeof(especialidade)-1] = '\0'; tokens[3] = especialidade; break;
                    case 4: strncpy(telefone, token, sizeof(telefone)); telefone[sizeof(telefone)-1] = '\0'; tokens[4] = telefone; break;
                }
                token = strtok(NULL, ",\n");
                i++;
            }

            int continuar = 1;
            while (continuar)
            {
                msg_25_atualizar_medico();
                char opcao_str[10];
                fgets(opcao_str, sizeof(opcao_str), stdin);
                int opcao = atoi(opcao_str);

                switch (opcao)
                {
                case 1:
                    printf("===========================\n"
                           "Insira seu novo Nome:\n"
                           "===========================\n");
                    char novo_nome[100];
                    fgets(novo_nome, sizeof(novo_nome), stdin); // Lê o novo nome do médico
                    novo_nome[strcspn(novo_nome, "\n")] = '\0'; // Remove o caractere de nova linha

                    // Validação do nome do médico
                    if (strlen(novo_nome) == 0 || strpbrk(novo_nome, "0123456789"))
                    {
                        msg_02_opcao_invalida();
                        printf("O nome do médico não pode estar vazio ou conter números.\n");
                        continue; // Volta para solicitar o nome de novo
                    }
                    strncpy(nome, novo_nome, sizeof(nome)); // Copia o novo nome para o buffer
                    nome[sizeof(nome)-1] = '\0'; // Garante que o nome esteja null-terminated
                    break;

                case 2:
                    msg_26_novo_crm();
                    char novo_crm[16];
                    fgets(novo_crm, sizeof(novo_crm), stdin);
                    novo_crm[strcspn(novo_crm, "\n")] = '\0';

                    // Validação do CRM
                    if (strlen(novo_crm) != 6 || strspn(novo_crm, "0123456789") != 6)
                    {
                        msg_02_opcao_invalida();
                        printf("O CRM deve conter 6 dígitos numéricos.\n");
                        continue; // Volta para solicitar o CRM de novo
                    }
                    strncpy(crm, novo_crm, sizeof(crm)); // Atualiza o CRM do médico
                    crm[sizeof(crm)-1] = '\0'; // Garante que o CRM esteja null-terminated
                    break;

                case 3:
                    printf("===========================\n"
                           "Insira seu novo Telefone:\n"
                           "===========================\n");
                    char novo_telefone[15];
                    fgets(novo_telefone, sizeof(novo_telefone), stdin);
                    novo_telefone[strcspn(novo_telefone, "\n")] = '\0';

                    // Validação do telefone
                    if (strlen(novo_telefone) != 11 || strspn(novo_telefone, "0123456789") != 11)
                    {
                        msg_02_opcao_invalida();
                        printf("O telefone deve conter 11 dígitos numéricos.\n");
                        continue; // Volta para solicitar o telefone de novo
                    }
                    strncpy(telefone, novo_telefone, sizeof(telefone)); // Atualiza o telefone do médico
                    telefone[sizeof(telefone)-1] = '\0'; //  Garante que o telefone esteja null-terminated
                    break;

                case 4:
                    msg_27_nova_especialidade();

                    // Exibe as opções da especialidade
                    for (int i = 0; i < TOTAL_ESPECIALIDADES; i++)
                    {
                        printf("[%d] - %s\n", i + 1, especialidade_string(i)); // Converte o enum para uma string
                    }
                    printf("Escolha uma especialidade (1-%d): ", TOTAL_ESPECIALIDADES);
                    char escolha_especialidade_str[10];
                    fgets(escolha_especialidade_str, sizeof(escolha_especialidade_str), stdin);
                    escolha_especialidade_str[strcspn(escolha_especialidade_str, "\n")] = '\0';

                    int escolha_especialidade;
                    if (sscanf(escolha_especialidade_str, "%d", &escolha_especialidade) != 1 || escolha_especialidade < 1 || escolha_especialidade > TOTAL_ESPECIALIDADES)
                    {
                        msg_02_opcao_invalida();
                        printf("Opção inválida! Por favor, escolha uma especialidade válida.\n");
                        continue; // Volta para solicitar a especialidade de novo
                    }

                    strncpy(especialidade, especialidade_string(escolha_especialidade - 1), sizeof(especialidade));
                    especialidade[sizeof(especialidade) - 1] = '\0';
                    break;

                default:
                    printf("Opção inválida!\n");
                    continue;
                }

                printf("Deseja atualizar mais alguma informação?\n");
                printf("[1] - SIM\n");
                printf("[2] - NÃO\n");
                char resp[10];
                fgets(resp, sizeof(resp), stdin);
                if (atoi(resp) == 2) // Se a resposta for NÃO
                {
                    continuar = 0; // Sai do loop
                }
            }

            // Atualizar a linha do médico
            char *valores[5];
            valores[0] = id;
            valores[1] = nome;
            valores[2] = crm;
            valores[3] = especialidade;
            valores[4] = telefone;

            if (att_row("data/registro_medicos.csv", linha, 5, valores))
                printf("Médico atualizado com sucesso!\n");
            else
                printf("Erro ao atualizar médico!\n");
            break;
        }

        case 4: // deletar
        {
            char crm_busca[16];
            printf("Digite o CRM do médico para deletar: ");
            fgets(crm_busca, sizeof(crm_busca), stdin);
            crm_busca[strcspn(crm_busca, "\n")] = '\0';

            int linha = buscar_linha("data/registro_medicos.csv", 2, crm_busca);
            if (linha < 0)
            {
                printf("Medico com CRM: %s, não encontrado!\n", crm_busca);
                break;
            }

            if (del_row("data/registro_medicos.csv", linha))
                printf("Medico deletado com sucesso!\n");
            else
                printf("Erro ao deletar médico!\n");
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

// Função para convertere especialidade enum para string
const char *especialidade_string(Especialidade especialidade)
{
    switch (especialidade)
    {
    case CLINICO_GERAL:
        return "Clínico Geral";
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
