#include <stdio.h>
#include "lista.h"
#include "config.h"
#include "impressao.h"

static void verificarLembreteInicial() {
    int atrasadas = contarTarefasAtrasadas();

    if (atrasadas > 0) {
        limparTela();
        exibirTitulo();
        printf("\n%s[!] LEMBRETE: Você possui %d tarefa(s) atrasada(s):%s\n\n",
               COR_VERMELHO, atrasadas, COR_RESET);
        listarTarefasAtrasadas(getInicio());
        aguardarEnter();
    }
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
                listarTarefas(getInicio());
                break;

            case 3:
                listarPorPalavraChave(getInicio());
                break;

            case 4: {
                DataHora dataBusca = lerDataBusca();
                listarPorData(dataBusca, getInicio());
                break;
            }

            case 5:
                listarTarefas(getInicio());
                id = lerInteiro("\nDigite o ID da tarefa para concluir (0 para cancelar): ");
                if (id > 0) {
                    concluirTarefa(id);
                } else {
                    printf("Operação cancelada.\n");
                }
                break;

            case 6:
                listarTarefas(getInicio());
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