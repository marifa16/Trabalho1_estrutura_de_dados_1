#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include "../include/estruturas.h"
#include "../include/auxiliar.h"
#include "../include/mensagens.h" 

// Cria um arquivo CSV com o cabecalho adequado se ele nao existir
void criar_arquivo_csv(const char *nome_arquivo)
{
    setlocale(LC_ALL, "pt_BR.UTF-8"); // Configura o locale para portugues do Brasil com suporte a UTF-8

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

// Inicializa todos os arquivos CSV necessarios
void inicializar_arquivos_csv()
{
    criar_arquivo_csv("data/registro_consultas.csv");
    criar_arquivo_csv("data/registro_pacientes.csv");
    criar_arquivo_csv("data/registro_medicos.csv");
    printf("\nInicialização Concluida\n");
}

// Adiciona uma nova linha ao CSV, gerando um novo id unico
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
    for (int i = 0; i < select_col - 1; i++) // -1 pois ja escrevemos o id
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
    fgets(linha, sizeof(linha), arquivo);

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
    int linha_atual = -1; // garante que a contagem das linhas de dados comece em 0, ignorando o cabecalho.
    // atualizar ou deletar a "linha 0" (primeira linha de dados), ela sera corretamente identificada.
    int atualizou = 0; // a funcao retorna atualizou para indicar sucesso (1) ou falha (0).

    // Copia o cabecalho
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

    // Substitui o arquivo original pelo temporario
    if (remove(nome_arquivo) != 0 || rename("temp.csv", nome_arquivo) != 0)
    {
        msg_erro_abrir_arquivo_nome(nome_arquivo);
        return 0;
    }

    return atualizou;
}

// Remove a linha 'row' (comecando em 0, sem contar o cabecalho) do arquivo CSV
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

    // Copia o cabecalho
    if (fgets(linha, sizeof(linha), arquivo))
    {
        fputs(linha, temp);
    }

    // Copia as linhas, pulando a que sera deletada
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
int exibir_arquivo(const char *nome_arquivo, const char *modo, const char *valor)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo '%s'.\n", nome_arquivo);
        return 0;
    }

    char linha[512];
    char id_consulta_csv[16]; // Para armazenar o ID da consulta lido do CSV
    char id_paciente_csv[16];
    char id_medico_csv[16];
    char data_hora_csv[32];
    char status_csv[16];
    char nome_paciente[120];
    char nome_medico[100];

    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho

    int encontrou_algo = 0;

    while (fgets(linha, sizeof(linha), arquivo))
    {
        // Zera as strings para evitar lixo de leituras anteriores
        memset(id_consulta_csv, 0, sizeof(id_consulta_csv));
        memset(id_paciente_csv, 0, sizeof(id_paciente_csv));
        memset(id_medico_csv, 0, sizeof(id_medico_csv));
        memset(data_hora_csv, 0, sizeof(data_hora_csv));
        memset(status_csv, 0, sizeof(status_csv));
        memset(nome_paciente, 0, sizeof(nome_paciente));
        memset(nome_medico, 0, sizeof(nome_medico));

        if (sscanf(linha, "%15[^,],%15[^,],%15[^,],%31[^,],%15[^\n]",
                   id_consulta_csv, id_paciente_csv, id_medico_csv, data_hora_csv, status_csv) != 5)
        {
            // Se o sscanf falhar em parsear 5 campos, a linha está malformada ou é o final do arquivo com linha em branco.
            // printf("Erro ao ler linha do arquivo de consultas ou linha malformada: %s", linha);
            continue;
        }

        int id_pac_int = atoi(id_paciente_csv);
        int id_med_int = atoi(id_medico_csv);

        buscar_nome_por_id(ARQ_PACIENTES, id_pac_int, nome_paciente, sizeof(nome_paciente));
        buscar_nome_por_id(ARQ_MEDICOS, id_med_int, nome_medico, sizeof(nome_medico));

        int exibir_linha_atual = 0;

        if (strcmp(modo, "paciente") == 0 && strcmp(id_paciente_csv, valor) == 0)
        {
            exibir_linha_atual = 1;
        }
        else if (strcmp(modo, "medico") == 0 && strcmp(id_medico_csv, valor) == 0)
        {
            exibir_linha_atual = 1;
        }
        else if (strcmp(modo, "especialidade") == 0)
        {
            char especialidade_medico_atual[100];
            if (buscar_especialidade_medico_por_id(id_med_int, especialidade_medico_atual, sizeof(especialidade_medico_atual)))
            {
                // Compara ignorando maiúsculas/minúsculas
                if (strcasecmp(especialidade_medico_atual, valor) == 0)
                {
                    exibir_linha_atual = 1;
                }
            }
        }
        else if (strcmp(modo, "data") == 0 && strstr(data_hora_csv, valor) != NULL) // valor deve ser DD/MM/YYYY
        {
            // Para comparar apenas a data, ignorando a hora:
            char data_consulta_parseada[11]; // Para "DD/MM/YYYY"
            strncpy(data_consulta_parseada, data_hora_csv, 10);
            data_consulta_parseada[10] = '\0';
            if (strcmp(data_consulta_parseada, valor) == 0)
            {
                exibir_linha_atual = 1;
            }
        }

        if (exibir_linha_atual)
        {
            if (!encontrou_algo)
            { // Imprime o cabeçalho apenas uma vez quando o primeiro item for encontrado
                msg_cabecalho_tabela_consultas();
                encontrou_algo = 1;
            }
            msg_linha_tabela_consulta(atoi(id_consulta_csv), nome_paciente, nome_medico, data_hora_csv, status_csv);
        }
    }

    if (encontrou_algo)
    {
        msg_rodape_tabela_consultas();
    }
    else
    {
        printf("Nenhuma consulta encontrada para os critérios informados.\n");
    }

    fclose(arquivo);
    return encontrou_algo; // Retorna 1 se encontrou algo, 0 caso contrário
}

