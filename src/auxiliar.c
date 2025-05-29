#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <strings.h> // Para strcasecmp
#include "../include/estruturas.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_paciente.h"
#include "../include/validacoes.h"

void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // Consome todos os caracteres restantes no buffer
}

int validar_nome_paciente(char *nome_paciente, size_t tamanho)
{
    do
    {
        msg_11_nome_paciente();
        fgets(nome_paciente, tamanho, stdin);               // Lê o nome do paciente
        nome_paciente[strcspn(nome_paciente, "\n")] = '\0'; // Remove o caractere de nova linha

        if (strlen(nome_paciente) == 0) // Verifica se o nome está vazio
        {
            msg_02_opcao_invalida();
            continue; // Volta para a pergunta do nome
        }

        return 1; // Nome válido
    } while (1);
}

int validar_opcao_usuario()
{
    char opcao[10];

    do
    {
        // Lê a opção do usuário
        // limpar_buffer(); // Limpa o buffer antes de capturar a entrada
        msg_40_opcoes();

        if (fgets(opcao, sizeof(opcao), stdin) == NULL)
        {
            msg_02_opcao_invalida();
            continue;
        }
        opcao[strcspn(opcao, "\n")] = '\0'; // Remove o caractere de nova linha

        if (strcmp(opcao, "1") == 0) // Se a opção for "SIM"
        {
            return 1; // Confirmação
        }
        else if (strcmp(opcao, "2") == 0) // Se a opção for "NÃO"
        {
            return 2; // Rejeição
        }
        else if (strcmp(opcao, "3") == 0) // Se a opção for "SAIR"
        {
            return 3; // Sair
        }
        else // Se a entrada for inválida
        {
            msg_02_opcao_invalida();
        }
    } while (1);
}

int validar_cpf(char *cpf, size_t tamanho)
{
    do
    {
        msg_13_informar_cpf(); // Mensagem para solicitar o CPF
        // limpar_buffer();                // Limpa o buffer antes de capturar o CPF
        fgets(cpf, tamanho, stdin);     // Lê o CPF como string
        cpf[strcspn(cpf, "\n")] = '\0'; // Remove o caractere de nova linha

        // Verifica se o CPF possui exatamente 11 dígitos
        if (strlen(cpf) != 11)
        {
            msg_02_opcao_invalida();
            printf("O CPF deve conter exatamente 11 dígitos.\n");
            continue;
        }

        // Verifica se todos os caracteres são números
        int valido = 1;
        for (size_t i = 0; i < strlen(cpf); i++)
        {
            if (cpf[i] < '0' || cpf[i] > '9')
            {
                valido = 0;
                break;
            }
        }

        if (!valido)
        {
            msg_02_opcao_invalida();
            printf("O CPF deve conter apenas números.\n");
            continue;
        }

        return 1; // CPF válido
    } while (1);
}

int validar_telefone(char *telefone, size_t tamanho)
{
    do
    {
        msg_14_informar_telefone(); // Mensagem para solicitar o telefone
        // limpar_buffer();                          // Limpa o buffer antes de capturar o telefone
        fgets(telefone, tamanho, stdin);          // Lê o telefone como string
        telefone[strcspn(telefone, "\n")] = '\0'; // Remove o caractere de nova linha

        // Verifica se o telefone possui exatamente 11 dígitos
        if (strlen(telefone) != 11)
        {
            msg_02_opcao_invalida();
            printf("O telefone deve conter exatamente 11 dígitos.\n");
            continue;
        }

        // Verifica se todos os caracteres são números
        int valido = 1;
        for (size_t i = 0; i < strlen(telefone); i++)
        {
            if (telefone[i] < '0' || telefone[i] > '9')
            {
                valido = 0;
                break;
            }
        }

        if (!valido)
        {
            msg_02_opcao_invalida();
            printf("O telefone deve conter apenas números.\n");
            continue;
        }

        return 1; // Telefone válido
    } while (1);
}

