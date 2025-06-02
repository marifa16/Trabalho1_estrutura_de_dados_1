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
        ;
}

int ler_opcao_menu(int min, int max)
{
    char entrada[16];
    int valor;
    int tentativas = 0; // Para evitar loops infinitos em caso de erro persistente

    do
    {
        printf("Escolha uma opção: ");
        if (fgets(entrada, sizeof(entrada), stdin) == NULL)
        {
            // Tratar erro de leitura, se necessário
            if (tentativas++ > 5)
                return -1; // Evitar loop infinito
            continue;
        }
        entrada[strcspn(entrada, "\n")] = '\0'; // Remove o newline

        if (sscanf(entrada, "%d", &valor) == 1 && valor >= min && valor <= max)
        {
            return valor; // Retorna o valor válido
        }
        else
        {
            msg_02_opcao_invalida();
            if (tentativas++ > 5)
            { // Limita o número de tentativas
                printf("Muitas tentativas inválidas. Retornando ao menu anterior.\n");
                return -1; // Ou um valor que indique falha/cancelamento
            }
        }
    } while (1); // Loop até obter uma entrada válida ou exceder tentativas
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
    valores[2] = (char *)m->especialidade;
    valores[3] = (char *)m->telefone; // Telefone agora é string, só aponta
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
int get_id(const char *nome_arquivo, int select_col, const char *valor)
{
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo '%s'.\n", nome_arquivo);
        return -1;
    }

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho

    while (fgets(linha, sizeof(linha), arquivo))
    {
        char colunas[10][128]; // Supondo no máximo 10 colunas
        int col_index = 0;
        char *token = strtok(linha, ",");
        while (token)
        {
            strcpy(colunas[col_index++], token);
            token = strtok(NULL, ",");
        }

        // Remove espaços ou caracteres indesejados do valor comparado
        colunas[select_col][strcspn(colunas[select_col], "\n")] = '\0';
        if (strcmp(colunas[select_col], valor) == 0)
        {
            fclose(arquivo);
            return atoi(colunas[0]); // Retorna o ID (primeira coluna)
        }
    }

    fclose(arquivo);
    return -1; // Retorna -1 se não encontrar
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
        /* acessa a "linha" e extrai o primeiro numero inteiro antes da virgula em "%d," e armazena no endereco id_temp
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
void buscar_nome_paciente_por_id(int id_paciente, char *nome_paciente, size_t tamanho)
{
    FILE *arquivo = fopen(ARQ_PACIENTES, "r");
    if (!arquivo)
    {
        strncpy(nome_paciente, "DESCONHECIDO", tamanho);
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho

    while (fgets(linha, sizeof(linha), arquivo))
    {
        int id;
        char nome[128];
        sscanf(linha, "%d,%127[^,]", &id, nome);

        if (id == id_paciente)
        {
            strncpy(nome_paciente, nome, tamanho);
            fclose(arquivo);
            return;
        }
    }

    fclose(arquivo);
    strncpy(nome_paciente, "DESCONHECIDO", tamanho);
}

// Busca o nome do médico pelo id_medico
void buscar_nome_medico_por_id(int id_medico, char *nome_medico, size_t tamanho)
{
    FILE *arquivo = fopen(ARQ_MEDICOS, "r");
    if (!arquivo)
    {
        strncpy(nome_medico, "DESCONHECIDO", tamanho);
        return;
    }

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho

    while (fgets(linha, sizeof(linha), arquivo))
    {
        int id;
        char nome[128];
        sscanf(linha, "%d,%127[^,]", &id, nome);

        if (id == id_medico)
        {
            strncpy(nome_medico, nome, tamanho);
            fclose(arquivo);
            return;
        }
    }

    fclose(arquivo);
    strncpy(nome_medico, "DESCONHECIDO", tamanho);
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
void atualizar_paciente(const char *file_paciente, char *cpf_busca, char *valores_finais[4])
{
    int linha = buscar_linha(file_paciente, 2, cpf_busca); // Coluna 2 é CPF no arquivo de pacientes
    if (linha < 0)
    {
        msg_paciente_nao_encontrado(cpf_busca);
        return;
    }

    FILE *arquivo_leitura = fopen(file_paciente, "r");
    if (!arquivo_leitura)
    {
        msg_erro_abrir_arquivo_nome(file_paciente);
        return;
    }

    char linha_csv[512];
    // Pula cabeçalho
    if (fgets(linha_csv, sizeof(linha_csv), arquivo_leitura) == NULL)
    {
        fclose(arquivo_leitura);
        msg_erro_atualizar_paciente();
        return;
    }

    // Lê até a linha do paciente
    for (int i = 0; i <= linha; i++) // Itera até a linha correta (0-indexed)
    {
        if (fgets(linha_csv, sizeof(linha_csv), arquivo_leitura) == NULL)
        {
            fclose(arquivo_leitura);
            msg_erro_atualizar_paciente(); // Erro inesperado ao ler a linha do paciente
            return;
        }
    }
    fclose(arquivo_leitura);

    // strtok modifica a string, então fazemos uma cópia para tokenização
    char linha_copia[512];
    strncpy(linha_copia, linha_csv, sizeof(linha_copia) - 1);
    linha_copia[sizeof(linha_copia) - 1] = '\0';

    // Buffers para os dados atuais do paciente
    char id_atual_str[16];
    char nome_atual[120];
    char cpf_original[12];
    char telefone_atual[12];

    char *token;
    token = strtok(linha_copia, ",\n");
    if (token)
        strncpy(id_atual_str, token, sizeof(id_atual_str) - 1);
    else
        strcpy(id_atual_str, "");
    id_atual_str[sizeof(id_atual_str) - 1] = '\0';

    token = strtok(NULL, ",\n");
    if (token)
        strncpy(nome_atual, token, sizeof(nome_atual) - 1);
    else
        strcpy(nome_atual, "");
    nome_atual[sizeof(nome_atual) - 1] = '\0';

    token = strtok(NULL, ",\n");
    if (token)
    {
        strncpy(cpf_original, token, sizeof(cpf_original) - 1);
        cpf_original[sizeof(cpf_original) - 1] = '\0';
    }
    else
        strcpy(cpf_original, "");

    token = strtok(NULL, ",\n");
    if (token)
        strncpy(telefone_atual, token, sizeof(telefone_atual) - 1);
    else
        strcpy(telefone_atual, "");
    telefone_atual[sizeof(telefone_atual) - 1] = '\0';

    // Buffers para os novos dados (inicializados com os atuais)
    char novo_nome[120];
    strncpy(novo_nome, nome_atual, sizeof(novo_nome));
    char novo_cpf[12];
    strncpy(novo_cpf, cpf_original, sizeof(novo_cpf));
    char novo_telefone[12];
    strncpy(novo_telefone, telefone_atual, sizeof(novo_telefone));

    int continuar_atualizacao_menu = 1;
    while (continuar_atualizacao_menu)
    {
        msg_16_atualizar_paciente();
        printf("[4] - Salvar e Sair\n");
        printf("[5] - Cancelar e Sair\n");
        printf("===========================\n");

        int escolha_campo = ler_opcao_menu(1, 5);

        switch (escolha_campo)
        {
        case 1: // Atualizar Nome
        {
            printf("Nome atual: %s\n", novo_nome);
            do
            {
                msg_17_atualizar_nome();
                if (!validar_nome_padrao(novo_nome, sizeof(novo_nome)))
                {
                    printf("Tente novamente ou cancele a alteração do nome.\n");
                    continue;
                }
                printf("Novo nome proposto: %s\n", novo_nome);
                msg_40_opcoes();
                int opcao_confirma = ler_opcao_menu(1, 3);

                if (opcao_confirma == 1)
                    break;
                else if (opcao_confirma == 2)
                {
                    strncpy(novo_nome, nome_atual, sizeof(novo_nome));
                    continue;
                }
                else if (opcao_confirma == 3)
                {
                    strncpy(novo_nome, nome_atual, sizeof(novo_nome));
                    msg_05_retornando_menu();
                    break;
                }
            } while (1);
            break;
        }
        case 2: // Atualizar CPF
        {
            printf("CPF atual: %s\n", novo_cpf);
            do
            {
                msg_19_novo_cpf();
                limpar_buffer(); // Mantido para limpar buffer antes de fgets para o novo CPF
                fgets(novo_cpf, sizeof(novo_cpf), stdin);
                novo_cpf[strcspn(novo_cpf, "\n")] = '\0';

                if (strlen(novo_cpf) == 0)
                {
                    printf("Entrada vazia. O CPF não será alterado.\n");
                    strncpy(novo_cpf, cpf_original, sizeof(novo_cpf));
                    break;
                }

                if (strlen(novo_cpf) != 11)
                {
                    msg_cpf_invalido();
                    strncpy(novo_cpf, cpf_original, sizeof(novo_cpf));
                    continue;
                }
                int valido_fmt = 1;
                for (size_t i = 0; i < strlen(novo_cpf); i++)
                {
                    if (!isdigit(novo_cpf[i]))
                    {
                        valido_fmt = 0;
                        break;
                    }
                }
                if (!valido_fmt)
                {
                    msg_erro_cpf_numeros();
                    strncpy(novo_cpf, cpf_original, sizeof(novo_cpf));
                    continue;
                }
                if (strcmp(novo_cpf, cpf_original) != 0 && contem_valor(file_paciente, 2, novo_cpf))
                {
                    printf("ERRO: Novo CPF (%s) já cadastrado para outro paciente.\n", novo_cpf);
                    strncpy(novo_cpf, cpf_original, sizeof(novo_cpf));
                    continue;
                }

                printf("Novo CPF proposto: %s\n", novo_cpf);
                msg_40_opcoes();
                int opcao_confirma = ler_opcao_menu(1, 3);

                if (opcao_confirma == 1)
                    break;
                else if (opcao_confirma == 2)
                {
                    strncpy(novo_cpf, cpf_original, sizeof(novo_cpf));
                    continue;
                }
                else if (opcao_confirma == 3)
                {
                    strncpy(novo_cpf, cpf_original, sizeof(novo_cpf));
                    msg_05_retornando_menu();
                    break;
                }
            } while (1);
            break;
        }
        case 3: // Atualizar Telefone
        {
            printf("Telefone atual: %s\n", novo_telefone);
            do
            {
                msg_20_novo_telefone();
                if (!validar_telefone_padrao(novo_telefone, sizeof(novo_telefone)))
                {
                    printf("Tente novamente ou cancele a alteração do telefone.\n");
                    continue;
                }
                printf("Novo telefone proposto: %s\n", novo_telefone);
                msg_40_opcoes();
                int opcao_confirma = ler_opcao_menu(1, 3);

                if (opcao_confirma == 1)
                    break;
                else if (opcao_confirma == 2)
                {
                    strncpy(novo_telefone, telefone_atual, sizeof(novo_telefone));
                    continue;
                }
                else if (opcao_confirma == 3)
                {
                    strncpy(novo_telefone, telefone_atual, sizeof(novo_telefone));
                    msg_05_retornando_menu();
                    break;
                }
            } while (1);
            break;
        }
        case 4: // Salvar Alterações e Sair
            continuar_atualizacao_menu = 0;

            valores_finais[0] = id_atual_str;
            valores_finais[1] = novo_nome;
            valores_finais[2] = novo_cpf;
            valores_finais[3] = novo_telefone;

            if (att_row(file_paciente, linha, 4, valores_finais))
            {
                msg_paciente_atualizado();
            }
            else
            {
                msg_erro_atualizar_paciente();
            }
            break;
        case 5: // Cancelar Alterações e Sair
            continuar_atualizacao_menu = 0;
            printf("===========================\n"
                   "Atualização cancelada.\nNenhuma alteração foi salva.\n"
                   "===========================\n");
            break;
        default:
            msg_02_opcao_invalida();
            break;
        }
    }
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
    char crm[16];
    char opcao[10];

    msg_30_crm();
    fgets(crm, sizeof(crm), stdin);
    crm[strcspn(crm, "\n")] = '\0';

    for (int i = 0; i < total_medicos; i++)
    {
        if (strcmp(medicos[i].crm, crm) == 0)
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
    fgets(opcao, sizeof(opcao), stdin);
    opcao[strcspn(opcao, "\n")] = '\0';
    if (strcmp(opcao, "SAIR") == 0)
    {
        return 0;
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
        fgets(opcao, sizeof(opcao), stdin);
        opcao[strcspn(opcao, "\n")] = '\0';

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

// Busca a especialidade do médico pelo id_medico
int buscar_especialidade_medico_por_id(int id_medico_busca, char *especialidade_medico, size_t tam)
{
    FILE *arquivo = fopen(ARQ_MEDICOS, "r");
    if (!arquivo)
    {
        strncpy(especialidade_medico, "DESCONHECIDA", tam - 1);
        especialidade_medico[tam - 1] = '\0';
        return 0; // Falha ao abrir o arquivo
    }

    char linha[512];
    char id_csv[16];
    char nome_csv[100];
    char crm_csv[16];
    char especialidade_csv[100];

    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho

    while (fgets(linha, sizeof(linha), arquivo))
    {
        if (sscanf(linha, "%15[^,],%99[^,],%15[^,],%99[^,],%*[^\n]",
                   id_csv, nome_csv, crm_csv, especialidade_csv) == 4)
        {
            if (atoi(id_csv) == id_medico_busca)
            {
                strncpy(especialidade_medico, especialidade_csv, tam - 1);
                especialidade_medico[tam - 1] = '\0';
                fclose(arquivo);
                return 1; // Encontrou
            }
        }
    }

    fclose(arquivo);
    strncpy(especialidade_medico, "DESCONHECIDA", tam - 1);
    especialidade_medico[tam - 1] = '\0';
    return 0; // Não encontrou
}

int validar_telefone_padrao(char *telefone, size_t tamanho)
{
    fgets(telefone, tamanho, stdin);
    telefone[strcspn(telefone, "\n")] = '\0';

    if (strlen(telefone) == 0 && tamanho > 1)
    {
        msg_erro_telefone_digitos(); // Ou "Telefone não pode ser vazio"
        limpar_buffer();             // Se a entrada foi só \n, já foi consumido. Mas se foi longa e o primeiro char foi \0 por algum motivo.
        return 0;
    }

    if (strlen(telefone) != 11)
    {
        msg_erro_telefone_digitos();
        // Se a entrada foi maior que o buffer, o \n e o resto ficaram.
        if (strlen(telefone) == tamanho - 1 && telefone[tamanho - 2] != '\0')
        { // Verifica se o buffer foi preenchido
            limpar_buffer();
        }
        return 0;
    }

    for (size_t i = 0; i < strlen(telefone); i++)
    {
        if (!isdigit((unsigned char)telefone[i]))
        {
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
        msg_erro_nome_vazio();
        // Se a entrada foi apenas '\n', o buffer de stdin está limpo.
        // Se a entrada foi "  \n" e nome ficou "", buffer limpo.
        // Se a entrada foi muito longa, o '\n' e o resto ficaram.
        limpar_buffer();
        return 0;
    }

    for (size_t i = 0; i < strlen(nome); i++)
    {
        // Permitir espaços, mas não apenas espaços. Poderia adicionar trim aqui.
        // Validação simples: não permite números.
        if (isdigit((unsigned char)nome[i]))
        {
            msg_erro_nome_numeros();
            // Não é necessário limpar_buffer aqui se a entrada coube e o \n foi removido.
            return 0;
        }
    }
    // Adicional: verificar se o nome não é composto apenas de espaços
    int so_espacos = 1;
    for (size_t i = 0; i < strlen(nome); i++)
    {
        if (!isspace((unsigned char)nome[i]))
        {
            so_espacos = 0;
            break;
        }
    }
    if (so_espacos && strlen(nome) > 0)
    {
        msg_erro_nome_vazio();
        return 0;
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

int cpf_valido(const char *cpf)
{
    if (strlen(cpf) != 11)
        return 0;
    for (size_t i = 0; i < 11; i++)
        if (cpf[i] < '0' || cpf[i] > '9')
            return 0;
    return 1;
}

void carregar_pacientes_do_arquivo()
{
    FILE *arquivo = fopen("data/registro_pacientes.csv", "r");
    if (!arquivo)
        return;

    char linha[512];
    fgets(linha, sizeof(linha), arquivo); // Pula o cabeçalho

    // Libera vetor antigo, se houver
    if (pacientes)
    {
        free(pacientes);
        pacientes = NULL;
        total_pacientes = 0;
    }

    while (fgets(linha, sizeof(linha), arquivo))
    {
        reg_paciente temp;
        int id;
        if (sscanf(linha, "%d,%119[^,],%11[^,],%11[^\n]", &id, temp.nome, temp.cpf, temp.telefone) == 4)
        {
            reg_paciente *novo = realloc(pacientes, (total_pacientes + 1) * sizeof(reg_paciente));
            if (!novo)
                break;
            pacientes = novo;
            pacientes[total_pacientes] = temp;
            pacientes[total_pacientes].id_paciente = id;
            total_pacientes++;
        }
    }
    fclose(arquivo);
}

void obter_data_atual(char *data_atual)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(data_atual, 11, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}
