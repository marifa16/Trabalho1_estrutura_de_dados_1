#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include "../include/estruturas.h"
#include "../include/auxiliar.h"
#include <locale.h>

// Cria um arquivo CSV com o cabe�alho adequado se ele n�o existir
void criar_arquivo_csv(const char *nome_arquivo)
{
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Configura o locale para portugu�s do Brasil com suporte a UTF-8

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

// Inicializa todos os arquivos CSV necess�rios
void inicializar_arquivos_csv()
{
    criar_arquivo_csv("data/registro_consultas.csv");
    criar_arquivo_csv("data/registro_pacientes.csv");
    criar_arquivo_csv("data/registro_medicos.csv");
    printf("\nInicialização Concluida\n");
}

// Adiciona uma nova linha ao CSV, gerando um novo id �nico
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
    for (int i = 0; i < select_col - 1; i++) // -1 pois j� escrevemos o id
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
    fgets(linha, sizeof(linha), arquivo); // Cabe�alho

    while (fgets(linha, sizeof(linha), arquivo))
    {
        if (tipo == 1)
        {
            int id_lido;
            char nome[100], crm[16], especialidade[100], telefone[16];
            if (sscanf(linha, "%d,%99[^,],%15[^,],%99[^,],%15[^,\n]", &id_lido, nome, crm, especialidade, telefone) == 5)
            {
                printf("CRM          : %s\n", crm);
                printf("Telefone     : %s\n", telefone);
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
                    printf("ID M?dico   : %d\n", id_med);
                    printf("Data/Hora   : %s\n", data_hora);
                    printf("Status      : %s\n", status);
                    printf("========================================\n");
                    fclose(arquivo);
                    return;
                }
            }
        }
    }
    printf("Registro n�o encontrado.\n");
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
        printf("Erro ao criar arquivo tempor�rio.\n");
        return 0;
    }

    char linha[512];
    int linha_atual = -1; // garante que a contagem das linhas de dados comece em 0, ignorando o cabe�alho.
    // atualizar ou deletar a "linha 0" (primeira linha de dados), ela ser� corretamente identificada.
    int atualizou = 0; // a fun��o retorna atualizou para indicar sucesso (1) ou falha (0).

    // Copia o cabe�alho
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

    // Substitui o arquivo original pelo tempor�rio
    remove(nome_arquivo);
    rename("temp.csv", nome_arquivo);

    return atualizou;
}

