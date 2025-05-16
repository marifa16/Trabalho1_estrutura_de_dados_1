# Sistema de Gerenciamento XXX

Este projeto é um sistema de gerenciamento médico desenvolvido em C. Ele permite gerenciar consultas, pacientes, médicos e relatórios.

## Estrutura do Projeto 

- `src/`: Contém os arquivos de código-fonte.
- `include/`: Contém os arquivos de cabeçalho.
- `.vscode/`: Configurações do Visual Studio Code.

## Compilação

Para compilar o projeto, use o seguinte comando:

```bash
gcc -Iinclude -o programa src/*.c


chcp 65001

gcc -Wall -Wextra -g3 -I../include -o main.exe main.c mensagens.c menus.c modulo_consulta.c modulo_gerenciar_paciente.c modulo_gerenciar_medico.c modulo_relatorios.c

gcc -Wall -Wextra -g3 -I../include -o main.exe *.c

mkdir bin
gcc -Wall -Wextra -g3 -I../include -o bin/main.exe *.c

./main.exe