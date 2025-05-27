#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estruturas.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_paciente.h"
#include "../include/validacoes.h"
#include "../include/auxiliar.h"
#include "../include/files_manager.h"

reg_paciente *pacientes = NULL; // Ponteiro para o vetor de pacientes
int total_pacientes = 0;        // Contador de pacientes cadastrados
char file_paciente[] = "data/registro_pacientes.csv";

Estado tratar_modulo_paciente()
{
    Estado estado_atual = ESTADO_GERENCIAR_PACIENTE;
    int escolha_modulo;

    do
    {
        msg_37_mostrar_modulo_paciente(); // Exibe o menu de gerenciamento de pacientes

        // Limpa o buffer de entrada se o próximo caractere for '\n'
        int c = getchar();
        if (c != '\n' && c != EOF)
        {
            ungetc(c, stdin); // Devolve o caractere se não era '\n'
        }

        char escolha_modulo_str[10];
        fgets(escolha_modulo_str, sizeof(escolha_modulo_str), stdin); // Lê a entrada como string
        escolha_modulo_str[strcspn(escolha_modulo_str, "\n")] = '\0'; // Remove o caractere de nova linha
        // printf("DEBUG: [%s]\n", escolha_modulo_str);

        // Converte a entrada para inteiro
        if (sscanf(escolha_modulo_str, "%d", &escolha_modulo) != 1)
        {
            msg_02_opcao_invalida();
            continue;
        }

        switch (escolha_modulo)
        {
        case 1: // Criar Paciente
        {
            char nome_paciente[100];
            char cpf_paciente[12];      // CPF como string (11 dígitos + '\0')
            char telefone_paciente[12]; // Telefone como string (11 dígitos + '\0')
            int opcao;

            msg_10_criar_paciente(); // Solicita o nome do paciente

            // Loop para validar o nome
            do
            {
                if (!validar_nome_paciente(nome_paciente, sizeof(nome_paciente)))
                {
                    msg_02_opcao_invalida();
                    continue; // Volta para solicitar o nome novamente
                }

                // Confirmação do nome
                opcao = validar_opcao_usuario();

                if (opcao == 1) // SIM
                {
                    break; // Nome confirmado, sai do loop
                }
                else if (opcao == 2) // NÃO
                {
                    continue; // Volta para solicitar o nome novamente
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    estado_atual = ESTADO_GERENCIAR_PACIENTE; // Retorna ao menu de gerenciamento de pacientes
                    return estado_atual;
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);

            // Loop para validar o CPF
            do
            {
                if (!validar_cpf(cpf_paciente, sizeof(cpf_paciente)))
                {
                    msg_02_opcao_invalida();
                    printf("O CPF deve conter exatamente 11 dígitos.\n");
                    continue; // Volta para solicitar o CPF novamente
                }

                // Confirmação do CPF
                limpar_buffer();
                int opcao = validar_opcao_usuario();

                if (opcao == 1) // SIM
                {
                    break; // CPF confirmado, sai do loop
                }
                else if (opcao == 2) // NÃO
                {
                    continue; // Volta para solicitar o CPF novamente
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    estado_atual = ESTADO_GERENCIAR_PACIENTE; // Retorna ao menu de gerenciamento de pacientes
                    return estado_atual;
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);

            // Loop para validar o telefone
            do
            {
                if (!validar_telefone(telefone_paciente, sizeof(telefone_paciente)))
                {
                    msg_02_opcao_invalida();
                    printf("O telefone deve conter exatamente 11 dígitos.\n");
                    continue; // Volta para solicitar o telefone novamente
                }

                // Confirmação do telefone
                limpar_buffer();
                int opcao = validar_opcao_usuario();

                if (opcao == 1) // SIM
                {
                    break; // Telefone confirmado, sai do loop
                }
                else if (opcao == 2) // NÃO
                {
                    continue; // Volta para solicitar o telefone novamente
                }
                else if (opcao == 3) // SAIR
                {
                    msg_05_retornando_menu();
                    estado_atual = ESTADO_GERENCIAR_PACIENTE; // Retorna ao menu de gerenciamento de pacientes
                    return estado_atual;
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            } while (1);

            // Salva os dados do paciente
            pacientes = realloc(pacientes, (total_pacientes + 1) * sizeof(reg_paciente));
            if (pacientes == NULL)
            {
                printf("Erro ao alocar memória para o paciente.\n");
                exit(1);
            }
            strcpy(pacientes[total_pacientes].nome, nome_paciente);         // Salva o nome
            strcpy(pacientes[total_pacientes].cpf, cpf_paciente);           // Salva o CPF
            strcpy(pacientes[total_pacientes].telefone, telefone_paciente); // Salva o telefone
            total_pacientes++;                                              // Incrementa o contador de pacientes

            char *valores[3];

            valores[0] = nome_paciente;
            valores[1] = cpf_paciente;
            valores[2] = telefone_paciente;
            add_row(file_paciente, 4, valores); // 4 colunas (id + 3 campos)
            msg_15_sucesso_cadastro();
            break;
        }

        case 2:
        { // Exibir Paciente
            char cpf_busca[12];
            printf("Digite o CPF do paciente para exibir: ");
            fgets(cpf_busca, sizeof(cpf_busca), stdin);
            cpf_busca[strcspn(cpf_busca, "\n")] = '\0';

            int id = get_id(file_paciente, 2, cpf_busca);
            if (id < 0)
            {
                printf("Paciente com CPF %s não encontrado!\n", cpf_busca);
            }
            else
            {
                read_row(file_paciente, id);
            }
            break;
        }
        case 3: // Atualizar Paciente
            // Implementação para atualizar paciente
            printf("Atualizando paciente...\n");
            break;

        case 4:
        { // Deletar Paciente
            char cpf_busca[12];
            printf("Digite o CPF do paciente para deletar: ");
            fgets(cpf_busca, sizeof(cpf_busca), stdin);
            cpf_busca[strcspn(cpf_busca, "\n")] = '\0';

            int linha = buscar_linha(file_paciente, 2, cpf_busca);
            if (linha < 0)
            {
                printf("Paciente com CPF %s não encontrado!\n", cpf_busca);
                break;
            }

            if (del_row(file_paciente, linha))
                printf("Paciente deletado com sucesso!\n");
            else
                printf("Erro ao deletar paciente!\n");
            break;
        }
        case 5: // Voltar ao menu principal
            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;

        case 6: // Sair do programa
            estado_atual = ESTADO_SAIR;
            break;

        default:
            msg_02_opcao_invalida();
            break;
        }
    } while (estado_atual == ESTADO_GERENCIAR_PACIENTE);

    return estado_atual;
}