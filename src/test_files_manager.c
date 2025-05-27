/*#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include "../include/estruturas.h"
#include "../include/auxiliar.h"
#include <locale.h>
 temporario
// Converte reg_medico para array de strings (buffers externos para campos num?ricos)
void medico_to_array(const reg_medico *m, char *valores[4], char crm[], char telefone[])
{
    snprintf(crm, 16, "%d", m->crm); // Converte int para string
    valores[1] = crm;
    valores[2] = (char *)m->especialidade;     // Especialidade j? ? string
    snprintf(telefone, 16, "%d", m->telefone); // Converte int para string
    valores[3] = telefone;
}

// Converte reg_paciente para array de strings (todos j? s?o strings)
void paciente_to_array(const reg_paciente *p, char *valores[3])
{
    valores[0] = (char *)p->nome;
    valores[1] = (char *)p->cpf;
    valores[2] = (char *)p->telefone;
}

// Converte reg_consulta para array de strings (buffers externos para campos num?ricos e data)
void consulta_to_array(const reg_consulta *c, char *valores[4], char id_paciente[], char id_medico[], char data_hora[], char status[])
{
    snprintf(id_paciente, 16, "%d", c->id_paciente);
    snprintf(id_medico, 16, "%d", c->id_medico);
    // Formata data/hora como string
    snprintf(data_hora, 32, "%02d/%02d/%04d %02d:%02d",
             c->data_hora.tm_mday, c->data_hora.tm_mon + 1, c->data_hora.tm_year + 1900,
             c->data_hora.tm_hour, c->data_hora.tm_min);

    // Converte status_consulta para string
    switch (c->status)
    {
    case AGENDADA:
        strcpy(status, "AGENDADA");
        break;
    case REALIZADA:
        strcpy(status, "REALIZADA");
        break;
    case CANCELADA:
        strcpy(status, "CANCELADA");
        break;
    default:
        strcpy(status, "DESCONHECIDO");
    }

    valores[0] = id_paciente;
    valores[1] = id_medico;
    valores[2] = data_hora;
    valores[3] = status;
}

// Retorna o id da primeira coluna da linha que cont?m o valor buscado na coluna select_col
int get_id(const char *nome_arquivo, int select_col, const char *valor_busca)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
        return -1;

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabe?alho

    while (fgets(linha, sizeof(linha), arquivo))
    {
        char *token;
        char linha_copia[512];
        strcpy(linha_copia, linha);

        token = strtok(linha_copia, ",\n");
        int coluna = 0;
        int id = -1;
        while (token)
        {
            if (coluna == 0)
                id = atoi(token); // id sempre na primeira coluna
            if (coluna == select_col && strcmp(token, valor_busca) == 0)
            {
                fclose(arquivo);
                return id;
            }
            token = strtok(NULL, ",\n");
            coluna++;
        }
    }
    fclose(arquivo);
    return -1; // N?o encontrado
}

void listar_medicos(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo '%s'.\n", nome_arquivo);
        return;
    }

    char linha[512];
    int primeira_linha = 1;
    while (fgets(linha, sizeof(linha), arquivo))
    {
        if (primeira_linha)
        {
            printf("%s", linha); // Cabe?alho
            primeira_linha = 0;
            continue;
        }
        printf("%s", linha);
    }
    fclose(arquivo);
}

// Retorna o maior id da primeira coluna do arquivo CSV
int get_maior_id(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
        return 0;

    char linha[512];
    int maior_id = 0;
    fgets(linha, sizeof(linha), arquivo); // l? a primeira linha e fica por isso mesmo

    while (fgets(linha, sizeof(linha), arquivo)) // l? as demais linhas
    {
        int id_temp = 0;

        sscanf(linha, "%d,", &id_temp);

        // verificar se o id_temp ? o maior
        if (id_temp > maior_id)
            maior_id = id_temp;
    }

    fclose(arquivo); // fecha o arquivo

    return maior_id;
}

// Retorna o n?mero da linha (come?ando em 0, sem contar o cabe?alho) onde encontrou o valor na coluna indicada, ou -1 se n?o encontrou
int buscar_linha(const char *nome_arquivo, int indice_coluna, const char *valor_busca)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
        return -1;

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabe?alho

    int linha_atual = 0;
    while (fgets(linha, sizeof(linha), arquivo))
    {
        char linha_copia[512];
        strcpy(linha_copia, linha);

        int coluna_atual = 0;
        char *token = strtok(linha_copia, ",\n");
        while (token != NULL)
        {
            if (coluna_atual == indice_coluna)
            {
                if (strcmp(token, valor_busca) == 0)
                {
                    fclose(arquivo);
                    return linha_atual;
                }
                break;
            }
            token = strtok(NULL, ",\n");
            coluna_atual++;
        }
        linha_atual++;
    }

    fclose(arquivo);
    return -1;
}

// Retorna 1 se encontrou o valor na coluna indicada, 0 se n?o encontrou
int contem_valor(const char *nome_arquivo, int indice_coluna, const char *valor_busca)
{
    return buscar_linha(nome_arquivo, indice_coluna, valor_busca) >= 0 ? 1 : 0;
}
// temporario


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
    criar_arquivo_csv("../data/registro_consultas.csv");
    criar_arquivo_csv("../data/registro_pacientes.csv");
    criar_arquivo_csv("../data/registro_medicos.csv");
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


void teste(const char *opc)
{
    // Garante que o arquivo existe e tem cabeçalho
    inicializar_arquivos_csv();

    char *arquivo;
    int num_colunas;
    char *cabecalho;
    if (strcmp(opc, "registro_medicos") == 0)
    {
        arquivo = "../data/registro_medicos.csv";
        num_colunas = 5;
        cabecalho = "id_medico,nome,crm,especialidade,telefone";

        printf("\n==== TESTANDO: %s ====\n", opc);

        // 1. Adiciona 3 entradas e guarda os IDs
        printf("Adicionando 3 entradas...\n");
        int ids[3];
        for (int i = 0; i < 3; i++)
        {
            char *valores[4];
            char buf1[100], buf2[16], buf3[100], buf4[16];
            snprintf(buf1, sizeof(buf1), "Medico%d", i + 1);        // nome
            snprintf(buf2, sizeof(buf2), "%d", 100000 + (i + 1));     // crm
            snprintf(buf3, sizeof(buf3), "Especialidade%d", i + 1); // especialidade
            snprintf(buf4, sizeof(buf4), "1199999000%d", i + 1);    // telefone
            valores[0] = buf1;
            valores[1] = buf2;
            valores[2] = buf3;
            valores[3] = buf4;
            ids[i] = add_row(arquivo, num_colunas, valores);
        }

        printf("\nExibindo todas as entradas do arquivo:\n");
        exibir_arquivo(arquivo, NULL, NULL);

        // 2. Exibe uma entrada específica (id 2)
        printf("\nExibindo entrada de id %d:\n", ids[1]);
        read_row(arquivo, ids[1]);

        printf("\nExibindo todas as entradas do arquivo:\n");
        exibir_arquivo(arquivo, NULL, NULL);

        // 3. Atualiza a entrada de id 2
        printf("\nAtualizando entrada de id %d...\n", ids[1]);
        char id_str[16];
        snprintf(id_str, sizeof(id_str), "%d", ids[1]);
        int linha = buscar_linha(arquivo, 0, id_str);
        if (linha >= 0)
        {
            char *novos_valores[5];
            char buf0[16], buf1[100], buf2[16], buf3[100], buf4[16];
            snprintf(buf0, sizeof(buf0), "%d", ids[1]);
            snprintf(buf1, sizeof(buf1), "MedicoAtualizado");
            snprintf(buf2, sizeof(buf2), "999900");
            snprintf(buf3, sizeof(buf3), "EspecialidadeAtualizada");
            snprintf(buf4, sizeof(buf4), "11999999999");
            novos_valores[0] = buf0; // id_medico
            novos_valores[1] = buf1; // nome
            novos_valores[2] = buf2; // crm
            novos_valores[3] = buf3; // especialidade
            novos_valores[4] = buf4; // telefone
            att_row(arquivo, linha, num_colunas, novos_valores);
        }

        printf("\nExibindo todas as entradas do arquivo:\n");
        exibir_arquivo(arquivo, NULL, NULL);

        // 4. Deleta a entrada de id 1
        printf("\nDeletando entrada de id %d...\n", ids[0]);
        snprintf(id_str, sizeof(id_str), "%d", ids[0]);
        linha = buscar_linha(arquivo, 0, id_str);
        if (linha >= 0)
        {
            del_row(arquivo, linha);
        }

        printf("\nExibindo todas as entradas do arquivo:\n");
        exibir_arquivo(arquivo, NULL, NULL);

        printf("==== FIM DO TESTE: %s ====\n\n", opc);
    }
    else if (strcmp(opc, "registro_pacientes") == 0)
    {
        arquivo = "../data/registro_pacientes.csv";
        num_colunas = 4;
        cabecalho = "id_paciente,nome,cpf,telefone";

        printf("\n==== TESTANDO: %s ====\n", opc);

        // 1. Adiciona 3 entradas e guarda os IDs
        printf("Adicionando 3 entradas...\n");
        int ids[3];
        for (int i = 0; i < 3; i++)
        {
            char *valores[3];
            char buf1[100], buf2[16], buf3[16];
            snprintf(buf1, sizeof(buf1), "Paciente%d", i + 1);   // nome
            snprintf(buf2, sizeof(buf2), "00000000%03d", i + 1);      // cpf
            snprintf(buf3, sizeof(buf3), "1199999000%d", i + 1); // telefone
            valores[0] = buf1;
            valores[1] = buf2;
            valores[2] = buf3;
            ids[i] = add_row(arquivo, num_colunas, valores);
        }

        printf("\nExibindo todas as entradas do arquivo:\n");
        exibir_arquivo(arquivo, NULL, NULL);

        // 2. Exibe uma entrada específica (id 2)
        printf("\nExibindo entrada de id %d:\n", ids[1]);
        read_row(arquivo, ids[1]);

        printf("\nExibindo todas as entradas do arquivo:\n");
        exibir_arquivo(arquivo, NULL, NULL);

        // 3. Atualiza a entrada de id 2
        printf("\nAtualizando entrada de id %d...\n", ids[1]);
        char id_str[16];
        snprintf(id_str, sizeof(id_str), "%d", ids[1]);
        int linha = buscar_linha(arquivo, 0, id_str);
        if (linha >= 0)
        {
            char *novos_valores[4];
            char buf0[16], buf1[100], buf2[16], buf3[16];
            snprintf(buf0, sizeof(buf0), "%d", ids[1]);
            snprintf(buf1, sizeof(buf1), "PacienteAtualizado");
            snprintf(buf2, sizeof(buf2), "99999999900");
            snprintf(buf3, sizeof(buf3), "11999999999");
            novos_valores[0] = buf0; // id_paciente
            novos_valores[1] = buf1; // nome
            novos_valores[2] = buf2; // cpf
            novos_valores[3] = buf3; // telefone
            att_row(arquivo, linha, num_colunas, novos_valores);
        }

        printf("\nExibindo todas as entradas do arquivo:\n");
        exibir_arquivo(arquivo, NULL, NULL);

        // 4. Deleta a entrada de id 1
        printf("\nDeletando entrada de id %d...\n", ids[0]);
        snprintf(id_str, sizeof(id_str), "%d", ids[0]);
        linha = buscar_linha(arquivo, 0, id_str);
        if (linha >= 0)
        {
            del_row(arquivo, linha);
        }

        printf("\nExibindo todas as entradas do arquivo:\n");
        exibir_arquivo(arquivo, NULL, NULL);

        printf("==== FIM DO TESTE: %s ====\n\n", opc);
    }
    else if (strcmp(opc, "registro_consultas") == 0)
    {
        arquivo = "../data/registro_consultas.csv";
        num_colunas = 5;
        cabecalho = "id_consulta,id_paciente,id_medico,data_hora,status";

        printf("\n==== TESTANDO: %s ====\n", opc);

        // 1. Adiciona 3 entradas e guarda os IDs
        printf("Adicionando 3 entradas...\n");
        int ids[3];
        for (int i = 0; i < 3; i++)
        {
            char *valores[4];
            char buf1[16], buf2[16], buf3[32], buf4[16];
            snprintf(buf1, sizeof(buf1), "%d", 100 + i);          // id_paciente
            snprintf(buf2, sizeof(buf2), "%d", 200 + i);          // id_medico
            snprintf(buf3, sizeof(buf3), "27/05/2025 1%d:00", i); // data_hora
            snprintf(buf4, sizeof(buf4), "AGENDADA");             // status
            valores[0] = buf1;
            valores[1] = buf2;
            valores[2] = buf3;
            valores[3] = buf4;
            ids[i] = add_row(arquivo, num_colunas, valores);
        }

        printf("\nExibindo todas as entradas do arquivo:\n");
        exibir_arquivo(arquivo, NULL, NULL);

        // 2. Exibe uma entrada específica (id 2)
        printf("\nExibindo entrada de id %d:\n", ids[1]);
        read_row(arquivo, ids[1]);

        printf("\nExibindo todas as entradas do arquivo:\n");
        exibir_arquivo(arquivo, NULL, NULL);

        // 3. Atualiza a entrada de id 2
        printf("\nAtualizando entrada de id %d...\n", ids[1]);
        char id_str[16];
        snprintf(id_str, sizeof(id_str), "%d", ids[1]);
        int linha = buscar_linha(arquivo, 0, id_str);
        if (linha >= 0)
        {
            char *novos_valores[5];
            char buf0[16], buf1[16], buf2[16], buf3[32], buf4[16];
            snprintf(buf0, sizeof(buf0), "%d", ids[1]);
            snprintf(buf1, sizeof(buf1), "999");              // novo id_paciente
            snprintf(buf2, sizeof(buf2), "888");              // novo id_medico
            snprintf(buf3, sizeof(buf3), "28/05/2025 10:30"); // nova data_hora
            snprintf(buf4, sizeof(buf4), "REALIZADA");        // novo status
            novos_valores[0] = buf0;                          // id_consulta
            novos_valores[1] = buf1;                          // id_paciente
            novos_valores[2] = buf2;                          // id_medico
            novos_valores[3] = buf3;                          // data_hora
            novos_valores[4] = buf4;                          // status
            att_row(arquivo, linha, num_colunas, novos_valores);
        }

        printf("\nExibindo todas as entradas do arquivo:\n");
        exibir_arquivo(arquivo, NULL, NULL);

        // 4. Deleta a entrada de id 1
        printf("\nDeletando entrada de id %d...\n", ids[0]);
        snprintf(id_str, sizeof(id_str), "%d", ids[0]);
        linha = buscar_linha(arquivo, 0, id_str);
        if (linha >= 0)
        {
            del_row(arquivo, linha);
        }

        printf("\nExibindo todas as entradas do arquivo:\n");
        exibir_arquivo(arquivo, NULL, NULL);

        printf("==== FIM DO TESTE: %s ====\n\n", opc);
    }
}
#include <direct.h>

int main()
{
    setlocale(LC_ALL, "Portuguese");

    //teste("registro_medicos");
    //teste("registro_pacientes");
    teste("registro_consultas");


    return 0;
}
*/