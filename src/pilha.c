#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilha.h"

typedef struct noPilha {
    char acao[100];
    struct noPilha *prox;
} NoPilha;

struct pilha {
    NoPilha *topo;
};

Pilha* criar_pilha() {
    Pilha* pi = (Pilha*) malloc(sizeof(Pilha));
    if(pi != NULL) {
        pi->topo = NULL;
    }
    return pi;
}

void liberar_pilha(Pilha* pi) {
    if(pi != NULL) {
        NoPilha* no;
        while(pi->topo != NULL) {
            no = pi->topo;
            pi->topo = pi->topo->prox;
            free(no);
        }
        free(pi);
    }
}

int push_historico(Pilha* pi, const char* acao) {
    if(pi == NULL) return 0;
    NoPilha* no = (NoPilha*) malloc(sizeof(NoPilha));
    if(no == NULL) return 0;

    strncpy(no->acao, acao, 99);
    no->acao[99] = '\0';
    no->prox = pi->topo;
    pi->topo = no;
    return 1;
}

int pop_historico(Pilha* pi) {
    if(pi == NULL || pi->topo == NULL) return 0;
    NoPilha* no = pi->topo;
    pi->topo = pi->topo->prox;
    free(no);
    return 1;
}

int peek_historico(Pilha* pi, char* destino) {
    if(pi == NULL || pi->topo == NULL) return 0;
    strcpy(destino, pi->topo->acao);
    return 1;
}

void imprimir_historico_5(Pilha* pi) {
    if(pi == NULL || pi->topo == NULL) {
        printf("Historico vazio.\n");
        return;
    }
    printf("\n--- ULTIMAS ACOES (HISTORICO) ---\n");
    NoPilha* aux = pi->topo;
    int cont = 0;
    while(aux != NULL && cont < 5) {
        printf("%d. %s\n", cont + 1, aux->acao);
        aux = aux->prox;
        cont++;
    }
    printf("---------------------------------\n");
}
