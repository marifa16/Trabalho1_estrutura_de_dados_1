#include "../include/estados.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_medico.h"

Estado tratar_modulo_medico(int entrada)
{
    switch (entrada)
    {
    case 1: // criar
        msg_22_criar_medico();
        msg_23_nome_medico();
        // Aqui você pode adicionar a lógica para criar um médico
        return ESTADO_MENU_PRINCIPAL;
    case 2: // exibir
        // exibir_medico();
        return ESTADO_MENU_PRINCIPAL;
    case 3: // atualizar
        msg_25_atualizar_medico();
        //
        return ESTADO_MENU_PRINCIPAL;

    case 4: // deletar
        // deletar_medico();
        return ESTADO_MENU_PRINCIPAL;

    case 5: // voltar
        return ESTADO_MENU_PRINCIPAL;

    case 6: // sair
        return ESTADO_SAIR;

    default: // inválido
        msg_02_opcao_invalida();
        return ESTADO_GERENCIAR_MEDICO;
    }
}