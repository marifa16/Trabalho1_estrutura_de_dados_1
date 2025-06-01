#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/auxiliar.h"
#include "../include/estruturas.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_paciente.h"
#include "../include/files_manager.h"

// Acesso ao vetor de pacientes
extern reg_paciente *pacientes; // Ponteiro para o vetor de pacientes
extern int total_pacientes;     // Contador de pacientes cadastrados
extern reg_medico *medicos;
extern int total_medicos;

// Declaração da função espcialidade_string
const char *especialidade_string(Especialidade especialidade);

void limpar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ; // Consome todos os caracteres restantes no buffer
}

int ler_opcao_menu(int min, int max)
{
    char entrada[16];
    int valor, valido = 0;

    do
    {
        msg_menu_escolha_opcao();

        // limpar_buffer();
        if (!fgets(entrada, sizeof(entrada), stdin))
        {
            msg_menu_opcao_invalida();
            continue;
        }
        entrada[strcspn(entrada, "\n")] = 0;
        int apenas_digitos = 1;
        for (size_t i = 0; entrada[i]; i++)
        {
            if (!isdigit((unsigned char)entrada[i]))
            {
                apenas_digitos = 0;
                break;
            }
        }
        if (!apenas_digitos || strlen(entrada) == 0)
        {
            msg_menu_opcao_invalida();
            continue;
        }
        valor = atoi(entrada);
        if (valor < min || valor > max)
        {
            msg_menu_opcao_invalida();
            continue;
        }
        valido = 1;
    } while (!valido);

    return valor;
}

int validar_cpf(char *cpf, size_t tamanho) // podia ter sido padronizado em uma função só, mas já tá ai pronta então não seguimos
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
            msg_erro_cpf_digitos(); // Substitui printf
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
            msg_erro_cpf_numeros(); // Substitui printf
            continue;
        }

        return 1; // CPF válido
    } while (1);
}

// Converte reg_medico para array de strings (buffers externos para campos numéricos)
void medico_to_array(const reg_medico *m, char *valores[4], char crm[])
{
    valores[0] = (char *)m->nome; // Nome já é string
    strncpy(crm, m->crm, 16);     // CRM já é string
    crm[15] = '\0';
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

// Função auxiliar para atualizar paciente
void atualizar_paciente(const char *file_paciente, char *cpf_paciente, char *valores[4])
{
    int linha = buscar_linha(file_paciente, 2, cpf_paciente);
    if (linha < 0)
    {
        msg_paciente_nao_encontrado(cpf_paciente);
        return;
    }

    FILE *arquivo = fopen(file_paciente, "r");
    if (!arquivo)
    {
        msg_erro_abrir_arquivo();
        return;
    }
    char linha_csv[512];
    fgets(linha_csv, sizeof(linha_csv), arquivo);
    for (int i = 0; i <= linha; i++)
        fgets(linha_csv, sizeof(linha_csv), arquivo);
    fclose(arquivo);

    char *tokens[4];
    char linha_copia[512];
    strcpy(linha_copia, linha_csv);
    char *token = strtok(linha_copia, ",\n");
    int i = 0;
    while (token && i < 4)
    {
        tokens[i++] = token;
        token = strtok(NULL, ",\n");
    }

    int continuar = 1;
    while (continuar)
    {
        msg_menu_atualizar_info();
        int opcao = ler_opcao_menu(1, 3);

        switch (opcao)
        {
        case 1:
            validar_nome_padrao(tokens[1], sizeof(linha_copia) - 1);
            break;
        case 2:
            validar_cpf(tokens[2], sizeof(linha_copia) - 1);
            break;
        case 3:
            validar_telefone_padrao(tokens[3], sizeof(linha_copia) - 1);
            break;
        }

        msg_menu_atualizar_mais();
        int resp = ler_opcao_menu(0, 1); // 0-Não, 1-Sim
        if (resp == 0)
            continuar = 0;
    }

    valores[0] = tokens[0];
    valores[1] = tokens[1];
    valores[2] = tokens[2];
    valores[3] = tokens[3];

    if (att_row(file_paciente, linha, 4, valores))
        msg_paciente_atualizado();
    else
        msg_erro_atualizar_paciente();
}

void listar_medicos(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        msg_erro_abrir_arquivo_nome(nome_arquivo);
        return;
    }
    printf("+-----+----------------------+---------+----------------------+-------------+\n");
    printf("| ID  | Nome                 | CRM     | Especialidade        | Telefone    |\n");
    printf("+-----+----------------------+---------+----------------------+-------------+\n");
    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Cabeçalho
    while (fgets(linha, sizeof(linha), arquivo))
    {
        int id;
        char nome[100], crm[16], especialidade[100], telefone[16];
        if (sscanf(linha, "%d,%99[^,],%15[^,],%99[^,],%15[^\n]", &id, nome, crm, especialidade, telefone) == 5)
            printf("| %-3d | %-20s | %-7s | %-20s | %-11s |\n", id, nome, crm, especialidade, telefone);
    }
    printf("+-----+----------------------+---------+----------------------+-------------+\n");
    fclose(arquivo);
}

