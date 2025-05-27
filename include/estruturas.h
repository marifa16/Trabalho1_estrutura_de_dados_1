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

// Definição da struct para horários
typedef struct {
    int hora;          // Hora do atendimento (8-16)
    int ocupado;       // 0 = Livre, 1 = Ocupado
    char paciente[50]; // Nome do paciente (opcional)
    char medico[50];   // Nome do médico (opcional)
} Horario;

// Definição da struct para médicos
typedef struct
{
    int id_medico;
    char nome[100];
    int crm;
    char especialidade[100];
    int telefone;
} reg_medico;

// Definição da struct para pacientes
typedef struct
{
    int id_paciente;
    char nome[120];
    char cpf[12];      // Alterado para string (11 dígitos + '\0')
    char telefone[12]; // Alterado para string (11 dígitos + '\0')
} reg_paciente;


// Declaração das variáveis globais como extern
extern reg_medico *medicos; // Ponteiro para o vetor de médicos
extern int total_medicos;   // Contador de médicos cadastrados

#endif