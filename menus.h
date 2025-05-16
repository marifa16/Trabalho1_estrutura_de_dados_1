#ifndef MENUS_H
#define MENUS_H

#include "estados.h"

Estado tratar_menu_principal(int entrada);

void tratar_menu_consuta(int entrada);
void tratar_menu_gerenciar_paciente(int entrada);
void tratar_menu_gerenciar_medico(int entrada);
void tratar_menu_modulo_relatorios(int entrada);

void mostrar_menu_principal();


#endif