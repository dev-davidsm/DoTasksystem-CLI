#include <stdio.h>
#include "lista.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#define COR_RESET    "\033[0m"
#define COR_VERMELHO "\033[1;31m"

static void limparTela() {
    printf("\033[2J\033[H");
    fflush(stdout);
}

static void exibirTitulo() {
    printf("\n=================================\n");
    printf("     GERENCIADOR DE TAREFAS      \n");
    printf("=================================\n");
}

static void aguardarEnter() {
    printf("\nAperte ENTER para voltar ao menu inicial...");
    fflush(stdout);
#ifdef _WIN32
    int tecla;
    do {
        tecla = _getch();
    } while (tecla != '\r' && tecla != '\n');
#else
    struct termios antigo, novo;
    tcgetattr(STDIN_FILENO, &antigo);
    novo = antigo;
    novo.c_lflag &= ~(tcflag_t)(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &novo);

    int tecla;
    do {
        tecla = getchar();
    } while (tecla != '\n' && tecla != '\r' && tecla != EOF);

    tcsetattr(STDIN_FILENO, TCSANOW, &antigo);
#endif
    printf("\n");
}

static void verificarLembreteInicial() {
    int atrasadas = contarTarefasAtrasadas();

    if (atrasadas > 0) {
        limparTela();
        exibirTitulo();
        printf("\n%s[!] LEMBRETE: Você possui %d tarefa(s) atrasada(s):%s\n\n",
               COR_VERMELHO, atrasadas, COR_RESET);
        listarTarefasAtrasadas();
        aguardarEnter();
    }
}

static int limparBuffer() {
    int c;
    int temLixo = 0;

    while ((c = getchar()) != '\n' && c != EOF) {
        if (c != ' ') {
            temLixo = 1;
        }
    }

    return temLixo;
}

static int lerInteiro(const char *mensagem) {
    int valor;

    while (1) {
        printf("%s", mensagem);
        int lidos = scanf("%d", &valor);
        int temLixo = limparBuffer();

        if (lidos == 1 && !temLixo) {
            return valor;
        }

        printf("Entrada inválida! Digite apenas um número inteiro.\n");
    }
}

static DataHora lerDataBusca() {
    DataHora d = {0};

    while (1) {
        printf("Digite a data para busca (dd/mm/aaaa): ");
        int lidos = scanf("%d/%d/%d", &d.dia, &d.mes, &d.ano);
        int temLixo = limparBuffer();

        if (lidos == 3 && !temLixo &&
            d.dia >= 1 && d.dia <= 31 &&
            d.mes >= 1 && d.mes <= 12 &&
            d.ano >= 1900) {
            return d;
        }

        printf("Data inválida! Use o formato dd/mm/aaaa.\n");
    }
}

static void exibirMenu() {
    printf("\n=================================\n");
    printf("     GERENCIADOR DE TAREFAS      \n");
    printf("=================================\n");
    printf("1 - Adicionar tarefa\n");
    printf("2 - Listar todas as tarefas\n");
    printf("3 - Buscar por palavra-chave\n");
    printf("4 - Buscar por data\n");
    printf("5 - Concluir tarefa\n");
    printf("6 - Remover tarefa\n");
    printf("0 - Salvar e sair\n");
    printf("=================================\n");
}

int main() {
    int opcao;
    int id;

    carregarTarefas();
    verificarLembreteInicial();

    do {
        limparTela();
        exibirMenu();
        opcao = lerInteiro("Escolha uma opção: ");

        if (opcao >= 1 && opcao <= 6) {
            limparTela();
        }

        switch (opcao) {
            case 1:
                adicionarNaLista();
                break;

            case 2:
                listarTarefas();
                break;

            case 3:
                listarPorPalavraChave();
                break;

            case 4: {
                DataHora dataBusca = lerDataBusca();
                listarPorData(dataBusca);
                break;
            }

            case 5:
                listarTarefas();
                id = lerInteiro("\nDigite o ID da tarefa para concluir (0 para cancelar): ");
                if (id > 0) {
                    concluirTarefa(id);
                } else {
                    printf("Operação cancelada.\n");
                }
                break;

            case 6:
                listarTarefas();
                id = lerInteiro("\nDigite o ID da tarefa para remover (0 para cancelar): ");
                if (id > 0) {
                    removerNaLista(id);
                } else {
                    printf("Operação cancelada.\n");
                }
                break;

            case 0:
                salvarTarefas();
                liberarLista();
                printf("\nAlterações salvas em tarefas.txt. Sistema encerrado.\n");
                break;

            default:
                printf("\nOpção inválida! Escolha entre 0 e 6.\n");
                break;
        }

        if (opcao != 0) {
            aguardarEnter();
        }
    } while (opcao != 0);

    return 0;
}