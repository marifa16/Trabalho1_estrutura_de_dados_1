#include <stdio.h>
#include "../include/estados.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_paciente.h"

Estado tratar_modulo_paciente(int entrada)
{
    switch (entrada)
    {
    case 1: // Criar Paciente
        printf("Criar paciente selecionado.\n");
        return ESTADO_GERENCIAR_PACIENTE;

    case 2: // Exibir Paciente
        printf("Exibir paciente selecionado.\n");
        return ESTADO_GERENCIAR_PACIENTE;

    case 3: // Atualizar Paciente
        printf("Atualizar paciente selecionado.\n");
        return ESTADO_GERENCIAR_PACIENTE;

    case 4: // Deletar Paciente
        printf("Deletar paciente selecionado.\n");
        return ESTADO_GERENCIAR_PACIENTE;

    case 5: // Voltar ao menu principal
        return ESTADO_MENU_PRINCIPAL;

    case 6: // Sair do programa
        return ESTADO_SAIR;

    default: // Opção inválida
        msg_02_opcao_invalida();
        return ESTADO_GERENCIAR_PACIENTE;
    }
}