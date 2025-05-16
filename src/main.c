#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "../include/mensagens.h"                 // Inclua o cabeçalho de mensagens
#include "../include/estados.h"                   // Inclua o cabeçalho de estados
#include "../include/menus.h"                     // Inclua o cabeçalho do módulo de menus
#include "../include/modulo_consulta.h"           // Inclua o cabeçalho do módulo de consulta
#include "../include/modulo_gerenciar_paciente.h" // Inclua o cabeçalho do módulo de gerenciamento de pacientes
#include "../include/modulo_gerenciar_medico.h"   // Inclua o cabeçalho do módulo de gerenciamento de médicos
#include "../include/modulo_relatorios.h"         // Inclua o cabeçalho do módulo de relatórios

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Configura o locale para português do Brasil com suporte a UTF-8

    informacoes_boas_vindas(); // mensagem de boas-vindas

    Estado estado_atual = ESTADO_MENU_PRINCIPAL; // variável para controlar os menus
    int entrada = 0;                             // variável para armazenar a entrada do usuário

    while (estado_atual != ESTADO_SAIR) // enquanto o estado
    {
        // limpa a tela para melhor legibilidade
        system("clear || cls"); // Limpa a tela (Linux ou Windows)
        // Exibe o menu atual com base no estado
        // e aguarda a entrada do usuário

        switch (estado_atual)
        {
        case ESTADO_MENU_PRINCIPAL:
            mostrar_menu_principal();
            scanf("%d", &entrada);
            estado_atual = tratar_menu_principal(entrada);
            break;
        case ESTADO_MENU_CONSULTA:
            mostrar_modulo_consulta();
            scanf("%d", &entrada);
            estado_atual = tratar_modulo_consulta(entrada);
            break;
        case ESTADO_GERENCIAR_PACIENTE:
            mostrar_modulo_paciente();
            scanf("%d", &entrada);
            estado_atual = tratar_modulo_paciente(entrada);
            break;
        case ESTADO_GERENCIAR_MEDICO:
            mostrar_modulo_medico();
            scanf("%d", &entrada);
            estado_atual = tratar_modulo_medico(entrada);
            break;
        case ESTADO_MODULO_RELATORIOS:
            mostrar_modulo_relatorios();
            scanf("%d", &entrada);
            estado_atual = tratar_modulo_relatorios(entrada);
            break;
        default:
            estado_atual = ESTADO_MENU_PRINCIPAL;
        }
    }

    mensagem_encerramento();
    return 0;
}
