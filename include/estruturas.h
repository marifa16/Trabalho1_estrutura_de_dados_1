#ifndef ESTRUTURAS_H // Se ainda não definimos ESTADOS_H
#define ESTRUTURAS_H // ... Agora definimos ESTADOS_H

#include <time.h> // Inclui a biblioteca para usar struct tm

typedef enum
{                          // É uma forma de criar um apelido para um tipo existente.
    ESTADO_MENU_PRINCIPAL, // Estado do menu principal
    ESTADO_MENU_CONSULTA,
    ESTADO_GERENCIAR_PACIENTE,
    ESTADO_GERENCIAR_MEDICO,
    ESTADO_MODULO_RELATORIOS,
    ESTADO_SAIR
} Estado;

// Definição da struct para consultas
typedef struct
{   
    int id_consulta;
    int id_paciente;
    int id_medico;
    struct tm data_hora;
    int cpf_paciente;
    int crm_medico;
    int dia, mes, ano;
} reg_consulta;

// Definição da struct para médicos
typedef struct
{
    char nome[100];
    int crm;
    char especialidade[100];
    int telefone;
} reg_medico;

// Definição da struct para pacientes
typedef struct {
    int id_paciente;
    char nome[120];
    int cpf;
    int telefone;
} reg_paciente;

#endif