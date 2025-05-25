#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estruturas.h"
#include "../include/modulo_consulta.h"
#include "../include/mensagens.h"
#include "../include/modulo_gerenciar_medico.h"

// Vetor de consultas
reg_consulta *consultas = NULL;
int total_consultas = 0;

// Externos já existentes
extern reg_medico *medicos;
extern int total_medicos;
extern int validar_paciente();

// Função para obter o CPF do paciente
int obter_cpf_paciente_valido()
{
    int cpf;
    // char opcao[10];
    msg_31_cpf();
    scanf("%d", &cpf);
    // Valida se o CPF existe no vetor de pacientes
    return cpf;
}

Estado tratar_modulo_consulta()
{
    Estado estado_atual = ESTADO_MENU_CONSULTA; // Inicializa o estado como ESTADO_MENU_CONSULTA
    int escolha = 0;

    do
    {
        // Exibe o menu de consultas
        msg_01_agendar_consulta();
        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1: // Agendar
        {
            // Valida paciente
            int cpf_paciente = obter_cpf_paciente_valido();
            if (!validar_paciente())
            {
                break; // Retorna ao menu de consultas
            }

            // Lista médicos cadastrados
            if (total_medicos == 0)
            {
                printf("Nenhum médico cadastrado.\n");
                break;
            }

            printf("=====================\n");
            printf("Médicos Disponíveis:\n");
            printf("=====================\n");
            for (int i = 0; i < total_medicos; i++)
            {
                printf("[%d] - %s (CRM: %d, Especialidade: %s)\n", i + 1, medicos[i].nome, medicos[i].crm, medicos[i].especialidade);
            }

            // Escolha do médico
            int escolha_medico = 0;
            char opcao[10];
            while (1)
            {
                msg_06_escolher_medico();
                scanf("%d", &escolha_medico);
                if (escolha_medico >= 1 && escolha_medico <= total_medicos)
                {
                    printf("Você escolheu: %s. Confirma?\n", medicos[escolha_medico - 1].nome);
                    printf("1 - Continuar\n2 - Escolher outro médico\n3 - Sair\n");
                    scanf("%s", opcao);
                    if (strcmp(opcao, "1") == 0)
                    {
                        break;
                    }
                    else if (strcmp(opcao, "2") == 0)
                    {
                        continue;
                    }
                    else if (strcmp(opcao, "3") == 0)
                    {
                        return ESTADO_MENU_CONSULTA;
                    }
                    else
                    {
                        msg_02_opcao_invalida();
                    }
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            }

            // Escolher data
            int dia, mes, ano = 2025;
            int dias_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

            while (1)
            {
                printf("Digite o mês desejado (1 - 12): ");
                scanf("%d", &mes);
                printf("Digite o dia desejado: ");
                scanf("%d", &dia);

                // Validação do mês e dia
                if (mes < 1 || mes > 12 || dia < 1 || dia > dias_mes[mes - 1])
                {
                    printf("Data inválida!\n");
                    continue;
                }

                // Verifica se é domingo
                struct tm data = {0};
                data.tm_year = ano - 1900;
                data.tm_mon = mes - 1;
                data.tm_mday = dia;
                mktime(&data);
                if (data.tm_wday == 0)
                {
                    printf("Não há atendimentos no domingo! Por favor, escolha outra data.\n");
                    continue;
                }

                printf("Você escolheu: %02d/%02d/%d. Podemos confirmar?\n", dia, mes, ano);
                printf("1 - Continuar\n2 - Escolher outra data\n3 - Sair\n");
                scanf("%s", opcao);
                if (strcmp(opcao, "1") == 0)
                {
                    break;
                }
                else if (strcmp(opcao, "2") == 0)
                {
                    continue;
                }
                else if (strcmp(opcao, "3") == 0)
                {
                    return ESTADO_MENU_CONSULTA;
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            }

            // Escolher horário
            int horarios_ocupados[9] = {0}; // 8 horas às 16 horas
            for (int i = 0; i < total_consultas; i++)
            {
                if (consultas[i].crm_medico == medicos[escolha_medico - 1].crm && consultas[i].dia == dia && consultas[i].mes == mes && consultas[i].ano == ano)
                {
                    int h = consultas[i].horario;
                    if (h >= 8 && h <= 16)
                    {
                        horarios_ocupados[h - 8] = 1;
                    }
                }
            }

            // Exibe horários livres
            int h_livre = 0;
            printf("Horários disponíveis para %02d/%02d/%d:\n", dia, mes, ano);
            for (int h = 8; h <= 16; h++)
            {
                if (!horarios_ocupados[h - 8])
                {
                    printf("[%d] - %02d:00\n", h, h);
                    h_livre = 1;
                }
            }
            if (!h_livre)
            {
                printf("Não possui horário disponível para esse dia!\n");
                break;
            }

            // Escolha do horário
            int escolha_horario;
            while (1)
            {
                printf("Escolha um horário (Digite a hora no formato: 08:00): \n");
                scanf("%d", &escolha_horario);
                if (escolha_horario >= 8 && escolha_horario <= 16 && !horarios_ocupados[escolha_horario - 8])
                {
                    printf("Você escolheu %02d:00. Podemos confirmar?\n", escolha_horario);
                    printf("1 - Continuar\n2 - Escolher outra data\n3 - Sair\n");
                    scanf("%s", opcao);
                    if (strcmp(opcao, "1") == 0)
                    {
                        break;
                    }
                    else if (strcmp(opcao, "2") == 0)
                    {
                        continue;
                    }
                    else if (strcmp(opcao, "3") == 0)
                    {
                        return ESTADO_MENU_CONSULTA;
                    }
                    else
                    {
                        msg_02_opcao_invalida();
                    }
                }
                else
                {
                    msg_02_opcao_invalida();
                }
            }

            // Salvar consulta
            Consulta nova;
            nova.cpf_paciente = cpf_paciente;
            nova.crm_medico = medicos[escolha_medico - 1].crm;
            nova.dia = dia;
            nova.mes = mes;
            nova.ano = ano;
            nova.horario = escolha_horario;

            Consulta *tmp = realloc(consultas, (total_consultas + 1) * sizeof(Consulta));
            if (tmp == NULL)
            {
                printf("Erro ao alocar memória para a consulta.\n");
                exit(1);
            }
            consultas = tmp;
            consultas[total_consultas++] = nova;

            msg_08_sucesso_agendamento();
            printf("Marcada para: %s em %02d/%02d/%d às %02d:00!\n", medicos[escolha_medico - 1].nome, dia, mes, ano, escolha_horario);

            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;
        }

        case 2: // Cancelar
            printf("Cancelamento de consulta selecionado.\n");
            estado_atual = ESTADO_MENU_CONSULTA;
            break;

        case 3: // Retornar ao menu principal
            estado_atual = ESTADO_MENU_PRINCIPAL;
            break;

        case 4: // Sair do programa
            estado_atual = ESTADO_SAIR;
            break;

        default:
            msg_02_opcao_invalida();
            break;
        }

    } while (estado_atual == ESTADO_MENU_CONSULTA);

    return estado_atual;
}

/*
int main()
{
    Estado estado_atual = ESTADO_MENU_CONSULTA;

    // Inicializa médicos para teste
    total_medicos = 3;
    medicos = malloc(total_medicos * sizeof(Medico));
    strcpy(medicos[0].nome, "Dr. João");
    medicos[0].crm = 123;
    strcpy(medicos[0].especialidade, "Cardiologia");
    medicos[0].telefone = 1111;

    strcpy(medicos[1].nome, "Dra. Maria");
    medicos[1].crm = 456;
    strcpy(medicos[1].especialidade, "Pediatria");
    medicos[1].telefone = 2222;

    strcpy(medicos[2].nome, "Dr. José");
    medicos[2].crm = 789;
    strcpy(medicos[2].especialidade, "Ortopedia");
    medicos[2].telefone = 3333;

    while (estado_atual != ESTADO_SAIR)
    {
        switch (estado_atual)
        {
        case ESTADO_MENU_CONSULTA:
            estado_atual = tratar_modulo_consulta();
            break;

        case ESTADO_MENU_PRINCIPAL:
            // Aqui você pode chamar o módulo de gerenciamento principal
            printf("Retornando ao menu principal...\n");
            estado_atual = ESTADO_SAIR; // Para sair do programa, remova esta linha para continuar no loop
            break;

        default:
            printf("Estado desconhecido!\n");
            estado_atual = ESTADO_SAIR;
            break;
        }
    }

    // Libera memória dos médicos
    free(medicos);

    return 0;
}
    */