int validar_paciente()
{
    char cpf[12];   // CPF como string (11 dígitos + '\0')
    char opcao[10]; // Variável para armazenar a opção sim/não/sair

    msg_31_cpf();
    fgets(cpf, sizeof(cpf), stdin); // Lê o CPF do usuário como string
    cpf[strcspn(cpf, "\n")] = '\0'; // Remove o caractere de nova linha

    for (int i = 0; i < total_pacientes; i++) // Percorre o vetor de pacientes
    {
        if (strcmp(pacientes[i].cpf, cpf) == 0) // Compara strings
        {
            printf("=====================\n");
            printf("Paciente cadastrado.\n");                // Exibe mensagem de sucesso
            printf("Nome: %s\n", pacientes[i].nome);         // Exibe o nome do paciente
            printf("CPF: %s\n", pacientes[i].cpf);           // Exibe o CPF do paciente
            printf("Telefone: %s\n", pacientes[i].telefone); // Exibe o telefone do paciente
            printf("=====================\n");
            return 1; // Retorna 1 se o CPF foi encontrado
        }
    }
    printf("Paciente não cadastrado.\n");         // Exibe mensagem de erro
    printf("Digite SAIR para voltar ao menu.\n"); // Informa ao usuário como voltar ao menu principal
    fgets(opcao, sizeof(opcao), stdin);           // Lê a opção do usuário
    opcao[strcspn(opcao, "\n")] = '\0';           // Remove o caractere de nova linha
    if (strcmp(opcao, "SAIR") == 0)               // Se a opção for "SAIR"
    {
        printf("Voltando ao menu...\n"); // Exibe mensagem de retorno
        return 0;                        // Retorna 0 para indicar que o usuário deseja voltar ao menu
    }
    msg_02_opcao_invalida();
    return 0; // Retorna 0 se o CPF não foi encontrado
}

int validar_medico()
{
    // Implementação validar médico
    char crm[16];
    char opcao[10];

    msg_30_crm();
    fgets(crm, sizeof(crm), stdin); // Lê a o CRM do usuário como string
    crm[strcspn(crm, "\n")] = '\0'; // Remove o caractere de nova linha

    for (int i = 0; i < total_medicos; i++) // Percorre o vetor médico
    {
        if (strcmp(medicos[i].crm, crm) == 0) // Seo CRM do médico cadastraddo for igual ao CRM informado
        {
            printf("=====================\n");
            printf("Médico cadastrado.\n");                          // Exibe mensagem de sucesso
            printf("Nome: %s\n", medicos[i].nome);                   // Exibe o nome do médico
            printf("CRM: %s\n", medicos[i].crm);                     // Exibe o CRM do médico
            printf("Especialidade: %s\n", medicos[i].especialidade); // Exibe a especialidade do médico
            printf("Telefone: %s\n", medicos[i].telefone);           // Exibe o telefone do médico
            printf("=====================\n");
            return 1; // Retorna 1 se o CPF foi encontrado
        }
    }
    printf("Medico não cadastrado.\n");           // Exibe mensagem de erro
    printf("Digite SAIR para voltar ao menu.\n"); // Informa ao usuário como voltar ao menu principal
    scanf("%s", opcao);                           // Lê a opção do usuário
    if (strcmp(opcao, "SAIR") == 0)               // Se a opção for "SAIR"
    {
        return 0; // Retorna 0 para indicar que o usuário deseja voltar ao menu
    }
    msg_02_opcao_invalida();
    return 0; // Retorna 0 se o CRM não foi encontrado
}

int validar_mais_op()
{
    char opcao[10];

    while (1)
    {
        msg_32_mudar_algo();
        scanf("%s", opcao);

        if (strcmp(opcao, "Sim") == 0)
        {
            return 1; // Retorna para continuar medando
        }
        else if (strcmp(opcao, "Não") == 0)
        {
            return 0; // Retorna para parar
        }
        else
        {
            msg_02_opcao_invalida();
        }
    }
}

void carregar_medicos_do_arquivo()
{
    FILE *arquivo = fopen("data/registro_medicos.csv", "r");
    if (!arquivo)
        return;

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho

    // Libera vetor antigo, se houver
    if (medicos)
    {
        free(medicos);
        medicos = NULL;
        total_medicos = 0;
    }

    while (fgets(linha, sizeof(linha), arquivo))
    {
        reg_medico temp;
        int id;
        char especialidade[100];
        if (sscanf(linha, "%d,%99[^,],%15[^,],%99[^,],%15[^,\n]",
                   &id, temp.nome, temp.crm, especialidade, temp.telefone) == 5)
        {
            temp.id_medico = id;
            strncpy(temp.especialidade, especialidade, sizeof(temp.especialidade));
            temp.especialidade[sizeof(temp.especialidade) - 1] = '\0';
            // Descobre o enum da especialidade
            for (int i = 0; i < TOTAL_ESPECIALIDADES; i++)
            {
                if (strcmp(especialidade, especialidade_string(i)) == 0)
                {
                    temp.especialidade_medico = i;
                    break;
                }
            }
            medicos = realloc(medicos, (total_medicos + 1) * sizeof(reg_medico));
            medicos[total_medicos++] = temp;
        }
    }
    fclose(arquivo);
}

