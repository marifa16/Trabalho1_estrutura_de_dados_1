#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "../include/mensagens.h"                 // Inclui o cabeçalho de mensagens
#include "../include/estruturas.h"                // Inclui o cabeçalho das estruturas
#include "../include/modulo_consulta.h"           // Inclui o cabeçalho do módulo de consulta
#include "../include/modulo_gerenciar_paciente.h" // Inclui o cabeçalho do módulo de gerenciamento de pacientes
#include "../include/modulo_gerenciar_medico.h"   // Inclui o cabeçalho do módulo de gerenciamento de médicos
#include "../include/modulo_relatorios.h"         // Inclui o cabeçalho do módulo de relatórios
#include "../include/menu_principal.h"            // Inclui o cabeçalho do menu principal
#include "../include/files_manager.h"             // Inclui o cabeçalho do arquivo de gerenciamento de arquivos

reg_paciente *pacientes = NULL;
int total_pacientes = 0;

int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");            // Configura o locale para português do Brasil com suporte a UTF-8
    Estado estado_atual = ESTADO_MENU_PRINCIPAL; // variável para controlar os menus
    inicializar_arquivos_csv();                  // responsável por inicalizar os arquivos persistentes do sistema.

    msg_00_informacoes_programa();
    while (estado_atual != ESTADO_SAIR) // enquanto o estado não for o estado de sair o código permanece em um loop principal
    {
        switch (estado_atual) // o estado atual é a saída de cada módulo do código
        {
        case ESTADO_MENU_PRINCIPAL:
        {
            estado_atual = tratar_menu_principal(); // lógica do menu principal (nomenclatura de inicio de projeto não foi tão boa :/)
            break;
        }
        case ESTADO_MENU_CONSULTA:
        {
            estado_atual = tratar_modulo_consulta(); // lógica do módulo consulta
            break;
        }
        case ESTADO_GERENCIAR_PACIENTE:
            estado_atual = tratar_modulo_paciente(); // lógica do módulo paciente
            break;
        case ESTADO_GERENCIAR_MEDICO:
            estado_atual = tratar_modulo_medico(); // lógica do módulo médico
            break;
        case ESTADO_MODULO_RELATORIOS:
            estado_atual = tratar_modulo_relatorios(); // lógica do módulo relatorios
            break;
        default:
            estado_atual = ESTADO_MENU_PRINCIPAL; // Permanece no módulo atual
        }
    }

    msg_03_finalizacao(); // encerra execução do programa

    // Libera memória alocada dinamicamente
    free(pacientes);
    pacientes = NULL;

    // Se usar médicos em memória:
    extern reg_medico *medicos;
    free(medicos);
    medicos = NULL;

    return 0;
}
