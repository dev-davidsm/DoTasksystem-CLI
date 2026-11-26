#pragma once
#include "lista.h"

void listarTarefas(TAREFA *aux);
void listarPorData(DataHora d, TAREFA *aux);
void listarPorPalavraChave(TAREFA *aux);
void listarTarefasAtrasadas(TAREFA *aux);

void exibirTitulo();
void exibirMenu(); 