#include "impressao.h"
#include <stdio.h>
#include <string.h>
#include "lista.h"
#include "config.h"
#include "validacao.h"


// Funções privadas
static const char *textoStatus(char status) {
    switch (status) {
        case 'C': return "Concluída";
        case 'A': return "Atrasada";
        default:  return "Pendente";
    }
}

static void exibirTarefa(TAREFA *t) {
    atualizarStatus(t);
    const char *cor = corStatus(t->status);

    printf("%s[%d]%s %s\n", cor, t->id, COR_RESET, t->descricao);
    printf("Prazo: %02d/%02d/%04d às %02d:%02d | Status: %s%s%s\n\n",
           t->prazo.dia, t->prazo.mes, t->prazo.ano,
           t->prazo.hora, t->prazo.minuto,
           cor, textoStatus(t->status), COR_RESET);
}

// Funções Públicas


// FUNÇÕES DE LISTAGEM DE TAREFAS
void listarTarefas(TAREFA *aux) {
    if (aux == NULL) {
        printf("\nNenhuma tarefa cadastrada.\n");
        return;
    }

    while (aux != NULL) {
        exibirTarefa(aux);
        aux = aux->prox; 
    }
}

void listarTarefasAtrasadas(TAREFA *aux) {
    while (aux != NULL) {
        atualizarStatus(aux);
        if (aux->status == 'A') {
            exibirTarefa(aux);
        }
        aux = aux->prox;
    }
}

void listarPorData(DataHora d, TAREFA *aux) {
    int trfEncontrada = 0;

    while (aux != NULL) {
        if (aux->prazo.ano == d.ano && aux->prazo.mes == d.mes && aux->prazo.dia == d.dia) {
            trfEncontrada++;

            exibirTarefa(aux);
        }
        aux = aux->prox;
    }

    if (trfEncontrada == 0) {
        printf("Nenhuma tarefa encontrada para essa data!\n");
    } else {
        printf("Total encontrado: %d tarefa(s).\n", trfEncontrada);
    }
    
}

void listarPorPalavraChave(TAREFA *aux) {
    if (aux == NULL) {
        printf("\nNenhuma tarefa cadastrada.\n");
        return;
    }

    char chave[100];
    printf("\nDigite a palavra-chave para buscar: ");

    if (fgets(chave, sizeof(chave), stdin) == NULL) {
        return;
    }

    // Se a entrada excedeu o tamanho do vetor, limpa o restante do buffer
    if (strchr(chave, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    }

    // Remove o '\n' do final da palavra-chave
    chave[strcspn(chave, "\n")] = '\0';

    if (chave[0] == '\0') {
        printf("Busca cancelada: nenhuma palavra-chave informada.\n");
        return;
    }

    int encontradas = 0;

    printf("\n=== RESULTADOS PARA \"%s\" ===\n", chave);
    while (aux != NULL) {
        if (contemPalavra(aux->descricao, chave)) {
            
            exibirTarefa(aux);
            encontradas++;
        }
        aux = aux->prox;
    }

    if (encontradas == 0) {
        printf("Nenhuma tarefa encontrada com \"%s\".\n", chave);
    } else {
        printf("Total encontrado: %d tarefa(s).\n", encontradas);
    }
}

// IMPRESSÕES GERAIS
void exibirTitulo() {
    printf("\n=================================\n");
    printf("     GERENCIADOR DE TAREFAS      \n");
    printf("=================================\n");
}

void exibirMenu() {
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