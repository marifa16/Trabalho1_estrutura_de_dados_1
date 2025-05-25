#include <stdio.h>
#include "../include/estruturas.h"
#include "../include/mensagens.h"

Estado tratar_menu_principal()
{
    int escolha = 0;

    msg_33_boas_vindas();  // exibe o menu principal
    scanf("%d", &escolha); // lê a entrada do usuário

    switch (escolha)
    {
    case 1: // Módulo Consultas
        return ESTADO_MENU_CONSULTA;

    case 2: // Módulo Pacientes
        return ESTADO_GERENCIAR_PACIENTE;

    case 3: // Módulo Médicos
        return ESTADO_GERENCIAR_MEDICO;

    case 4: // Módulo Relatórios
        return ESTADO_MODULO_RELATORIOS;

    case 5: // Sair do programa
        return ESTADO_SAIR;

    default: // Opção inválida
        msg_02_opcao_invalida();
        return ESTADO_MENU_PRINCIPAL;
    }
}