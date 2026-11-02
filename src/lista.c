#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

struct noLista {
    int id;
    struct noLista *prox;
};

struct lista {
    NoLista *inicio;
    NoLista *fim;
};

Lista* criar_lista() {
    Lista* li = (Lista*) malloc(sizeof(Lista));
    if(li != NULL) {
        li->inicio = NULL;
        li->fim = NULL;
    }
    return li;
}

int inserir_lista_final(Lista* li, int id) {
    if(li == NULL) return 0;

    NoLista* no = (NoLista*) malloc(sizeof(NoLista));
    if(no == NULL) return 0;

    no->id = id;
    no->prox = NULL;

    if(li->inicio == NULL) {
        li->inicio = no;
    } else {
        li->fim->prox = no;
    }
    li->fim = no;

    return 1;
}

void imprimir_lista(Lista* li) {
    if(li == NULL || li->inicio == NULL) {
        printf("Lista vazia.\n");
        return;
    }
    NoLista* aux = li->inicio;
    while(aux != NULL) {
        printf("%d ", aux->id);
        aux = aux->prox;
    }
    printf("\n");
}

void liberar_lista(Lista* li) {
    if(li != NULL) {
        NoLista* no;
        while(li->inicio != NULL) {
            no = li->inicio;
            li->inicio = li->inicio->prox;
            free(no);
        }
        free(li);
    }
}
