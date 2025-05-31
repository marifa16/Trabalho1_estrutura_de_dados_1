#include <stdio.h>
#include <string.h>
#include "../include/estruturas.h"                // Estruturas utilizadas no código
#include "../include/mensagens.h"                 // mensagens padronizadas
#include "../include/modulo_gerenciar_paciente.h" // header do módulo paciente
#include "../include/validacoes.h"                // biblioteca de funções de validação
#include "../include/auxiliar.h"                  // funções auxiliares para suporte as funções no módulo
#include "../include/files_manager.h"             // funções auxiliares para funções com arquivos

char file_paciente[] = "data/registro_pacientes.csv"; // caminho do arquivo que será usado

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
                if (!validar_nome_paciente(nome_paciente, sizeof(nome_paciente)))
                {
                    msg_02_opcao_invalida();
                    continue;
                }
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

            // Loop para validar o CPF
            do
            {
                if (!validar_cpf(cpf_paciente, sizeof(cpf_paciente)))
                {
                    msg_02_opcao_invalida();
                    msg_cpf_invalido();
                    continue;
                }
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
                if (!validar_telefone(telefone_paciente, sizeof(telefone_paciente)))
                {
                    msg_02_opcao_invalida();
                    msg_telefone_invalido();
                    continue;
                }
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

            add_row(file_paciente, 4, valores); // função para adicionar a linha
            msg_15_sucesso_cadastro();
            break;
        }
        case 2: // Exibir Paciente
        {
            msg_digite_cpf_exibir(); // essa lógica se repete abaixo, como já fiz a parte grosseira não modularizei
            // limpar_buffer();
            // lê uma linha digitada pelo usuário do tamanho de cpf_paciente
            fgets(cpf_paciente, sizeof(cpf_paciente), stdin);
            // strcspn(cpf_paciente, "\n") retorna a posição do primeiro \n (nova linha) encontrado.
            // Substitui esse caractere por \0 (fim de string), removendo o ENTER digitado pelo usuário.
            cpf_paciente[strcspn(cpf_paciente, "\n")] = '\0';

            // Chama a função validar_cpf para verificar se o CPF digitado é válido (tamanho correto, só números, etc).
            // Se não for válido, entra no bloco do if.
            if (!validar_cpf(cpf_paciente, sizeof(cpf_paciente)))
            {
                msg_02_opcao_invalida();
                msg_cpf_invalido();
                break;
            }

            int id = get_id(file_paciente, 2, cpf_paciente); // função para resgatar o id do paciente pelo cpf
            if (id < 0)                                      // se id não for válido
                msg_paciente_nao_encontrado(cpf_paciente);
            else                             // se for válido
                read_row(file_paciente, id); // função para ler a linha com o id
            break;
        }
        case 3: // Atualizar Paciente
        {
            msg_digite_cpf_exibir();
            // limpar_buffer();
            // lê uma linha digitada pelo usuário do tamanho de cpf_paciente
            fgets(cpf_paciente, sizeof(cpf_paciente), stdin);
            // strcspn(cpf_paciente, "\n") retorna a posição do primeiro \n (nova linha) encontrado.
            // Substitui esse caractere por \0 (fim de string), removendo o ENTER digitado pelo usuário.
            cpf_paciente[strcspn(cpf_paciente, "\n")] = '\0';

            // Chama a função validar_cpf para verificar se o CPF digitado é válido (tamanho correto, só números, etc).
            // Se não for válido, entra no bloco do if.
            if (!validar_cpf(cpf_paciente, sizeof(cpf_paciente)))
            {
                msg_02_opcao_invalida();
                msg_cpf_invalido();
                break;
            }

            atualizar_paciente(file_paciente, cpf_paciente, valores); // função para atualizar o paciente
            break;
        }
        case 4: // Deletar Paciente
        {
            msg_digite_cpf_deletar();
            // limpar_buffer();
            // lê uma linha digitada pelo usuário do tamanho de cpf_paciente
            fgets(cpf_paciente, sizeof(cpf_paciente), stdin);
            // strcspn(cpf_paciente, "\n") retorna a posição do primeiro \n (nova linha) encontrado.
            // Substitui esse caractere por \0 (fim de string), removendo o ENTER digitado pelo usuário.
            cpf_paciente[strcspn(cpf_paciente, "\n")] = '\0';

            // Chama a função validar_cpf para verificar se o CPF digitado é válido (tamanho correto, só números, etc).
            // Se não for válido, entra no bloco do if.
            if (!validar_cpf(cpf_paciente, sizeof(cpf_paciente)))
            {
                msg_02_opcao_invalida();
                msg_cpf_invalido();
                break;
            }

            int linha_del = buscar_linha(file_paciente, 2, cpf_paciente); // busca a linha com o cpf do paciente
            if (linha_del < 0)                                            // se a linha não for válida
            {
                msg_paciente_nao_encontrado(cpf_paciente);
                break;
            }

            if (del_row(file_paciente, linha_del)) // deleta a linha do id_informado e usa o retorno dela para exibir mensagem
                msg_paciente_deletado();
            else
                msg_erro_deletar_paciente();
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
