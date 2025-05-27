#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include "../include/estruturas.h"
#include "../include/auxiliar.h"
#include <locale.h>

// Cria um arquivo CSV com o cabeçalho adequado se ele não existir
void criar_arquivo_csv(const char *nome_arquivo)
{
    struct stat buffer;

    int arquivo_status = (stat(nome_arquivo, &buffer) == 0);

    if (!arquivo_status)
    {
        FILE *arquivo = fopen(nome_arquivo, "w");
        if (arquivo == NULL)
        {
            printf("Erro ao criar o arquivo '%s'.\n", nome_arquivo);
            return;
        }

        if (strstr(nome_arquivo, "registro_consultas.csv") != NULL)
        {
            fprintf(arquivo, "id_consulta,id_paciente,id_medico,data_hora,status\n");
        }
        else if (strstr(nome_arquivo, "registro_pacientes.csv") != NULL)
        {
            fprintf(arquivo, "id_paciente,nome,cpf,telefone\n");
        }
        else if (strstr(nome_arquivo, "registro_medicos.csv") != NULL)
        {
            fprintf(arquivo, "id_medico,nome,crm,especialidade,telefone\n");
        }

        fclose(arquivo);
    }
}

// Inicializa todos os arquivos CSV necessários
void inicializar_arquivos_csv()
{
    criar_arquivo_csv("data/registro_consultas.csv");
    criar_arquivo_csv("data/registro_pacientes.csv");
    criar_arquivo_csv("data/registro_medicos.csv");
    printf("\nInicialização concluída\n");
}

// Adiciona uma nova linha ao CSV, gerando um novo id único
int add_row(const char *nome_arquivo, int select_col, char *valores[])
{
    int novo_id = get_maior_id(nome_arquivo) + 1; // Busca o maior id e incrementa

    FILE *arquivo = fopen(nome_arquivo, "a"); // Abre o arquivo para adicionar no final
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo '%s' para escrita.\n", nome_arquivo);
        return -1;
    }

    fprintf(arquivo, "%d", novo_id);         // Escreve o novo id
    for (int i = 0; i < select_col - 1; i++) // -1 pois já escrevemos o id
    {                                        // Escreve os demais campos
        fprintf(arquivo, ",%s", valores[i]);
    }
    fprintf(arquivo, "\n"); // Nova linha

    fclose(arquivo); // Fecha o arquivo
    return novo_id;  // Retorna o id gerado
}

void read_row(const char *nome_arquivo, int id)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo '%s'.\n", nome_arquivo);
        return;
    }

    // Detecta o tipo de arquivo para ajustar o layout
    int tipo = 0; // 1=medico, 2=paciente, 3=consulta
    if (strstr(nome_arquivo, "medico"))
        tipo = 1;
    else if (strstr(nome_arquivo, "paciente"))
        tipo = 2;
    else if (strstr(nome_arquivo, "consulta"))
        tipo = 3;

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Cabeçalho

    while (fgets(linha, sizeof(linha), arquivo))
    {
        if (tipo == 1)
        {
            int id_lido, crm;
            char nome[100], especialidade[100], telefone[16];
            if (sscanf(linha, "%d,%99[^,],%d,%99[^,],%15[^,\n]", &id_lido, nome, &crm, especialidade, telefone) == 5)
            {
                if (id_lido == id)
                {
                    printf("========================================\n");
                    printf("ID           : %d\n", id_lido);
                    printf("Nome         : %s\n", nome);
                    printf("CRM          : %d\n", crm);
                    printf("Especialidade: %s\n", especialidade);
                    printf("Telefone     : %s\n", telefone);
                    printf("========================================\n");
                    fclose(arquivo);
                    return;
                }
            }
        }
        else if (tipo == 2)
        {
            int id_lido;
            char nome[120], cpf[12], telefone[12];
            if (sscanf(linha, "%d,%119[^,],%11[^,],%11[^\n]", &id_lido, nome, cpf, telefone) == 4)
            {
                if (id_lido == id)
                {
                    printf("========================================\n");
                    printf("ID      : %d\n", id_lido);
                    printf("Nome    : %s\n", nome);
                    printf("CPF     : %s\n", cpf);
                    printf("Telefone: %s\n", telefone);
                    printf("========================================\n");
                    fclose(arquivo);
                    return;
                }
            }
        }
        else if (tipo == 3)
        {
            int id_lido, id_pac, id_med;
            char data_hora[32], status[16];
            if (sscanf(linha, "%d,%d,%d,%31[^,],%15[^\n]", &id_lido, &id_pac, &id_med, data_hora, status) == 5)
            {
                if (id_lido == id)
                {
                    printf("========================================\n");
                    printf("ID Consulta : %d\n", id_lido);
                    printf("ID Paciente : %d\n", id_pac);
                    printf("ID Médico   : %d\n", id_med);
                    printf("Data/Hora   : %s\n", data_hora);
                    printf("Status      : %s\n", status);
                    printf("========================================\n");
                    fclose(arquivo);
                    return;
                }
            }
        }
    }
    printf("Registro não encontrado.\n");
    fclose(arquivo);
}

