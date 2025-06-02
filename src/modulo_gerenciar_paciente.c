#include <stdio.h>
#include <string.h>
#include <ctype.h>                                // Adicione esta linha
#include "../include/estruturas.h"                // Estruturas utilizadas no código
#include "../include/mensagens.h"                 // mensagens padronizadas
#include "../include/modulo_gerenciar_paciente.h" // header do módulo paciente
#include "../include/auxiliar.h"                  // funções auxiliares para suporte as funções no módulo
#include "../include/files_manager.h"             // funções auxiliares para funções com arquivos

Estado tratar_modulo_paciente() // função para tratar as funcionalidades do modulo paciente retornado o estado
{
    Estado estado_atual = ESTADO_GERENCIAR_PACIENTE; // estado padrão

    // variáveis compartilhadas para usar no módulo de código
    int escolha_modulo;
    char nome_paciente[100];
    char cpf_paciente[12];      // 11 digitos + \n
    char telefone_paciente[12]; // 11 digitos + \n
    char *valores[4];           // Para uso em add_row, att_row, etc

    do // Loop que irá manter o usuário no módulo enquanto o estado permanecer
    {
        msg_37_mostrar_modulo_paciente();      // Exibe o menu de gerenciamento de pacientes
        escolha_modulo = ler_opcao_menu(1, 6); // valida se a entrada é válida

        switch (escolha_modulo) // pula para a funcionalidade selecionada
        {
        case 1: // Criar Paciente
        {
            msg_10_criar_paciente();

            // Loop para validar o nome
            do
            {
                msg_11_nome_paciente();
                if (!validar_nome_padrao(nome_paciente, sizeof(nome_paciente)))
                {
                    msg_02_opcao_invalida();
                    continue;
                }
                msg_12_validar_nome_paciente();
                printf("%s\n", nome_paciente);
                msg_40_opcoes();
                int opcao = ler_opcao_menu(1, 3);
                if (opcao == 1)
                    break;
                else if (opcao == 2)
                    continue;
                else if (opcao == 3)
                {
                    msg_05_retornando_menu();
                    return estado_atual;
                }
            } while (1);

            // Loop para validar o CPF
            do
            {
                msg_13_informar_cpf();
                fgets(cpf_paciente, sizeof(cpf_paciente), stdin);
                cpf_paciente[strcspn(cpf_paciente, "\n")] = '\0';

                // Validação simples: tamanho e só dígitos
                if (strlen(cpf_paciente) != 11)
                {
                    msg_02_opcao_invalida();
                    msg_cpf_invalido();
                    continue;
                }
                int valido = 1;
                for (size_t i = 0; i < strlen(cpf_paciente); i++)
                {
                    if (cpf_paciente[i] < '0' || cpf_paciente[i] > '9')
                    {
                        valido = 0;
                        break;
                    }
                }
                if (!valido)
                {
                    msg_02_opcao_invalida();
                    msg_cpf_invalido();
                    continue;
                }
                // Exibe o CPF digitado e pede confirmação
                printf("CPF informado: %s\n", cpf_paciente);
                msg_40_opcoes();
                limpar_buffer();
                int opcao = ler_opcao_menu(1, 3); // 1=SIM, 2=NÃO, 3=SAIR
                if (opcao == 1)
                    break;
                else if (opcao == 2)
                    continue;
                else if (opcao == 3)
                {
                    msg_05_retornando_menu();
                    return estado_atual;
                }
            } while (1);

            // Loop para validar o telefone
            do
            {
                msg_14_informar_telefone();
                if (!validar_telefone_padrao(telefone_paciente, sizeof(telefone_paciente)))
                {
                    msg_02_opcao_invalida();
                    msg_telefone_invalido();
                    continue;
                }
                printf("Telefone informado: %s\n", telefone_paciente); // Opcional, para feedback
                msg_40_opcoes();
                limpar_buffer();
                int opcao = ler_opcao_menu(1, 3); // 1=SIM, 2=NÃO, 3=SAIR
                if (opcao == 1)
                    break;
                else if (opcao == 2)
                    continue;
                else if (opcao == 3)
                {
                    msg_05_retornando_menu();
                    return estado_atual;
                }
            } while (1);

            // atribuir a um vetor para ser repassado mais facilmente a função add_row
            valores[0] = nome_paciente;
            valores[1] = cpf_paciente;
            valores[2] = telefone_paciente;

            add_row(ARQ_PACIENTES, 4, valores); // função para adicionar a linha

            // Alocação dinâmica de memória para vetor de pacientes
            extern reg_paciente *pacientes;
            extern int total_pacientes;

            reg_paciente *novo = realloc(pacientes, (total_pacientes + 1) * sizeof(reg_paciente));
            if (novo == NULL)
            {
                msg_erro_memoria_paciente();
                break;
            }
            pacientes = novo;
            strcpy(pacientes[total_pacientes].nome, nome_paciente);
            strcpy(pacientes[total_pacientes].cpf, cpf_paciente);
            strcpy(pacientes[total_pacientes].telefone, telefone_paciente);
            total_pacientes++;

            msg_15_sucesso_cadastro();
            break;
        }
        case 2: // Exibir Paciente
        {
            carregar_pacientes_do_arquivo();
            msg_digite_cpf_exibir();

            fgets(cpf_paciente, sizeof(cpf_paciente), stdin);
            cpf_paciente[strcspn(cpf_paciente, "\n")] = '\0';

            // Valide o CPF já lido
            if (!cpf_valido(cpf_paciente))
            {
                msg_02_opcao_invalida();
                msg_cpf_invalido();
                break;
            }

            // Busca o paciente em memória usando ponteiro
            reg_paciente *p = buscar_paciente_por_cpf(cpf_paciente);
            if (p == NULL)
            {
                msg_paciente_nao_encontrado(cpf_paciente);
            }
            else
            {
                printf("========================================\n");
                printf("Nome    : %s\n", p->nome);
                printf("CPF     : %s\n", p->cpf);
                printf("Telefone: %s\n", p->telefone);
                printf("========================================\n");

                // Manipulação de string com ponteiros
                // Cópia
                char nome_copia[120];
                strcpy(nome_copia, p->nome);

                // Concatenação
                char mensagem[256] = "Paciente: ";
                strcat(mensagem, p->nome);
                strcat(mensagem, " | Telefone: ");
                strcat(mensagem, p->telefone);

                // Comparação
                if (strcmp(p->nome, "Anderson Carlos") == 0)
                {
                    printf("Bem-vindo, Anderson!\n");
                }

                printf("Mensagem concatenada: %s\n", mensagem);
                printf("Nome copiado: %s\n", nome_copia);
            }
            limpar_buffer();
            break;
        }
        case 3: // Atualizar Paciente
        {
            msg_digite_cpf_atualizar();

            fgets(cpf_paciente, sizeof(cpf_paciente), stdin);
            cpf_paciente[strcspn(cpf_paciente, "\n")] = '\0';

            if (strlen(cpf_paciente) == 0)
            {
                msg_cpf_invalido();
                break;
            }
            if (strlen(cpf_paciente) != 11)
            {
                msg_cpf_invalido();
                break;
            }
            int cpf_formato_valido = 1;
            for (size_t i = 0; i < strlen(cpf_paciente); i++)
            {
                if (!isdigit(cpf_paciente[i]))
                {
                    cpf_formato_valido = 0;
                    break;
                }
            }
            if (!cpf_formato_valido)
            {
                msg_erro_cpf_numeros();
                limpar_buffer();
                break;
            }

            limpar_buffer();
            atualizar_paciente(ARQ_PACIENTES, cpf_paciente, valores);

            carregar_pacientes_do_arquivo();
            break;
        }
        case 4: // Deletar Paciente
        {
            carregar_pacientes_do_arquivo();

            msg_digite_cpf_deletar();
            fgets(cpf_paciente, sizeof(cpf_paciente), stdin);
            cpf_paciente[strcspn(cpf_paciente, "\n")] = '\0';

            // Validação simples: tamanho e só dígitos
            if (strlen(cpf_paciente) != 11)
            {
                msg_02_opcao_invalida();
                msg_cpf_invalido();
                break;
            }
            int valido = 1;
            for (size_t i = 0; i < strlen(cpf_paciente); i++)
            {
                if (cpf_paciente[i] < '0' || cpf_paciente[i] > '9')
                {
                    valido = 0;
                    break;
                }
            }
            if (!valido)
            {
                msg_02_opcao_invalida();
                msg_cpf_invalido();
                break;
            }

            int linha_del = buscar_linha(ARQ_PACIENTES, 2, cpf_paciente); // busca a linha com o cpf do paciente
            if (linha_del < 0)                                            // se a linha não for válida
            {
                msg_paciente_nao_encontrado(cpf_paciente);
                break;
            }

            if (del_row(ARQ_PACIENTES, linha_del)) // deleta a linha do id_informado e usa o retorno dela para exibir mensagem
                msg_paciente_deletado();
            else
                msg_erro_deletar_paciente();

            limpar_buffer();
            break;
        }
        case 5: // volta ao menu principal
        {
            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;
        }
        case 6: // sair do código
        {
            estado_atual = ESTADO_SAIR;
            break;
        }
        default: // opção inválida
        {
            msg_02_opcao_invalida();
            break;
        }
        }

    } while (estado_atual == ESTADO_GERENCIAR_PACIENTE);

    return estado_atual; // retornar o estado atual que é atualizado no loop
}
