#include <stdio.h>
#include <stdlib.h>
#include "../include/mensagens.h"

void msg_00_informacoes_programa()
{
    printf("Bem vindo ao sistema de gerenciamento médico\n");
    printf("Desenvolvido por:\n"
           "ANDERSON CARLOS DA SILVA MORAIS\n"
           "MARILIA FONSECA ANDRADE\n");
    printf("Versão 0.300.151 Alfa\n");
}

void msg_01_agendar_consulta()
{
    printf("===========================\n"
           "     Módulo Consultas      \n"
           "===========================\n"
           "Opções disponíveis:\n\n"
           "[1] - Agendar\n"
           "[2] - Cancelar\n"
           "[3] - Voltar\n"
           "[4] - Sair\n\n"
           "===========================\n");
}

void msg_02_opcao_invalida()
{
    printf("===========================\n"
           "Opção inválida.\n"
           "Por favor, escolha dentre as opções disponíveis.\n"
           "===========================\n");
}

void msg_03_finalizacao()
{
    printf("===========================\n"
           "Programa encerrado.\n"
           "===========================\n");
}

void msg_04_pergunta_medico()
{
    printf("===========================\n"
           "Está acessando como médico?\n\n"
           "[1] - Sim\n"
           "[2] - Não\n"
           "===========================\n");
}

void msg_05_retornando_menu()
{
    printf("===========================\n"
           "Retornando ao menu...\n"
           "===========================\n");
}

void msg_06_escolher_medico()
{
    printf("===========================\n"
           "Qual médico Sr.(a) deseja se consultar:\n\n"
           "===========================\n");
}

void msg_07_exibir_horarios()
{
    printf("===========================\n"
           "Escolha um horário disponível para a consulta.\n"
           "===========================\n");
}

void msg_08_sucesso_agendamento()
{
    printf("===========================\n"
           "Sua consulta foi marcada com sucesso!\n"
           "Sua consulta é no dia, no horário, com o médico.\n"
           "===========================\n");
}

void msg_09_desmarcar_consulta()
{
    printf("===========================\n"
           "Deseja desmarcar a consulta?\n\n"
           "[1] - Sim\n"
           "[2] - Não\n"
           "[3] - Sair\n"
           "===========================\n");
}

void msg_10_criar_paciente()
{
    printf("===========================\n"
           "Bem-vindo!\n"
           "Vamos criar seu cadastro.\n"
           "===========================\n");
}

void msg_11_nome_paciente()
{
    printf("===========================\n"
           "Insira seu nome completo:\n"
           "===========================\n");
}

void msg_12_validar_nome_paciente()
{
    printf("===========================\n"
           "Podemos seguir com o nome:\n"
           "===========================\n");
}

void msg_13_informar_cpf()
{
    printf("===========================\n"
           "Insira seu CPF: (Só os números)\n"
           "===========================\n");
}

void msg_14_informar_telefone()
{
    printf("===========================\n"
           "Insira telefone para contato:\n"
           "===========================\n");
}

void msg_15_sucesso_cadastro()
{
    printf("===========================\n"
           "Seu cadastro foi realizado com sucesso!\n"
           "===========================\n");
}

void msg_16_atualizar_paciente()
{
    printf("===========================\n"
           "Deseja atualizar qual informação:\n\n"
           "[1] - Nome\n"
           "[2] - CPF\n"
           "[3] - Telefone\n"
           "===========================\n");
}

void msg_17_atualizar_nome()
{
    printf("===========================\n"
           "Insira o novo nome:\n"
           "===========================\n");
}

void msg_18_novo_nome()
{
    printf("===========================\n"
           "Podemos seguir com o nome: *******?\n"
           "===========================\n");
}

void msg_19_novo_cpf()
{
    printf("===========================\n"
           "Insira seu novo CPF:\n"
           "===========================\n");
}

void msg_20_novo_telefone()
{
    printf("===========================\n"
           "Insira o novo telefone para contato:\n"
           "===========================\n");
}

void msg_21_validar_paciente()
{
    printf("===========================\n"
           "Deseja apagar cadastro deste paciente *******?\n\n"
           "[1] - Sim\n"
           "[2] - Não\n"
           "[3] - Sair\n"
           "===========================\n");
}

void msg_22_criar_medico()
{
    printf("===========================\n"
           "Bem-vindo!\n"
           "Vamos criar seu cadastro.\n"
           "===========================\n");
}

void msg_23_nome_medico()
{
    printf("===========================\n"
           "Insira seu nome completo:\n"
           "===========================\n");
}

void msg_24_validar_nome_medico()
{
    printf("===========================\n"
           "Podemos seguir com o nome:\n"
           "===========================\n");
}

