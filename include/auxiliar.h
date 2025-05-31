#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validar_nome_paciente(char *nome_paciente, size_t tamanho);

int validar_cpf(char *cpf, size_t tamanho);

int validar_telefone(char *telefone, size_t tamanho);

void limpar_buffer();

void medico_to_array(const reg_medico *m, char *valores[4], char crm[], char telefone[]);

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

#endif