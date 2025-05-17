#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "../include/mensagens.h"                 // Inclua o cabeçalho de mensagens
#include "../include/estados.h"                   // Inclua o cabeçalho de estados
#include "../include/modulo_consulta.h"           // Inclua o cabeçalho do módulo de consulta
#include "../include/modulo_gerenciar_paciente.h" // Inclua o cabeçalho do módulo de gerenciamento de pacientes
#include "../include/modulo_gerenciar_medico.h"   // Inclua o cabeçalho do módulo de gerenciamento de médicos
#include "../include/modulo_relatorios.h"         // Inclua o cabeçalho do módulo de relatórios
#include "../include/menu_principal.h"            // Inclua o cabeçalho do menu principal

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");            // Configura o locale para português do Brasil com suporte a UTF-8
    Estado estado_atual = ESTADO_MENU_PRINCIPAL; // variável para controlar os menus
    int entrada, escolha_modulo = 0;             // variável para armazenar a entrada do usuário

    msg_00_informacoes_programa();

    while (estado_atual != ESTADO_SAIR) // enquanto o estado
    {
        // clear(); // limpa a tela
        switch (estado_atual)
        {
        case ESTADO_MENU_PRINCIPAL:
        {
            msg_33_boas_vindas();                          // exibe o menu principal
            scanf("%d", &entrada);                         // lê a entrada do usuário
            estado_atual = tratar_menu_principal(entrada); // trata a entrada do menu principal
            break;
        }

        case ESTADO_MENU_CONSULTA:
        {
            do
            {
                // clear(); // limpa a tela

                msg_01_agendar_consulta();
                scanf("%d", &escolha_modulo);
                estado_atual = tratar_modulo_consulta(escolha_modulo);
            } while (estado_atual == ESTADO_MENU_CONSULTA);
            break;
        }

        case ESTADO_GERENCIAR_PACIENTE:
        {
            do
            {
                msg_37_mostrar_modulo_paciente();
                scanf("%d", &escolha_modulo);
                estado_atual = tratar_modulo_paciente(escolha_modulo);
            } while (estado_atual == ESTADO_GERENCIAR_PACIENTE);
            break;
        }

        case ESTADO_GERENCIAR_MEDICO:
        {
            do
            {
                msg_38_mostrar_modulo_medico();
                scanf("%d", &escolha_modulo);
                estado_atual = tratar_modulo_medico(escolha_modulo);
            } while (estado_atual == ESTADO_GERENCIAR_MEDICO);
            break;
        }

        case ESTADO_MODULO_RELATORIOS:
        {
            do
            {
                msg_39_mostrar_modulo_relatorios();

                scanf("%d", &escolha_modulo);
                estado_atual = tratar_modulo_relatorios(escolha_modulo);
            } while (estado_atual == ESTADO_MODULO_RELATORIOS);
            break;
        }

        default:
            estado_atual = ESTADO_MENU_PRINCIPAL;
        }
    }

    msg_03_finalizacao();
    return 0;
}
