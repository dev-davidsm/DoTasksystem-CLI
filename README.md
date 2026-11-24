# Do Task System

Sistema gerenciador de tarefas individuais que permite ao usuário controlar atividades do seu cotidiano, centralizando todas elas em um único sistema.

## Pré-requisitos

Para compilar e executar o projeto, certifique-se de ter as seguintes ferramentas instaladas em seu ambiente (Linux, macOS ou WSL/MinGW no Windows):

- **GCC** (compilador C com suporte ao padrão C11)
- **Make** (ferramenta de automação de compilação)

## Estrutura do Projeto

Com base na configuração de compilação, o projeto está organizado nos seguintes módulos:

```text
.
├── app.c          # Ponto de entrada da aplicação
├── lista.c        # Implementação da estrutura e operações da lista de tarefas
├── lista.h        # Cabeçalho da lista de tarefas
├── validacao.c    # Implementação das rotinas de validação
├── validacao.h    # Cabeçalho das rotinas de validação
└── Makefile       # Script de automação de compilação e execução
```

## Como Compilar e Rodar o Projeto

O projeto utiliza um `Makefile` configurado com o compilador `gcc` (padrão `C11` e flags de aviso `-Wall -Wextra -Wpedantic -g`).

Abra o terminal na pasta raiz do projeto e utilize os comandos abaixo:

### 1. Compilar e executar diretamente
Para compilar o projeto (caso ainda não tenha sido compilado ou haja alterações) e iniciar o programa em seguida:

```bash
make run
```

### 2. Apenas compilar
Se desejar apenas gerar o executável (`tarefas`) sem rodá-lo imediatamente:

```bash
make
```
*(ou `make all`)*

Depois de compilado, você pode executar o programa manualmente com:

```bash
./tarefas
```

### 3. Limpar arquivos gerados
Para remover os arquivos objeto (`.o`) e o executável (`tarefas`), deixando o diretório limpo para uma nova compilação do zero:

```bash
make clean
```