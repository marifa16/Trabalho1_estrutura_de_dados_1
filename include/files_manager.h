#ifndef FILES_MANAGER_H
#define FILES_MANAGER_H

void criar_pasta(const char *nome_pasta);

void criar_arquivo_csv(const char *nome_arquivo);

// Função para inicializar os arquivos CSV
void inicializar_arquivos_csv();

int add_row(const char *nome_arquivo, int select_col, char *valores[]);

void read_row(const char *nome_arquivo, int id);

int att_row(const char *nome_arquivo, int row, int num_colunas, char *valores[]);

int del_row(const char *nome_arquivo, int row);

int exibir_arquivo(const char *nome_arquivo, const char *modo, const char *valor);

void relatorio_contagem_consultas_por_especialidade();

#endif