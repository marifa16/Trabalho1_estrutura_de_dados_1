#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "../include/mensagens.h"                 // Inclui o cabeçalho de mensagens
#include "../include/estados.h"                   // Inclui o cabeçalho de estados
#include "../include/modulo_consulta.h"           // Inclui o cabeçalho do módulo de consulta
#include "../include/modulo_gerenciar_paciente.h" // Inclui o cabeçalho do módulo de gerenciamento de pacientes
#include "../include/modulo_gerenciar_medico.h"   // Inclui o cabeçalho do módulo de gerenciamento de médicos
#include "../include/modulo_relatorios.h"         // Inclui o cabeçalho do módulo de relatórios
#include "../include/menu_principal.h"            // Inclui o cabeçalho do menu principal
#include "../include/files_manager.h"             // Inclui o cabeçalho do arquivo de gerenciamento de arquivos

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");            // Configura o locale para português do Brasil com suporte a UTF-8
    Estado estado_atual = ESTADO_MENU_PRINCIPAL; // variável para controlar os menus             // variável para armazenar a entrada do usuário

    inicializar_arquivos_csv();

    msg_00_informacoes_programa();

    while (estado_atual != ESTADO_SAIR) // enquanto o estado
    {
        // clear(); // limpa a tela
        switch (estado_atual)
        {
        case ESTADO_MENU_PRINCIPAL:
        {

            estado_atual = tratar_menu_principal(); // trata a entrada do menu principal
            break;
        }

        case ESTADO_MENU_CONSULTA:
        {
            estado_atual = tratar_modulo_consulta();
            break;
        }

        case ESTADO_GERENCIAR_PACIENTE:
            estado_atual = tratar_modulo_paciente();
            break;

        case ESTADO_GERENCIAR_MEDICO:
            estado_atual = tratar_modulo_medico();
            break;

        case ESTADO_MODULO_RELATORIOS:
            estado_atual = tratar_modulo_relatorios();
            break;

        default:
            estado_atual = ESTADO_MENU_PRINCIPAL;
        }
    }

    msg_03_finalizacao();
    return 0;
}
