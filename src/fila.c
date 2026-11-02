#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

typedef struct noFila {
    int id;
    struct noFila *prox;
} NoFila;

struct fila {
    NoFila *inicio;
    NoFila *fim;
};

Fila* criar_fila() {
    Fila* fi = (Fila*) malloc(sizeof(Fila));
    if(fi != NULL) {
        fi->inicio = NULL;
        fi->fim = NULL;
    }
    return fi;
}

void liberar_fila(Fila* fi) {
    if(fi != NULL) {
        NoFila* no;
        while(fi->inicio != NULL) {
            no = fi->inicio;
            fi->inicio = fi->inicio->prox;
            free(no);
        }
        free(fi);
    }
}

int enfileirar(Fila* fi, int id) {
    if(fi == NULL) return 0;
    NoFila* no = (NoFila*) malloc(sizeof(NoFila));
    if(no == NULL) return 0;

    no->id = id;
    no->prox = NULL;

    if(fi->inicio == NULL) {
        fi->inicio = no;
    } else {
        fi->fim->prox = no;
    }
    fi->fim = no;
    return 1;
}

int desenfileirar(Fila* fi, int *id) {
    if(fi == NULL || fi->inicio == NULL) return 0;

    NoFila* no = fi->inicio;
    *id = no->id;
    fi->inicio = fi->inicio->prox;

    if(fi->inicio == NULL) {
        fi->fim = NULL;
    }
    free(no);
    return 1;
}

int fila_vazia(Fila* fi) {
    if(fi == NULL || fi->inicio == NULL) return 1;
    return 0;
}
