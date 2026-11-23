#pragma once

typedef struct DataHora {
    int dia, mes, ano;
    int hora, minuto;
}DataHora;

typedef struct TAREFA {
    int id;
    char descricao[100];
    // aqui vai guardar todas as informaçõe de data e hora da tarefa
    DataHora prazo;  
    char status;
    
    struct TAREFA *prox;
    struct TAREFA *ant;
}TAREFA;

void adicionarNaLista();
void removerNaLista(int pos);
void concluirTarefa(int id);
void listarTarefas();
void listarPorData(DataHora d);
void listarPorPalavraChave();
void carregarTarefas();
void salvarTarefas();
void liberarLista(); 
int contarTarefasAtrasadas();
void listarTarefasAtrasadas();