void msg_25_atualizar_medico()
{
    printf("===========================\n"
           "Deseja atualizar qual informação:\n\n"
           "[1] - Nome\n"
           "[2] - CRM\n"
           "[3] - Telefone\n"
           "[4] - Especialidade\n"
           "===========================\n");
}

void msg_26_novo_crm()
{
    printf("===========================\n"
           "Insira seu novo CRM:\n"
           "===========================\n");
}

void msg_27_nova_especialidade()
{
    printf("===========================\n"
           "Insira a especialidade que deseja mudar:\n"
           "===========================\n");
}

void msg_28_validar_medico()
{
    printf("===========================\n"
           "Deseja apagar cadastro deste médico *******?\n\n"
           "[1] - Sim\n"
           "[2] - Não\n"
           "[3] - Sair\n"
           "===========================\n");
}

void msg_29_sucesso_cadastro_medico()
{
    printf("===========================\n"
           "Seu cadastro foi feito com sucesso.\n"
           "===========================\n");
}

void msg_30_crm()
{
    printf("===========================\n"
           "Informe seu CRM:\n"
           "===========================\n");
}

void msg_31_cpf()
{
    printf("===========================\n"
           "Informe seu CPF:\n"
           "===========================\n");
}

void msg_32_mudar_algo()
{
    printf("===========================\n"
           "Deseja mudar mais alguma coisa? (Sim/Não)\n"
           "===========================\n");
}

void msg_33_boas_vindas()
{
    printf("===========================\n"
           "      Módulo Principal      \n"
           "===========================\n"
           "O que deseja realizar hoje:\n\n"
           "[1] - Agendar Consulta\n"
           "[2] - Gerenciar Paciente\n"
           "[3] - Gerenciar Médico\n"
           "[4] - Módulo Relatório\n"
           "[5] - Sair\n"
           "===========================\n");
}

void msg_34_gerenciar()
{
    printf("===========================\n"
           "Gerenciar:\n\n"
           "[1] - Criar\n"
           "[2] - Exibir\n"
           "[3] - Atualizar\n"
           "[4] - Deletar\n"
           "[5] - Voltar\n"
           "[6] - Sair\n"
           "===========================\n");
}

void msg_35_relatorio()
{
    printf("===========================\n"
           "Módulo Relatório:\n\n"
           "[1] - Consultas de pacientes\n"
           "[2] - Consultas de médico\n"
           "[3] - Consulta de especialista\n"
           "[4] - Consultas do dia\n"
           "[5] - Voltar\n"
           "[6] - Sair\n"
           "===========================\n");
}

void msg_36_especialidade_consulta()
{
    printf("===========================\n"
           "Qual especialidade deseja consultar:\n\n"

           "===========================\n");
}

void msg_37_mostrar_modulo_paciente()
{
    printf("===========================\n"
           "      Módulo paciente      \n"
           "===========================\n"
           "[1] - Criar\n"
           "[2] - Exibir\n"
           "[3] - Atualizar\n"
           "[4] - Deletar\n"
           "[5] - Voltar\n"
           "[6] - Sair\n"
           "===========================\n");
}

void msg_38_mostrar_modulo_medico()
{
    printf("===========================\n"
           "        Módulo médico      \n"
           "===========================\n"
           "[1] - Criar\n"
           "[2] - Exibir\n"
           "[3] - Atualizar\n"
           "[4] - Deletar\n"
           "[5] - Voltar\n"
           "[6] - Sair\n"
           "===========================\n");
}

void msg_39_mostrar_modulo_relatorios()
{
    printf("===========================\n"
           "    Módulo Relatórios      \n"
           "===========================\n"
           "[1] - Consultas por paciente informado\n"
           "[2] - Consultas por médico informado\n"
           "[3] - Consulta por especialista\n"
           "[4] - Consultas do dia atual\n"
           "[5] - Relatório de consultas por especialidade\n"
           "[6] - Voltar\n"
           "[7] - Sair\n"
           "===========================\n");
}

void msg_40_opcoes()
{
    printf("Podemos seguir com a opção fornecida?\n"
           "[1] - SIM  \n"
           "[2] - NÃO  \n"
           "[3] - SAIR \n");
}

void msg_41_especialidade_medico()
{
    printf("===========================\n"
           "        Especialidade      \n"
           "===========================\n");
}

void clear()
{
    system("clear || cls");
}

void msg_42_especialidade_opcoes()
{
    printf("===========================\n"
           "Escolha a especialidade:\n"
           "[1] - CLINICO_GERAL\n"
           "[2] - PEDIATRA\n"
           "[3] - CARDIOLOGISTA\n"
           "[4] - DERMATOLOGISTA\n"
           "[5] - PSIQUIATRA\n"
           "[0] - Voltar\n"
           "===========================\n");
}