// Atualiza a linha 'row' do arquivo com os novos valores (incluindo o id)
int att_row(const char *nome_arquivo, int row, int num_colunas, char *valores[])
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo '%s'.\n", nome_arquivo);
        return 0;
    }

    FILE *temp = fopen("temp.csv", "w");
    if (!temp)
    {
        fclose(arquivo);
        printf("Erro ao criar arquivo temporário.\n");
        return 0;
    }

    char linha[512];
    int linha_atual = -1; // garante que a contagem das linhas de dados comece em 0, ignorando o cabeçalho.
    // atualizar ou deletar a "linha 0" (primeira linha de dados), ela será corretamente identificada.
    int atualizou = 0; // a função retorna atualizou para indicar sucesso (1) ou falha (0).

    // Copia o cabeçalho
    if (fgets(linha, sizeof(linha), arquivo))
    {
        fputs(linha, temp);
    }

    // Copia ou atualiza as linhas
    while (fgets(linha, sizeof(linha), arquivo)) // percorrendo o arquivo e preencher em temp
    {
        linha_atual++;
        if (linha_atual == row)
        {
            // Atualiza a linha
            fprintf(temp, "%s", valores[0]);
            for (int i = 1; i < num_colunas; i++)
            {
                fprintf(temp, ",%s", valores[i]);
            }
            fprintf(temp, "\n");
            atualizou = 1;
        }
        else
        {
            fputs(linha, temp);
        }
    }

    fclose(arquivo);
    fclose(temp);

    // Substitui o arquivo original pelo temporário
    remove(nome_arquivo);
    rename("temp.csv", nome_arquivo);

    return atualizou;
}

// Remove a linha 'row' (começando em 0, sem contar o cabeçalho) do arquivo CSV
int del_row(const char *nome_arquivo, int row)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo '%s'.\n", nome_arquivo);
        return 0;
    }

    FILE *temp = fopen("temp.csv", "w");
    if (!temp)
    {
        fclose(arquivo);
        printf("Erro ao criar arquivo temporário.\n");
        return 0;
    }

    char linha[512];
    int linha_atual = -1;
    int deletou = 0;

    // Copia o cabeçalho
    if (fgets(linha, sizeof(linha), arquivo))
    {
        fputs(linha, temp);
    }

    // Copia as linhas, pulando a que será deletada
    while (fgets(linha, sizeof(linha), arquivo))
    {
        linha_atual++;
        if (linha_atual == row)
        {
            deletou = 1;
            continue;
        }
        fputs(linha, temp);
    }

    fclose(arquivo);
    fclose(temp);

    remove(nome_arquivo);
    rename("temp.csv", nome_arquivo);

    return deletou;
}

