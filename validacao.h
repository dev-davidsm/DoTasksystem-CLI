#pragma once
#include "lista.h"

void setData(int *dia, int *mes, int *ano);
void setHora(int dia, int mes, int ano, int *hora, int *minuto);
int estaAtrasada(TAREFA *t);
int descricaoValida(const char *texto);
int contemPalavra(const char *texto, const char *chave);