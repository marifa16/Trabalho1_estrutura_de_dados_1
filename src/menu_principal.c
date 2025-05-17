#include <stdio.h>
#include "../include/estados.h"
#include "../include/mensagens.h"

Estado tratar_menu_principal(int entrada)
{
    switch (entrada)
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