void relatorio_contagem_consultas_por_especialidade()
{
    // Nomes das especialidades para exibição e comparação.
    // A ordem DEVE corresponder ao enum Especialidade em estruturas.h
    const char *nomes_especialidades_str[] = {
        "CLINICO_GERAL",  // Corresponde a CLINICO_GERAL = 0
        "PEDIATRA",       // Corresponde a PEDIATRA = 1
        "CARDIOLOGISTA",  // Corresponde a CARDIOLOGISTA = 2
        "DERMATOLOGISTA", // Corresponde a DERMATOLOGISTA = 3
        "PSIQUIATRA"      // Corresponde a PSIQUIATRA = 4
    };
    // TOTAL_ESPECIALIDADES deve ser 5, conforme definido em estruturas.h
    int contagem[TOTAL_ESPECIALIDADES] = {0}; // Inicializa todas as contagens com 0

    FILE *arq_consultas = fopen(ARQ_CONSULTAS, "r");
    if (!arq_consultas)
    {
        msg_erro_abrir_arquivo_nome(ARQ_CONSULTAS);
        return;
    }

    char linha_consulta[512];
    fgets(linha_consulta, sizeof(linha_consulta), arq_consultas); // Pula o cabeçalho do arquivo de consultas

    while (fgets(linha_consulta, sizeof(linha_consulta), arq_consultas))
    {
        char id_consulta_str[16], id_paciente_str[16], id_medico_str[16];
        char data_hora_str[32], status_str[16];

        if (sscanf(linha_consulta, "%15[^,],%15[^,],%15[^,],%31[^,],%15[^\n]",
                   id_consulta_str, id_paciente_str, id_medico_str, data_hora_str, status_str) == 5)
        {
            int id_medico = atoi(id_medico_str);
            char especialidade_medico_str[100]; // Buffer para armazenar a especialidade do médico

            // Busca a especialidade do médico pelo ID
            // buscar_especialidade_medico_por_id deve ler o arquivo de médicos,
            // encontrar o médico pelo id_medico e preencher especialidade_medico_str.
            if (buscar_especialidade_medico_por_id(id_medico, especialidade_medico_str, sizeof(especialidade_medico_str)))
            {
                // Compara a especialidade encontrada com a lista de especialidades conhecidas
                for (int i = 0; i < TOTAL_ESPECIALIDADES; i++)
                {
                    // Comparação case-insensitive é mais robusta se os dados no CSV puderem variar
                    if (strcasecmp(especialidade_medico_str, nomes_especialidades_str[i]) == 0)
                    {
                        contagem[i]++; // Incrementa a contagem para a especialidade correspondente
                        break;         // Sai do loop interno, pois a especialidade foi encontrada
                    }
                }
            }
            // else {
            //     printf("DEBUG: Não foi possível buscar especialidade para o médico ID %d\n", id_medico);
            // }
        }
    }
    fclose(arq_consultas);

    // Exibe o relatório formatado
    printf("\nRelatório de Consultas por Especialidade\n");
    printf("+-------------------+-------------------------+\n");
    printf("| Especialidade     | Quantidade de Consultas |\n");
    printf("+-------------------+-------------------------+\n");
    for (int i = 0; i < TOTAL_ESPECIALIDADES; i++)
    {
        // Usa os nomes do array nomes_especialidades_str para exibição
        printf("| %-17s | %-23d |\n", nomes_especialidades_str[i], contagem[i]);
    }
    printf("+-------------------+-------------------------+\n");
}