#include <stdio.h>
#include "../include/menus.h"

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

Estado tratar_modulo_paciente(int entrada)
{
    return ESTADO_GERENCIAR_PACIENTE;
}

Estado tratar_modulo_medico(int entrada)
{
    return ESTADO_GERENCIAR_MEDICO;
}

Estado tratar_modulo_relatorios(int entrada)
{
    return ESTADO_MODULO_RELATORIOS;
}
