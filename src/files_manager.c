#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h> // Necessário para usar a função stat

// Função para criar um arquivo CSV
void criar_arquivo_csv(const char *nome_arquivo)
{
    struct stat buffer; // A função stat verifica informações sobre um arquivo ou diretório.
    // Ela retorna 0 se o arquivo existir e for acessível.
    // Caso contrário, retorna um valor diferente de 0.

    int arquivo_status = (stat(nome_arquivo, &buffer) == 0);
    // Se stat retornar 0, significa que o arquivo existe, e a função retorna true.
    // Caso contrário, retorna false.

    if (!arquivo_status)
    {
        FILE *arquivo = fopen(nome_arquivo, "w");
        if (arquivo == NULL)
        {
            printf("Erro ao criar o arquivo '%s'.\n", nome_arquivo);
            return; // Sai da função, pois não faz sentido continuar
        }
        // printf("O arquivo '%s' foi criado com sucesso.\n", nome_arquivo);
        fclose(arquivo);
    }
}

// Função principal para gerenciar os arquivos
void inicializar_arquivos_csv()
{
    // Cria os arquivos dentro da pasta 'data'
    criar_arquivo_csv("data/registro_consultas.csv");
    criar_arquivo_csv("data/registro_pacientes.csv");
    criar_arquivo_csv("data/registro_medicos.csv");

    printf("\nInicialização concluida\n");
}