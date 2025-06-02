# Sistema de Gerenciamento Médico

Este projeto é um sistema de gerenciamento para clínicas ou consultórios médicos, desenvolvido em linguagem C. Ele permite o cadastro e gerenciamento de pacientes, médicos e consultas, além da geração de relatórios básicos. Os dados são persistidos em arquivos CSV.

## Funcionalidades Principais

O sistema é modularizado e oferece as seguintes funcionalidades:

1.  **Módulo de Consultas:**
    *   Agendar novas consultas, selecionando paciente, médico, data e horário.
    *   Cancelar consultas existentes (por médico ou paciente).

2.  **Módulo de Gerenciamento de Pacientes:**
    *   Criar novos registros de pacientes (nome, CPF, telefone).
    *   Exibir informações de um paciente específico (buscando por CPF).
    *   Atualizar dados cadastrais de pacientes.
    *   Deletar registros de pacientes.

3.  **Módulo de Gerenciamento de Médicos:**
    *   Criar novos registros de médicos (nome, CRM, especialidade, telefone).
    *   Exibir informações de um médico específico (buscando por CRM).
    *   Atualizar dados cadastrais de médicos.
    *   Deletar registros de médicos.

4.  **Módulo de Relatórios:**
    *   Listar todas as consultas de um paciente específico (buscando por CPF).
    *   Listar todas as consultas de um médico específico (buscando por CRM).
    *   Listar consultas por especialidade médica.
    *   Listar consultas agendadas para o dia atual.
    *   Exibir a quantidade total de consultas agrupadas por especialidade médica.

## Estrutura do Projeto

O projeto está organizado da seguinte forma:

-   `src/`: Contém os arquivos de código-fonte (`.c`) com a lógica dos módulos e funcionalidades.
    -   `main.c`: Ponto de entrada do programa e loop principal de controle de estados.
    -   `menu_principal.c`: Lógica do menu principal.
    -   `modulo_consulta.c`: Lógica para agendamento e cancelamento de consultas.
    -   `modulo_gerenciar_paciente.c`: Lógica para CRUD de pacientes.
    -   `modulo_gerenciar_medico.c`: Lógica para CRUD de médicos.
    -   `modulo_relatorios.c`: Lógica para geração de relatórios.
    -   `files_manager.c`: Funções para manipulação dos arquivos CSV (leitura, escrita, atualização, deleção).
    -   `auxiliar.c`: Funções utilitárias diversas (validações, limpeza de buffer, conversões, etc.).
    -   `mensagens.c`: Funções para exibição de mensagens e menus padronizados.
-   `include/`: Contém os arquivos de cabeçalho (`.h`) com as declarações de estruturas, funções e constantes.
    -   `estruturas.h`: Definição das principais estruturas de dados (paciente, médico, consulta, enums de estado e especialidade).
    -   `files_manager.h`: Cabeçalho para `files_manager.c`.
    -   `auxiliar.h`: Cabeçalho para `auxiliar.c`.
    -   `mensagens.h`: Cabeçalho para `mensagens.c`.
    -   `menu_principal.h`: Cabeçalho para `menu_principal.c`.
    -   `modulo_consulta.h`: Cabeçalho para `modulo_consulta.c`.
    -   `modulo_gerenciar_paciente.h`: Cabeçalho para `modulo_gerenciar_paciente.c`.
    -   `modulo_gerenciar_medico.h`: Cabeçalho para `modulo_gerenciar_medico.c`.
    -   `modulo_relatorios.h`: Cabeçalho para `modulo_relatorios.c`.
-   `data/`: Contém os arquivos CSV utilizados para persistência dos dados.
    -   `registro_pacientes.csv`: Armazena os dados dos pacientes (id_paciente, nome, cpf, telefone).
    -   `registro_medicos.csv`: Armazena os dados dos médicos (id_medico, nome, crm, especialidade, telefone).
    -   `registro_consultas.csv`: Armazena os dados das consultas (id_consulta, id_paciente, id_medico, data_hora, status).
-   `Makefile`: Arquivo para automatizar o processo de compilação (usando `mingw32-make`).
-   `bin/`: (Opcional, geralmente criado pela compilação) Contém o executável do programa.

## Tecnologias Utilizadas

-   Linguagem C
-   Arquivos CSV para persistência de dados

## Pré-requisitos

-   Compilador C (como o GCC, parte do MinGW para Windows)
-   Utilitário `make` (como `mingw32-make` para Windows)

## Compilação e Execução

Para compilar e executar o projeto, navegue até a pasta raiz do projeto (`Trabalho1_estrutura_de_dados_1`) pelo terminal e utilize o seguinte comando:

```bash
mingw32-make run
```
Este comando irá compilar os arquivos-fonte e, se a compilação for bem-sucedida, executará o programa `bin/main.exe`.

Se desejar apenas compilar, você pode usar:
```bash
mingw32-make
```
E para limpar os arquivos compilados:
```bash
mingw32-make clean
```

## Autores

-   ANDERSON CARLOS DA SILVA MORAIS
-   MARILIA FONSECA ANDRADE

## Versão

