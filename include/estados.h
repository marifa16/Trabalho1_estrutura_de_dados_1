#ifndef ESTADOS_H // Se ainda não definimos ESTADOS_H
#define ESTADOS_H // ... Agora definimos ESTADOS_H

typedef enum {  // É uma forma de criar um apelido para um tipo existente.
    ESTADO_MENU_PRINCIPAL, // Estado do menu principal
    ESTADO_MENU_CONSULTA, 
    ESTADO_GERENCIAR_PACIENTE,
    ESTADO_GERENCIAR_MEDICO,
    ESTADO_MODULO_RELATORIOS,
    ESTADO_SAIR
} Estado;

#endif