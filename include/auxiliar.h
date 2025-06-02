#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h"

#define ARQ_CONSULTAS "data/registro_consultas.csv"
#define ARQ_PACIENTES "data/registro_pacientes.csv"
#define ARQ_MEDICOS "data/registro_medicos.csv"

int validar_cpf(char *cpf, size_t tamanho);
void limpar_buffer();
void medico_to_array(const reg_medico *m, char *valores[4], char crm[]);
void paciente_to_array(const reg_paciente *p, char *valores[3]);
void consulta_to_array(const reg_consulta *c, char *valores[4], char id_paciente[], char id_medico[], char data_hora[], char status[]);
int get_id(const char *nome_arquivo, int select_col, const char *valor_busca);
void listar_medicos(const char *nome_arquivo);
int get_maior_id(const char *nome_arquivo);
int buscar_linha(const char *nome_arquivo, int indice_coluna, const char *valor_busca);
int contem_valor(const char *nome_arquivo, int indice_coluna, const char *valor_busca);
void buscar_nome_medico_por_id(int id_medico, char *nome, size_t tamanho);
void buscar_nome_paciente_por_id(int id_paciente, char *nome, size_t tamanho);
int validar_dia(const char *data_hora);
int get_medico_especial(int id_medico, const char *especialidade);
int ler_opcao_menu(int min, int max);
void atualizar_paciente(const char *file_paciente, char *cpf_paciente, char *valores[4]);
int validar_paciente();
int validar_medico();
int validar_mais_op();
void carregar_medicos_do_arquivo(void);
const char *especialidade_string(Especialidade especialidade);
int validar_crm(char *crm, size_t tamanho);
int validar_data(const char *data);
int validar_mes(int mes);
int validar_dia2(int dia, int mes, int ano);
void exibir_lista_medicos();
int buscar_especialidade_medico_por_id(int id_medico, char *especialidade, size_t tam);
int validar_telefone_padrao(char *telefone, size_t tamanho);
int validar_nome_padrao(char *nome, size_t tamanho);
void buscar_nome_por_id(const char *arquivo, int id, char *nome, size_t tamanho);
reg_paciente *buscar_paciente_por_cpf(const char *cpf);
int cpf_valido(const char *cpf);
void carregar_pacientes_do_arquivo();
void obter_data_atual(char *data_atual);
int buscar_especialidade_medico_por_id(int id_medico_busca, char *especialidade_medico, size_t tam);
#endif