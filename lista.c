#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validacao.h"

#define ARQUIVO_DADOS "tarefas.txt"
#define COR_RESET    "\033[0m"
#define COR_VERMELHO "\033[1;31m"
#define COR_VERDE    "\033[1;32m"
#define COR_AMARELO  "\033[1;33m"

static TAREFA *inicio = NULL;
static TAREFA *fim = NULL;
static int tam = 0;
static int proximoId = 1;

// Funções privadas
static const char *corStatus(char status) {
    switch (status) {
        case 'C': return COR_VERDE;
        case 'A': return COR_VERMELHO;
        default:  return COR_AMARELO;
    }
}

static int gerarIdUnico() {
    return proximoId++;
}

static void sanitizarDescricao(char *desc) {
    for (int i = 0; desc[i] != '\0'; i++) {
        if (desc[i] == '|') {
            desc[i] = '-';
        }
    }
}

static void atualizarStatus(TAREFA *t) {
    if (t->status == 'P' && estaAtrasada(t)) {
        t->status = 'A';
    }
}

static int contemPalavra(const char *texto, const char *chave) {
    if (*chave == '\0') {
        return 0;
    }

    for (; *texto != '\0'; texto++) {
        const char *t = texto;
        const char *c = chave;

        while (*t != '\0' && *c != '\0' &&
               tolower((unsigned char)*t) == tolower((unsigned char)*c)) {
            t++;
            c++;
        }

        // Se chegou ao fim da palavra-chave, encontrou correspondência
        if (*c == '\0') {
            return 1;
        }
    }

    return 0;
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



// Funções públicas
void adicionarNaLista() {
    TAREFA *novo = malloc(sizeof(TAREFA));
    
    novo->id = gerarIdUnico();
    
    printf("Des: \n");
    fgets(novo->descricao, sizeof(novo->descricao), stdin);
    if (strchr(novo->descricao, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    }   
    novo->descricao[strcspn(novo->descricao, "\n")] = '\0';
    
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
        return; // Primeira execução: arquivo ainda não existe
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

        novo->status = toupper((unsigned char)novo->status);
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

void listarTarefas() {
    TAREFA *aux = inicio;
    while (aux != NULL) {
        exibirTarefa(aux);
        aux = aux->prox; 
    }
}

void listarPorPalavraChave() {
    if (inicio == NULL) {
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

    TAREFA *aux = inicio;
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

void listarPorData(DataHora d) {
    TAREFA *aux = inicio;
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

void listarTarefasAtrasadas() {
    TAREFA *aux = inicio;

    while (aux != NULL) {
        atualizarStatus(aux);
        if (aux->status == 'A') {
            printf("[%d] %s\n", aux->id, aux->descricao);
            printf("Prazo: %02d/%02d/%04d às %02d:%02d | Status: Atrasada\n\n",
                   aux->prazo.dia, aux->prazo.mes, aux->prazo.ano,
                   aux->prazo.hora, aux->prazo.minuto);
        }
        aux = aux->prox;
    }
}