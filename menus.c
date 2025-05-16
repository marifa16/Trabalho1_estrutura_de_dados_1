#include <stdio.h>
#include "menus.h"

void mostrar_menu_principal()
{
    // 33 - Boas-vindas
    printf("===========================\n"
           "O que deseja realizar hoje:\n\n"
           "[1] - Agendar Consulta\n"
           "[2] - Gerenciar Paciente\n"
           "[3] - Gerenciar Médico\n"
           "[4] - Módulo Relatório\n"
           "[5] - Sair\n"
           "===========================\n");
}

Estado tratar_menu_principal(int entrada)
{
    switch (entrada)
    {
    case 1:
        return ESTADO_MENU_CONSULTA;
    case 2:
        return ESTADO_GERENCIAR_PACIENTE;
    case 3:
        return ESTADO_GERENCIAR_MEDICO;
    case 4:
        return ESTADO_MODULO_RELATORIOS;
    case 5:
        return ESTADO_SAIR;
    default:
        return ESTADO_MENU_PRINCIPAL;
    }
}
void mostrar_modulo_consulta()
{
    printf("\nMódulo Consulta:\n");
    printf("(Retornando automaticamente...)\n");
}

Estado tratar_modulo_consulta(int entrada)
{
    return ESTADO_MENU_CONSULTA;
}
void mostrar_modulo_paciente()
{
    printf("\nGerenciar Paciente:\n");
    printf("(Retornando automaticamente...)\n");
}

Estado tratar_modulo_paciente(int entrada)
{
    return ESTADO_GERENCIAR_PACIENTE;
}

void mostrar_modulo_medico()
{
    printf("\nGerenciar Médico:\n");
    printf("(Retornando automaticamente...)\n");
}

Estado tratar_modulo_medico(int entrada)
{
    return ESTADO_GERENCIAR_MEDICO;
}

void mostrar_modulo_relatorios()
{
    printf("\nMódulo Relatórios:\n");
    printf("(Retornando automaticamente...)\n");
}

Estado tratar_modulo_relatorios(int entrada)
{
    return ESTADO_MODULO_RELATORIOS;
}
