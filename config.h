#pragma once
#include "lista.h"

#define ARQUIVO_DADOS "tarefas.txt"
#define COR_RESET    "\033[0m"
#define COR_VERMELHO "\033[1;31m"
#define COR_VERDE    "\033[1;32m"
#define COR_AMARELO  "\033[1;33m"

const char *corStatus(char status);
void atualizarStatus(TAREFA *t);
void sanitizarDescricao(char *desc);
void aguardarEnter();
void limparTela();
int limparBuffer();