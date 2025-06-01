#include <stdio.h>
#include "../include/estruturas.h"
#include "../include/mensagens.h"
#include "../include/auxiliar.h"

Estado tratar_menu_principal()
{
    msg_33_boas_vindas();               // exibe o menu principal
    int escolha = ler_opcao_menu(1, 5); // trata a entrada do usuário para um valor válido

    switch (escolha) // depende da escolha do usuário é levado a um novo estado
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