// Remove a linha 'row' (come�ando em 0, sem contar o cabe�alho) do arquivo CSV
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
        printf("Erro ao criar arquivo tempor�rio.\n");
        return 0;
    }

    char linha[512];
    int linha_atual = -1;
    int deletou = 0;

    // Copia o cabe�alho
    if (fgets(linha, sizeof(linha), arquivo))
    {
        fputs(linha, temp);
    }

    // Copia as linhas, pulando a que ser� deletada
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

    // Cabe�alhos padronizados
    if (tipo == 1) // M�dicos
    {
        printf("+-----+----------------------------------------------------------------------------------------------------+---------+----------------------------------------------------------------------------------------------------+-------------+\n");
        printf("| ID  | Nome                                                                                               | CRM     | Especialidade                                                                                      | Telefone    |\n");
        printf("+-----+----------------------------------------------------------------------------------------------------+---------+----------------------------------------------------------------------------------------------------+-------------+\n");
    }
    else if (tipo == 2) // Pacientes
    {
        printf("+-----+----------------------------------------------------------------------------------------------------------------------------+-------------+-------------+\n");
        printf("| ID  | Nome                                                                                                                       | CPF         | Telefone    |\n");
        printf("+-----+----------------------------------------------------------------------------------------------------------------------------+-------------+-------------+\n");
    }
    else if (tipo == 3) // Consultas
    {
        printf("+-----+----------------------------------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+---------------------+------------+\n");
        printf("| ID  | Paciente                                                                                                                   | Medico                                                                                             | Data/Hora           | Status     |\n");
        printf("+-----+----------------------------------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+---------------------+------------+\n");
    }

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabe�alho

    while (fgets(linha, sizeof(linha), arquivo))
    {
        int exibir = 1;
        if (modo)
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
            if (valor && strcmp(modo, "paciente") == 0 && strcmp(tokens[1], valor) == 0)
                exibir = 1;
            else if (valor && strcmp(modo, "medico") == 0 && strcmp(tokens[2], valor) == 0)
                exibir = 1;
            else if (valor && strcmp(modo, "especialidade") == 0 && strstr(tokens[4], valor))
                exibir = 1;
            else if (strcmp(modo, "dia") == 0 && validar_dia(tokens[3]))
                exibir = 1;
        }

        if (exibir)
        {
            if (tipo == 1)
            {
                int id;
                char nome[100], crm[16], especialidade[100], telefone[16];
                if (sscanf(linha, "%d,%99[^,],%15[^,],%99[^,],%15[^,\n]", &id, nome, crm, especialidade, telefone) == 5)
                    printf("| %-3d | %-24s | %-7s | %-24s | %-11s |\n", id, nome, crm, especialidade, telefone);
            }
            else if (tipo == 2)
            {
                int id;
                char nome[120], cpf[12], telefone[12];
                if (sscanf(linha, "%d,%119[^,],%11[^,],%11[^\n]", &id, nome, cpf, telefone) == 4)
                    printf("| %-4d | %-120s | %-11s | %-11s |\n", id, nome, cpf, telefone);
            }
            else if (tipo == 3)
            {
                int id, id_pac, id_med;
                char data_hora[32], status[16];
                if (sscanf(linha, "%d,%d,%d,%31[^,],%15[^\n]", &id, &id_pac, &id_med, data_hora, status) == 5)
                {
                    char nome_paciente[122] = ""; // 120 + margem
                    char nome_medico[102] = "";   // 100 + margem
                    buscar_nome_paciente_por_id(id_pac, nome_paciente, sizeof(nome_paciente));
                    buscar_nome_medico_por_id(id_med, nome_medico, sizeof(nome_medico));
                    printf("| %-4d | %-120s | %-100s | %-19s | %-10s |\n",
                           id, nome_paciente, nome_medico, data_hora, status);
                }
            }
        }
    }

    // Linha final
    if (tipo == 1)
        printf("+-----+----------------------------------------------------------------------------------------------------+---------+----------------------------------------------------------------------------------------------------+-------------+\n");
    else if (tipo == 2)
        printf("+-----+----------------------------------------------------------------------------------------------------------------------------+-------------+-------------+\n");
    else if (tipo == 3)
        printf("+-----+----------------------------------------------------------------------------------------------------------------------------+----------------------------------------------------------------------------------------------------+---------------------+------------+\n");

    fclose(arquivo);
}

void relatorio_contagem_consultas_por_especialidade()
{
    const char *especialidades[] = {
        "CLINICO_GERAL",
        "PEDIATRA",
        "CARDIOLOGISTA",
        "DERMATOLOGISTA",
        "PSIQUIATRA"};
    int contagem[5] = {0, 0, 0, 0, 0};

    // Percorre todas as consultas
    FILE *arq_cons = fopen("data/registro_consultas.csv", "r");
    if (!arq_cons)
    {
        printf("Erro ao abrir o arquivo de consultas.\n");
        return;
    }
    char linha[512];
    fgets(linha, sizeof(linha), arq_cons); // pula cabe�alho

    while (fgets(linha, sizeof(linha), arq_cons))
    {
        int id_consulta, id_paciente, id_medico;
        char data_hora[32], status[16];
        if (sscanf(linha, "%d,%d,%d,%31[^,],%15[^\n]", &id_consulta, &id_paciente, &id_medico, data_hora, status) == 5)
        {
            // Descobre a especialidade do m�dico
            FILE *arq_med = fopen("data/registro_medicos.csv", "r");
            if (!arq_med)
                continue;
            char linha_med[512];
            fgets(linha_med, sizeof(linha_med), arq_med); // pula cabe�alho
            while (fgets(linha_med, sizeof(linha_med), arq_med))
            {
                int id_med;
                char nome[100], crm[16], especialidade[100], telefone[16];
                if (sscanf(linha_med, "%d,%99[^,],%15[^,],%99[^,],%15[^\n]", &id_med, nome, crm, especialidade, telefone) == 5)
                {
                    if (id_med == id_medico)
                    {
                        // Conta para a especialidade correta
                        for (int i = 0; i < 5; i++)
                        {
                            if (strcasecmp(especialidade, especialidades[i]) == 0)
                            {
                                contagem[i]++;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            fclose(arq_med);
        }
    }
    fclose(arq_cons);

    // Exibe o relat�rio
    printf("\nRelat�rio de Consultas por Especialidade\n");
    printf("+-------------------+---------------------+\n");
    printf("| Especialidade     | Quantidade de Consultas |\n");
    printf("+-------------------+---------------------+\n");
    for (int i = 0; i < 5; i++)
    {
        printf("| %-17s | %-20d |\n", especialidades[i], contagem[i]);
    }
    printf("+-------------------+---------------------+\n");
}