// Função para convertere especialidade enum para string
const char *especialidade_string(Especialidade especialidade)
{
    switch (especialidade)
    {
    case CLINICO_GERAL:
        return "Clínico Geral";
    case PEDIATRA:
        return "Pedriatra";
    case CARDIOLOGISTA:
        return "Cardiologista";
    case DERMATOLOGISTA:
        return "Dermatologista";
    case PSIQUIATRA:
        return "Psiquiatra";
    default:
        return "Especialidade Desconhecida"; // Retorna uma string para especialidade desconhecida
    }
}

int validar_crm(char *crm, size_t tamanho)
{
    fgets(crm, tamanho, stdin);
    crm[strcspn(crm, "\n")] = '\0';
    if (strlen(crm) != 6)
    {
        msg_02_opcao_invalida();
        msg_erro_crm_digitos();
        return 0;
    }
    for (size_t i = 0; i < strlen(crm); i++)
    {
        if (!isdigit(crm[i]))
        {
            msg_02_opcao_invalida();
            msg_erro_crm_numeros();
            return 0;
        }
    }
    return 1;
}

int validar_data(const char *data)
{
    // Espera formato "AAAA-MM-DD"
    if (strlen(data) != 10)
        return 0;
    // Checa formato
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
        {
            if (data[i] != '-')
                return 0;
        }
        else
        {
            if (!isdigit((unsigned char)data[i]))
                return 0;
        }
    }
    int ano, mes, dia;
    if (sscanf(data, "%4d-%2d-%2d", &ano, &mes, &dia) != 3)
        return 0;
    // Checa valores válidos
    if (ano < 1900 || mes < 1 || mes > 12 || dia < 1 || dia > 31)
        return 0;
    // Checa dias do mês
    int dias_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // Ano bissexto
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
        dias_mes[1] = 29;
    if (dia > dias_mes[mes - 1])
        return 0;
    return 1;
}

int validar_mes(int mes)
{
    return mes >= 1 && mes <= 12;
}

int validar_dia2(int dia, int mes, int ano)
{
    int dias_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // Ano bissexto
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
        dias_mes[1] = 29;
    return dia >= 1 && dia <= dias_mes[mes - 1];
}

void exibir_lista_medicos()
{
    for (int i = 0; i < total_medicos; i++)
    {
        printf("[%d] - %s (CRM: %s, Especialidade: %s)\n",
               i + 1, medicos[i].nome, medicos[i].crm, especialidade_string(medicos[i].especialidade_medico));
    }
}

// auxiliar.c
int buscar_especialidade_medico_por_id(int id_medico, char *especialidade, size_t tam)
{
    FILE *arq = fopen(ARQ_MEDICOS, "r");
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
            if (id == id_medico)
            {
                strncpy(especialidade, espec, tam);
                fclose(arq);
                return 1;
            }
        }
    }
    fclose(arq);
    return 0;
}

int validar_telefone_padrao(char *telefone, size_t tamanho)
{
    fgets(telefone, tamanho, stdin);
    telefone[strcspn(telefone, "\n")] = '\0';
    if (strlen(telefone) != 11)
    {
        msg_02_opcao_invalida();
        msg_erro_telefone_digitos();
        return 0;
    }
    for (size_t i = 0; i < strlen(telefone); i++)
    {
        if (!isdigit(telefone[i]))
        {
            msg_02_opcao_invalida();
            msg_erro_telefone_numeros();
            return 0;
        }
    }
    return 1;
}

int validar_nome_padrao(char *nome, size_t tamanho)
{
    fgets(nome, tamanho, stdin);
    nome[strcspn(nome, "\n")] = '\0';
    if (strlen(nome) == 0)
    {
        msg_02_opcao_invalida();
        msg_erro_nome_vazio();
        return 0;
    }
    for (size_t i = 0; i < strlen(nome); i++)
    {
        if (isdigit(nome[i]))
        {
            msg_02_opcao_invalida();
            msg_erro_nome_numeros();
            return 0;
        }
    }
    return 1;
}

void buscar_nome_por_id(const char *arquivo, int id, char *nome, size_t tamanho)
{
    char id_str[16];
    snprintf(id_str, sizeof(id_str), "%d", id);
    int linha = buscar_linha(arquivo, 0, id_str);
    if (linha < 0)
    {
        strncpy(nome, "Desconhecido", tamanho);
        return;
    }
    FILE *arq = fopen(arquivo, "r");
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

// Retorna ponteiro para paciente pelo CPF, ou NULL se não encontrar
reg_paciente *buscar_paciente_por_cpf(const char *cpf)
{
    for (int i = 0; i < total_pacientes; i++)
    {
        if (strcmp(pacientes[i].cpf, cpf) == 0)
            return &pacientes[i]; // Retorna ponteiro para o registro encontrado
    }
    return NULL; // Não encontrou
}
