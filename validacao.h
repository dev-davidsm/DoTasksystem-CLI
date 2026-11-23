#pragma once
#include "lista.h"

char setStatus();
void setData(int *dia, int *mes, int *ano);
void setHora(int dia, int mes, int ano, int *hora, int *minuto);
int estaAtrasada(TAREFA *t);