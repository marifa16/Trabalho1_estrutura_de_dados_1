#include <stdio.h>
#include "../include/estruturas.h"
#include "../include/mensagens.h"
#include "../include/modulo_relatorios.h"

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
            // Lógica para exibir consultas de pacientes
            break;

        case 2: // Consultas de médicos
            // Lógica para exibir consultas de médicos
            break;

        case 3: // Consultas de especialistas
            // Lógica para exibir consultas de especialistas
            break;

        case 4: // Consultas do dia
            // Lógica para exibir consultas do dia
            break;

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
    } while (estado_atual == ESTADO_MODULO_RELATORIOS);

    return estado_atual;
}