// Converte reg_medico para array de strings (buffers externos para campos numéricos)
void medico_to_array(const reg_medico *m, char *valores[4], char crm[])
{
    valores[0] = (char *)m->nome;    // Nome já é string
    snprintf(crm, 16, "%d", m->crm); // CRM ainda é int, converte para string
    valores[1] = crm;
    valores[2] = (char *)m->especialidade; // Especialidade já é string
    valores[3] = (char *)m->telefone;      // Telefone agora é string, só aponta
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

// Retorna o id da primeira coluna da linha que contém o valor buscado na coluna select_col
int get_id(const char *nome_arquivo, int select_col, const char *valor_busca)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
        return -1;

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho

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
    return -1; // Não encontrado
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
            printf("%s", linha); // Cabeçalho
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
    fgets(linha, sizeof(linha), arquivo); // lê a primeira linha e fica por isso mesmo

    while (fgets(linha, sizeof(linha), arquivo)) // lê as demais linhas
    {
        int id_temp = 0;
        /* acessa a "linha" e extrai o primeiro numero inteiro antes da virgula em "%d," e armazena no endereÃ§o id_temp
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

// Retorna o número da linha (começando em 0, sem contar o cabeçalho) onde encontrou o valor na coluna indicada, ou -1 se não encontrou
int buscar_linha(const char *nome_arquivo, int indice_coluna, const char *valor_busca)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL)
        return -1;

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho

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

// Retorna 1 se encontrou o valor na coluna indicada, 0 se não encontrou
int contem_valor(const char *nome_arquivo, int indice_coluna, const char *valor_busca)
{
    return buscar_linha(nome_arquivo, indice_coluna, valor_busca) >= 0 ? 1 : 0;
}

// Busca o nome do paciente pelo id_paciente
void buscar_nome_paciente_por_id(int id_paciente, char *nome, size_t tamanho)
{
    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%d", id_paciente);
    int linha = buscar_linha("data/registro_pacientes.csv", 0, id_str);
    if (linha < 0)
    {
        strncpy(nome, "Desconhecido", tamanho);
        return;
    }
    FILE *arq = fopen("data/registro_pacientes.csv", "r");
    if (!arq)
    {
        strncpy(nome, "Desconhecido", tamanho);
        return;
    }
    char linha_arq[512];
    fgets(linha_arq, sizeof(linha_arq), arq); // pula cabeçalho
    for (int i = 0; i <= linha; i++)
        fgets(linha_arq, sizeof(linha_arq), arq);
    int id_tmp;
    char nome_lido[120];
    if (sscanf(linha_arq, "%d,%119[^,]", &id_tmp, nome_lido) == 2)
        strncpy(nome, nome_lido, tamanho);
    else
        strncpy(nome, "Desconhecido", tamanho);
    fclose(arq);
}

// Busca o nome do médico pelo id_medico
void buscar_nome_medico_por_id(int id_medico, char *nome, size_t tamanho)
{
    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%d", id_medico);
    int linha = buscar_linha("data/registro_medicos.csv", 0, id_str);
    if (linha < 0)
    {
        strncpy(nome, "Desconhecido", tamanho);
        return;
    }
    FILE *arq = fopen("data/registro_medicos.csv", "r");
    if (!arq)
    {
        strncpy(nome, "Desconhecido", tamanho);
        return;
    }
    char linha_arq[512];
    fgets(linha_arq, sizeof(linha_arq), arq); // pula cabeçalho
    for (int i = 0; i <= linha; i++)
        fgets(linha_arq, sizeof(linha_arq), arq);
    int id_tmp;
    char nome_lido[100];
    if (sscanf(linha_arq, "%d,%99[^,]", &id_tmp, nome_lido) == 2)
        strncpy(nome, nome_lido, tamanho);
    else
        strncpy(nome, "Desconhecido", tamanho);
    fclose(arq);
}

// Verifica se a string data_hora está no dia atual
int validar_dia(const char *data_hora)
{
    int dia, mes, ano;
    // Espera formato "dd/mm/yyyy hh:mm"
    if (sscanf(data_hora, "%d/%d/%d", &dia, &mes, &ano) != 3)
        return 0;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    return (dia == tm.tm_mday && mes == tm.tm_mon + 1 && ano == tm.tm_year + 1900);
}

// Retorna 1 se o médico do id possui a especialidade, 0 caso contrário
int get_medico_especial(int id_medico, const char *especialidade)
{
    FILE *arq = fopen("data/registro_medicos.csv", "r");
    if (!arq)
        return 0;
    char linha[512];
    fgets(linha, sizeof(linha), arq); // pula cabeçalho
    while (fgets(linha, sizeof(linha), arq))
    {
        int id;
        char nome[100], crm[16], espec[100], telefone[16];
        if (sscanf(linha, "%d,%99[^,],%15[^,],%99[^,],%15[^\n]", &id, nome, crm, espec, telefone) == 5)
        {
            // Use strcasecmp para ignorar maiúsculas/minúsculas
            if (id == id_medico && strcasecmp(espec, especialidade) == 0)
            {
                fclose(arq);
                return 1;
            }
        }
    }
    fclose(arq);
    return 0;
}