void exibir_arquivo(const char *nome_arquivo, const char *modo, const char *valor)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo '%s'.\n", nome_arquivo);
        return;
    }

    int tipo = 0;
    if (strstr(nome_arquivo, "medico"))
        tipo = 1;
    else if (strstr(nome_arquivo, "paciente"))
        tipo = 2;
    else if (strstr(nome_arquivo, "consulta"))
        tipo = 3;

    // Cabeçalhos padronizados
    if (tipo == 1)
    {
        printf("+-----+--------------------------+---------+--------------------------+-------------+\n");
        printf("| ID  | Nome                     | CRM     | Especialidade            | Telefone    |\n");
        printf("+-----+--------------------------+---------+--------------------------+-------------+\n");
    }
    else if (tipo == 2)
    {
        printf("+-----+--------------------------+-------------+-------------+\n");
        printf("| ID  | Nome                     | CPF         | Telefone    |\n");
        printf("+-----+--------------------------+-------------+-------------+\n");
    }
    else if (tipo == 3)
    {
        printf("+-----+-----+-----+---------------------+------------+\n");
        printf("| ID  | Pac | Med | Data/Hora           | Status     |\n");
        printf("+-----+-----+-----+---------------------+------------+\n");
    }

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    while (fgets(linha, sizeof(linha), arquivo))
    {
        int exibir = 1;
        if (modo && valor)
        {
            char linha_copia[512];
            strcpy(linha_copia, linha);
            char *tokens[10];
            int i = 0;
            char *token = strtok(linha_copia, ",\n");
            while (token && i < 10)
            {
                tokens[i++] = token;
                token = strtok(NULL, ",\n");
            }
            exibir = 0;
            if (strcmp(modo, "paciente") == 0 && strcmp(tokens[1], valor) == 0)
                exibir = 1;
            else if (strcmp(modo, "medico") == 0 && strcmp(tokens[2], valor) == 0)
                exibir = 1;
            else if (strcmp(modo, "especialidade") == 0 && strstr(tokens[4], valor))
                exibir = 1;
            else if (strcmp(modo, "dia") == 0)
            {
                int dia, mes, ano;
                sscanf(tokens[3], "%d/%d/%d", &dia, &mes, &ano);
                if (dia == tm.tm_mday && mes == tm.tm_mon + 1 && ano == tm.tm_year + 1900)
                    exibir = 1;
            }
        }
        if (exibir)
        {
            if (tipo == 1)
            {
                int id, crm;
                char nome[100], especialidade[100], telefone[16];
                if (sscanf(linha, "%d,%99[^,],%d,%99[^,],%15[^,\n]", &id, nome, &crm, especialidade, telefone) == 5)
                    printf("| %-3d | %-24s | %-7d | %-24s | %-11s |\n", id, nome, crm, especialidade, telefone);
            }
            else if (tipo == 2)
            {
                int id;
                char nome[120], cpf[12], telefone[12];
                if (sscanf(linha, "%d,%119[^,],%11[^,],%11[^\n]", &id, nome, cpf, telefone) == 4)
                    printf("| %-3d | %-24s | %-11s | %-11s |\n", id, nome, cpf, telefone);
            }
            else if (tipo == 3)
            {
                int id, id_pac, id_med;
                char data_hora[32], status[16];
                if (sscanf(linha, "%d,%d,%d,%31[^,],%15[^\n]", &id, &id_pac, &id_med, data_hora, status) == 5)
                    printf("| %-3d | %-3d | %-3d | %-19s | %-10s |\n", id, id_pac, id_med, data_hora, status);
            }
        }
    }

    // Linha final
    if (tipo == 1)
        printf("+-----+--------------------------+---------+--------------------------+-------------+\n");
    else if (tipo == 2)
        printf("+-----+--------------------------+-------------+-------------+\n");
    else if (tipo == 3)
        printf("+-----+-----+-----+---------------------+------------+\n");

    fclose(arquivo);
}