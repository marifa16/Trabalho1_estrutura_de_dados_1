#include <stdio.h>
#include "../include/modulo_consulta.h"
#include "../include/estados.h"
#include "../include/mensagens.h"

Estado tratar_modulo_consulta(int entrada)
{
    switch (entrada)
    {
    case 1: // Agendar
        // validar paciente
        // listar médicos
        // perguntar qual medico deseja se consultar
        // receber o médico escolhido
        // opção é válida?
        // listar horários disponíveis
        // exibir horários disponíveis
        // usuario escolhe horário
        // validar horário
        // gera um reg_consulta
        // salvar reg_consulta e exibir mensagem de sucesso
        printf("Lógica concluida, seguir de volta pro menu principal.\n");

        return ESTADO_MENU_CONSULTA; //

    case 2: // Cancelar
        printf("Cancelamento de consulta selecionado.\n");
        // Aqui você pode chamar a função de cancelamento

        return ESTADO_MENU_CONSULTA;

    case 3: // Retornar menu
        return ESTADO_MENU_PRINCIPAL;

    case 4: // Sair programa
        return ESTADO_SAIR;

    default: // inválido
        msg_02_opcao_invalida();
        return ESTADO_MENU_CONSULTA;
    }
}