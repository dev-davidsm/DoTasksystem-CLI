#include "config.h"
#include <stdio.h>
#include "lista.h"
#include "validacao.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

const char *corStatus(char status) {
    switch (status) {
        case 'C': return COR_VERDE;
        case 'A': return COR_VERMELHO;
        default:  return COR_AMARELO;
    }
}

void atualizarStatus(TAREFA *t) {
    if (t->status == 'P' && estaAtrasada(t)) {
        t->status = 'A';
    }
}

void sanitizarDescricao(char *desc) {
    for (int i = 0; desc[i] != '\0'; i++) {
        if (desc[i] == '|') {
            desc[i] = '-';
        }
    }
}

void aguardarEnter() {
    printf("\nAperte ENTER para voltar ao menu inicial...");
    fflush(stdout);
#ifdef _WIN32
    int tecla;
    do {
        tecla = _getch();
    } while (tecla != '\r' && tecla != '\n');
#else
    struct termios antigo, novo;
    tcgetattr(STDIN_FILENO, &antigo);
    novo = antigo;
    novo.c_lflag &= ~(tcflag_t)(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &novo);

    int tecla;
    do {
        tecla = getchar();
    } while (tecla != '\n' && tecla != '\r' && tecla != EOF);

    tcsetattr(STDIN_FILENO, TCSANOW, &antigo);
#endif
    printf("\n");
}

void limparTela() {
    printf("\033[2J\033[H");
    fflush(stdout);
}

int limparBuffer() {
    int c;
    int temLixo = 0;

    while ((c = getchar()) != '\n' && c != EOF) {
        if (c != ' ') {
            temLixo = 1;
        }
    }

    return temLixo;
}