#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "validacao.h"
#include "config.h"


static TAREFA *inicio = NULL;
static TAREFA *fim = NULL;
static int tam = 0;
static int proximoId = 1;

// Funções privadas
static int gerarIdUnico() {
    return proximoId++;
}

static TAREFA *buscarPorId(int id) {
    TAREFA *aux = inicio;
    while (aux != NULL) {
        if (aux->id == id) {
            return aux;
        }
        aux = aux->prox;
    }
    return NULL;
}


// Funções públicas
TAREFA *getInicio() {
    return inicio;
}

void adicionarNaLista() {
    TAREFA *novo = malloc(sizeof(TAREFA));
    
    novo->id = gerarIdUnico();
    
    // Fluxo para criação da descrição
    while (1) {
        printf("Descrição: ");

        if (fgets(novo->descricao, sizeof(novo->descricao), stdin) == NULL) {
            free(novo);
            return;
        }

        // Se o texto excedeu o tamanho do vetor, limpa o restante do buffer
        if (strchr(novo->descricao, '\n') == NULL) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
        }

        // conta quantos caracteres existem até chegar em \n, o que coincide no indíce do caractere \n    
        novo->descricao[strcspn(novo->descricao, "\n")] = '\0';

        if (descricaoValida(novo->descricao)) {
            break;
        }

        printf("Descrição inválida! A tarefa não pode ficar vazia ou só com espaços.\n");
    }

    printf("Data: \n");
    setData(&novo->prazo.dia, &novo->prazo.mes, &novo->prazo.ano); 
    
    printf("Horário (hh:mm 24h): \n");
    setHora(novo->prazo.dia, novo->prazo.mes, novo->prazo.ano, &novo->prazo.hora, &novo->prazo.minuto);
    
    novo->status = 'P';
    
    novo->prox = NULL;
    novo->ant = NULL;
    
    if (inicio == NULL) {
        inicio = novo;
        fim = novo;
        tam++;
    } else {
        fim->prox = novo;
        novo->ant = fim;
        fim = novo;
        tam++;
    }
}

void carregarTarefas() {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "r");
    if (arquivo == NULL) {
        return; // Caso o arquivo não exista
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        TAREFA *novo = malloc(sizeof(TAREFA));
        if (novo == NULL) {
            printf("Erro ao alocar memória ao carregar tarefas!\n");
            break;
        }

        // Formato: ID|Descrição|YYYY-MM-DD|HH:MM|Status
        int lidos = sscanf(linha, "%d|%99[^|]|%d-%d-%d|%d:%d| %c",
                           &novo->id,
                           novo->descricao,
                           &novo->prazo.ano, &novo->prazo.mes, &novo->prazo.dia,
                           &novo->prazo.hora, &novo->prazo.minuto,
                           &novo->status);

        if (lidos != 8) {
            free(novo);
            continue;
        }

        atualizarStatus(novo); // Marca como 'A' se o prazo já venceu

        novo->prox = NULL;
        novo->ant = NULL;

        if (inicio == NULL) {
            inicio = novo;
            fim = novo;
        } else {
            fim->prox = novo;
            novo->ant = fim;
            fim = novo;
        }

        tam++;
        if (novo->id >= proximoId) {
            proximoId = novo->id + 1;
        }
    }

    fclose(arquivo);
}

void salvarTarefas() {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir %s para salvar os dados!\n", ARQUIVO_DADOS);
        return;
    }

    TAREFA *atual = inicio;
    while (atual != NULL) {
        // Remove quebras de linha e protege o delimitador '|'
        atual->descricao[strcspn(atual->descricao, "\r\n")] = '\0';
        sanitizarDescricao(atual->descricao);

        // Salva apenas 'C' ou 'P'; o estado 'A' é recalculado ao consultar
        char statusArquivo = (atual->status == 'C') ? 'C' : 'P';

        fprintf(arquivo, "%d|%s|%04d-%02d-%02d|%02d:%02d|%c\n",
                atual->id,
                atual->descricao,
                atual->prazo.ano, atual->prazo.mes, atual->prazo.dia,
                atual->prazo.hora, atual->prazo.minuto,
                statusArquivo);

        atual = atual->prox;
    }

    fclose(arquivo);
}

void removerNaLista(int id) {
    if (inicio == NULL) {
        printf("Lista vazia!\n");
        return;
    }

    TAREFA *aux = buscarPorId(id);

    if (aux == NULL) {
        printf("Tarefa com ID %d não encontrada!\n", id);
        return;
    }

    // Ajusta o nó anterior ou atualiza inicio
    if (aux->ant != NULL) {
        aux->ant->prox = aux->prox;
    } else {
        inicio = aux->prox;
    }

    // Ajusta o próximo nó ou atualiza fim
    if (aux->prox != NULL) {
        aux->prox->ant = aux->ant;
    } else {
        fim = aux->ant;
    }

    free(aux);
    tam--;
    printf("Tarefa %d removida com sucesso!\n", id);
}

void concluirTarefa(int id) {
    TAREFA *atual = buscarPorId(id);

    if (atual == NULL) {
        printf("Tarefa %d não encontrada.\n", id);
        return;
    }

    if (atual->status == 'C') {
        printf("A tarefa %d já estava concluída.\n", id);
    } else {
        atual->status = 'C';
        printf("Tarefa %d concluída com sucesso!\n", id);
    }
}

void liberarLista() {
    TAREFA *atual = inicio;
    while (atual != NULL) {
        TAREFA *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    inicio = NULL;
    fim = NULL;
    tam = 0;
}

int contarTarefasAtrasadas() {
    TAREFA *aux = inicio;
    int total = 0;

    while (aux != NULL) {
        atualizarStatus(aux);
        if (aux->status == 'A') {
            total++;
        }
        aux = aux->prox;
    }

    return total;
}
