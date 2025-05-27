#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include "../include/estruturas.h"

// Cria um arquivo CSV com o cabeçalho adequado se ele não existir
void criar_arquivo_csv(const char *nome_arquivo)
{
    struct stat buffer; // Estrutura usada para obter informações sobre arquivos do sistema operacional (ex: verificar se o arquivo existe)

    int arquivo_status = (stat(nome_arquivo, &buffer) == 0); // Verifica se o arquivo já existe

    if (!arquivo_status)
    {
        FILE *arquivo = fopen(nome_arquivo, "w"); // Cria o arquivo para escrita
        if (arquivo == NULL)
        {
            printf("Erro ao criar o arquivo '%s'.\n", nome_arquivo);
            return;
        }

        // strstr: procura uma substring dentro de uma string
        // Verifica se o nome do arquivo contém o texto "registro_consultas.csv" -> basicamente verifica qual o arquivo para escrever
        // Se sim, escreve o cabeçalho específico para o arquivo de consultas
        if (strstr(nome_arquivo, "registro_consultas.csv") != NULL)
        {
            fprintf(arquivo, "id_consulta,id_paciente,id_medico,data_hora,status\n"); // cabeçalho de consulta
        }
        else if (strstr(nome_arquivo, "registro_pacientes.csv") != NULL)
        {
            fprintf(arquivo, "id_paciente,nome,cpf,telefone\n"); // cabeçalho de registro_pacientes, pensando em remover id_paciente
        }
        else if (strstr(nome_arquivo, "registro_medicos.csv") != NULL)
        {
            fprintf(arquivo, "id_medico,nome,crm,especialidade,telefone\n");
        }

        fclose(arquivo); // Fecha o arquivo
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

// Converte reg_medico para array de strings (buffers externos para campos numéricos)
void medico_to_array(const reg_medico *m, char *valores[4], char crm[], char telefone[])
{
    valores[0] = (char *)m->nome; // Nome já é string, pode apontar direto
    /* snprintf é uma função que escreve dados formatados em uma string
     crm é o buffer onde a string será armazenada
     16 é o tamanho máximo do buffer.
     "%d" indica que queremos escrever um número inteiro decimal.
     m->crm é o valor inteiro que será convertido para texto.
    */
    snprintf(crm, 16, "%d", m->crm); // Converte int para string
    valores[1] = crm;
    valores[2] = (char *)m->especialidade;     // Especialidade já é string
    snprintf(telefone, 16, "%d", m->telefone); // Converte int para string
    valores[3] = telefone;
}

// Converte reg_paciente para array de strings (todos já são strings)
void paciente_to_array(const reg_paciente *p, char *valores[3])
{
    valores[0] = (char *)p->nome;
    valores[1] = (char *)p->cpf;
    valores[2] = (char *)p->telefone;
}

// Converte reg_consulta para array de strings (buffers externos para campos numéricos e data)
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

// Retorna o maior id da primeira coluna do arquivo CSV
int get_maior_id(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
        return 0;

    char linha[512];
    int maior_id = 0;
    fgets(linha, sizeof(linha), arquivo); // lê a primeira linha e fica por isso mesmo

    while (fgets(linha, sizeof(linha), arquivo)) // lê as demais linhas
    {
        int id_temp = 0;
        /* acessa a "linha" e extrai o primeiro numero inteiro antes da virgula em "%d," e armazena no endereço id_temp
        sscanf : ler dados formatados de uma string
        */
        sscanf(linha, "%d,", &id_temp);

        // verificar se o id_temp é o maior
        if (id_temp > maior_id)
            maior_id = id_temp;
    }

    fclose(arquivo); // fecha o arquivo

    return maior_id;
}

// Retorna 1 se encontrar, 0 se não encontrar. Se row==true, retorna a linha encontrada (>=0) ou -1 se não encontrou
int get_linhas(const char *nome_arquivo, int indice_coluna, const char *valor_busca, bool row)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
        return row ? -1 : 0;

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho

    int linha_atual = 0;
    while (fgets(linha, sizeof(linha), arquivo))
    {
        char *token;
        char linha_copia[512];
        strcpy(linha_copia, linha);

        int coluna_atual = 0;
        token = strtok(linha_copia, ",\n");
        while (token != NULL)
        {
            if (coluna_atual == indice_coluna)
            {
                if (strcmp(token, valor_busca) == 0)
                {
                    fclose(arquivo);
                    return row ? linha_atual : 1;
                }
                break;
            }
            token = strtok(NULL, ",\n");
            coluna_atual++;
        }
        linha_atual++;
    }

    fclose(arquivo);
    return row ? -1 : 0;
}

void view_file(const char *nome_arquivo)
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
        char *token;
        char *resto = linha;
        int coluna = 0;

        // Imprime cada campo com largura fixa para alinhar
        while ((token = strtok(coluna == 0 ? resto : NULL, ",\n")) != NULL)
        {
            printf("| %-18s ", token);
            coluna++;
        }
        printf("|\n");

        // Linha separadora após o cabeçalho
        if (primeira_linha)
        {
            for (int i = 0; i < coluna; i++)
                printf("+--------------------");
            printf("+\n");
            primeira_linha = 0;
        }
    }
    fclose(arquivo);
}