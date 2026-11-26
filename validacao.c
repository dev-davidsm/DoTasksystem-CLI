#include "validacao.h"
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include "lista.h"

// Funções Auxiliares
static int ehBissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

// Verifica se a data existe no calendário
static int dataValida(int dia, int mes, int ano) {
    if (ano < 1 || ano > 2100) {
        return 0;
    }

    if (mes < 1 || mes > 12) {
        return 0;
    }

    int diasNoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (ehBissexto(ano)) {
        diasNoMes[1] = 29;
    }

    if (dia < 1 || dia > diasNoMes[mes - 1]) {
        return 0;
    }

    return 1;
}

// Retorna 1 se a data for anterior a hoje, e 0 se for hoje ou no futuro
static int dataNoPassado(int dia, int mes, int ano) {
    time_t agora = time(NULL);          // Pega o tempo atual em segundos
    struct tm *hoje = localtime(&agora); // Converte para o fuso horário local

    int anoAtual = hoje->tm_year + 1900;
    int mesAtual = hoje->tm_mon + 1;
    int diaAtual = hoje->tm_mday;

    if (ano < anoAtual) return 1;
    if (ano > anoAtual) return 0;

    if (mes < mesAtual) return 1;
    if (mes > mesAtual) return 0;

    if (dia < diaAtual) return 1;

    return 0;
}


static int horaValida(int hora, int minuto) {
    return (hora >= 0 && hora <= 23) && (minuto >= 0 && minuto <= 59);
}

// Verifica se o horário já passou (apenas se a data escolhida for hoje)
static int horarioNoPassado(int dia, int mes, int ano, int hora, int minuto) {
    time_t agora = time(NULL);
    struct tm *hoje = localtime(&agora);
    
    int anoAtual = hoje->tm_year + 1900;
    int mesAtual = hoje->tm_mon + 1;
    int diaAtual = hoje->tm_mday;
    
    if (ano == anoAtual && mes == mesAtual && dia == diaAtual) {
        if (hora < hoje->tm_hour) return 1;
        if (hora == hoje->tm_hour && minuto <= hoje->tm_min) return 1;
    }
    
    return 0;
}



// Funções públixas

void setHora(int dia, int mes, int ano, int *hora, int *minuto) {
    int lidos;
    int buffer;
    int tem_lixo;
    
    while (1) {
        tem_lixo = 0;
        
        lidos = scanf("%d:%d", hora, minuto); // scanf retorna quantidade de valores lidos
        
        // Consome o restante da linha e verifica se há sujeira
        while ((buffer = getchar()) != '\n' && buffer != EOF) {
            if (buffer != ' ') {
                tem_lixo = 1;
            }
        }
        
        if (lidos != 2 || tem_lixo) {
            printf("Formato inválido! Digite no padrão hh:mm (24h): \n");
            continue;
        }
        
        // 2ª Validação: limites de 00 a 23 e 00 a 59
        if (!horaValida(*hora, *minuto)) {
            printf("Horário inválido! Use horas de 00 a 23 e minutos de 00 a 59: \n");
            continue;
        }
        
        // 3ª Validação: horário anterior ao atual caso o prazo seja hoje
        if (horarioNoPassado(dia, mes, ano, *hora, *minuto)) {
            printf("Esse horário já passou hoje! Digite um horário futuro: \n");
            continue;
        }
        
        break;
    }
}   

void setData(int *dia, int *mes, int *ano) {
    int lidos;
    int c;
    int tem_lixo;

    while (1) {
        tem_lixo = 0;

        lidos = scanf("%d/%d/%d", dia, mes, ano);

        while ((c = getchar()) != '\n' && c != EOF) {
            if (c != ' ') {
                tem_lixo = 1;
            }
        }

        // 1ª Validação: formato
        if (lidos != 3 || tem_lixo) {
            printf("Formato inválido! Digite no padrão dd/mm/aaaa: \n");
            continue;
        }

        // 2ª Validação: existência no calendário
        if (!dataValida(*dia, *mes, *ano)) {
            printf("Data inexistente! Tente novamente (dd/mm/aaaa): \n");
            continue;
        }

        // 3ª Validação: data no passado
        if (dataNoPassado(*dia, *mes, *ano)) {
            printf("O prazo não pode estar no passado! Digite uma data atual ou futura: \n");
            continue;
        }

        break;
    }
}

int estaAtrasada(TAREFA *t) {
    if (t->status == 'C' || t->status == 'c') {
        return 0;
    }

    // Pega a data/hora atual do computador em segundos
    time_t agora = time(NULL);

    // Monta uma struct tm com o prazo da tarefa
    struct tm prazo_tm = {0}; // Zera todos os campos para evitar lixo de memória
    prazo_tm.tm_mday = t->prazo.dia;
    prazo_tm.tm_mon  = t->prazo.mes - 1;     // Mês vai de 0 a 11
    prazo_tm.tm_year = t->prazo.ano - 1900;  // Ano contado a partir de 1900
    prazo_tm.tm_hour = t->prazo.hora;
    prazo_tm.tm_min  = t->prazo.minuto;
    prazo_tm.tm_sec  = 0;
    prazo_tm.tm_isdst = -1; // Deixa o sistema ajustar horário de verão se houver

    // 3. Converte o prazo da tarefa para segundos
    time_t prazo_segundos = mktime(&prazo_tm);

    // 4. Compara: se o prazo for menor que agora, já passou da hora!
    if (prazo_segundos < agora) {
        return 1; // Atrasada
    }
    
    return 0; // No prazo
}

int descricaoValida(const char *texto) {
    for (int i = 0; texto[i] != '\0'; i++) {
        if (!isspace((unsigned char)texto[i])) {
            return 1;
        }
    }
    return 0;
}

int contemPalavra(const char *texto, const char *chave) {
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