-   1.0 (Conforme `mensagens.c`)
```<!-- filepath: c:\Users\AnD_B\Desktop\github-projects\Trabalho1_estrutura_de_dados_1\README.md -->
# Sistema de Gerenciamento Médico

Este projeto é um sistema de gerenciamento para clínicas ou consultórios médicos, desenvolvido em linguagem C. Ele permite o cadastro e gerenciamento de pacientes, médicos e consultas, além da geração de relatórios básicos. Os dados são persistidos em arquivos CSV.

## Funcionalidades Principais

O sistema é modularizado e oferece as seguintes funcionalidades:

1.  **Módulo de Consultas:**
    *   Agendar novas consultas, selecionando paciente, médico, data e horário.
    *   Cancelar consultas existentes (por médico ou paciente).

2.  **Módulo de Gerenciamento de Pacientes:**
    *   Criar novos registros de pacientes (nome, CPF, telefone).
    *   Exibir informações de um paciente específico (buscando por CPF).
    *   Atualizar dados cadastrais de pacientes.
    *   Deletar registros de pacientes.

3.  **Módulo de Gerenciamento de Médicos:**
    *   Criar novos registros de médicos (nome, CRM, especialidade, telefone).
    *   Exibir informações de um médico específico (buscando por CRM).
    *   Atualizar dados cadastrais de médicos.
    *   Deletar registros de médicos.

4.  **Módulo de Relatórios:**
    *   Listar todas as consultas de um paciente específico (buscando por CPF).
    *   Listar todas as consultas de um médico específico (buscando por CRM).
    *   Listar consultas por especialidade médica.
    *   Listar consultas agendadas para o dia atual.
    *   Exibir a quantidade total de consultas agrupadas por especialidade médica.

## Estrutura do Projeto

O projeto está organizado da seguinte forma:

-   `src/`: Contém os arquivos de código-fonte (`.c`) com a lógica dos módulos e funcionalidades.
    -   `main.c`: Ponto de entrada do programa e loop principal de controle de estados.
    -   `menu_principal.c`: Lógica do menu principal.
    -   `modulo_consulta.c`: Lógica para agendamento e cancelamento de consultas.
    -   `modulo_gerenciar_paciente.c`: Lógica para CRUD de pacientes.
    -   `modulo_gerenciar_medico.c`: Lógica para CRUD de médicos.
    -   `modulo_relatorios.c`: Lógica para geração de relatórios.
    -   `files_manager.c`: Funções para manipulação dos arquivos CSV (leitura, escrita, atualização, deleção).
    -   `auxiliar.c`: Funções utilitárias diversas (validações, limpeza de buffer, conversões, etc.).
    -   `mensagens.c`: Funções para exibição de mensagens e menus padronizados.
-   `include/`: Contém os arquivos de cabeçalho (`.h`) com as declarações de estruturas, funções e constantes.
    -   `estruturas.h`: Definição das principais estruturas de dados (paciente, médico, consulta, enums de estado e especialidade).
    -   `files_manager.h`: Cabeçalho para `files_manager.c`.
    -   `auxiliar.h`: Cabeçalho para `auxiliar.c`.
    -   `mensagens.h`: Cabeçalho para `mensagens.c`.
    -   `menu_principal.h`: Cabeçalho para `menu_principal.c`.
    -   `modulo_consulta.h`: Cabeçalho para `modulo_consulta.c`.
    -   `modulo_gerenciar_paciente.h`: Cabeçalho para `modulo_gerenciar_paciente.c`.
    -   `modulo_gerenciar_medico.h`: Cabeçalho para `modulo_gerenciar_medico.c`.
    -   `modulo_relatorios.h`: Cabeçalho para `modulo_relatorios.c`.
-   `data/`: Contém os arquivos CSV utilizados para persistência dos dados.
    -   `registro_pacientes.csv`: Armazena os dados dos pacientes (id_paciente, nome, cpf, telefone).
    -   `registro_medicos.csv`: Armazena os dados dos médicos (id_medico, nome, crm, especialidade, telefone).
    -   `registro_consultas.csv`: Armazena os dados das consultas (id_consulta, id_paciente, id_medico, data_hora, status).
-   `Makefile`: Arquivo para automatizar o processo de compilação (usando `mingw32-make`).
-   `bin/`: (Opcional, geralmente criado pela compilação) Contém o executável do programa.

## Tecnologias Utilizadas

-   Linguagem C
-   Arquivos CSV para persistência de dados

## Pré-requisitos

-   Compilador C (como o GCC, parte do MinGW para Windows)
-   Utilitário `make` (como `mingw32-make` para Windows)

## Compilação e Execução

Para compilar e executar o projeto, navegue até a pasta raiz do projeto (`Trabalho1_estrutura_de_dados_1`) pelo terminal e utilize o seguinte comando:

```bash
mingw32-make run
```
Este comando irá compilar os arquivos-fonte e, se a compilação for bem-sucedida, executará o programa `bin/main.exe`.

Se desejar apenas compilar, você pode usar:
```bash
mingw32-make
```
E para limpar os arquivos compilados:
```bash
mingw32-make clean
```

## Autores

-   ANDERSON CARLOS DA SILVA MORAIS
-   MARILIA FONSECA ANDRADE

## Versão

-   1.0 (Conforme `mensagens.c`)