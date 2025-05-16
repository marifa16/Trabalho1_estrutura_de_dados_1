#include <stdio.h>
#include "../include/estados.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_paciente.h"

Estado tratar_modulo_consulta(int entrada)
{
    switch (entrada)
    {
    case 1:
        printf("Agendamento de consulta selecionado.\n");
        return ESTADO_MENU_CONSULTA; // Pode ser ajustado para outro estado, se necessário
    case 2:
        printf("Cancelamento de consulta selecionado.\n");
        return ESTADO_MENU_CONSULTA; // Pode ser ajustado para outro estado, se necessário
    case 3:
        return ESTADO_MENU_PRINCIPAL;
    default:
        printf("Opção inválida. Retornando ao menu de consulta.\n");
        return ESTADO_MENU_CONSULTA;
    }
}