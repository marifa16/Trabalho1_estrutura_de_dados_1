#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validar_nome_paciente(char *nome_paciente, size_t tamanho);

int validar_opcao_usuario();

int validar_cpf(char *cpf, size_t tamanho);

int validar_telefone(char *telefone, size_t tamanho);

void limpar_buffer();
#endif