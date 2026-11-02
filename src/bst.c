#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

struct noBST {
    Rota dados;
    struct noBST *esq;
    struct noBST *dir;
};

ArvoreBST* criar_bst() {
    ArvoreBST* raiz = (ArvoreBST*) malloc(sizeof(ArvoreBST));
    if(raiz != NULL) {
        *raiz = NULL;
    }
    return raiz;
}

static void liberar_no(NoBST* no) {
    if(no == NULL) return;
    liberar_no(no->esq);
    liberar_no(no->dir);
    free(no);
}

void liberar_bst(ArvoreBST* raiz) {
    if(raiz == NULL) return;
    liberar_no(*raiz);
    free(raiz);
}

int bst_vazia(ArvoreBST* raiz) {
    if(raiz == NULL || *raiz == NULL) return 1;
    return 0;
}

int inserir_bst(ArvoreBST* raiz, Rota r) {
    if(raiz == NULL) return 0;

    NoBST* novo = (NoBST*) malloc(sizeof(NoBST));
    if(novo == NULL) return 0;

    novo->dados = r;
    novo->esq = NULL;
    novo->dir = NULL;

    if(*raiz == NULL) {
        *raiz = novo;
        return 1;
    }

    NoBST* atual = *raiz;
    NoBST* ant = NULL;

    while(atual != NULL) {
        ant = atual;
        if(r.id == atual->dados.id) {
            free(novo); // ID duplicado é rejeitado
            return 0;
        }
        if(r.id > atual->dados.id) {
            atual = atual->dir;
        } else {
            atual = atual->esq;
        }
    }

    if(r.id > ant->dados.id) {
        ant->dir = novo;
    } else {
        ant->esq = novo;
    }
    return 1;
}

static NoBST* procurar_menor(NoBST* atual) {
    NoBST* no1 = atual;
    NoBST* no2 = atual->esq;
    while(no2 != NULL) {
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

static int remover_no(ArvoreBST* raiz, int id) {
    if(raiz == NULL || *raiz == NULL) return 0;

    NoBST* ant = NULL;
    NoBST* atual = *raiz;

    while(atual != NULL) {
        if(id == atual->dados.id) {
            // Trata os 3 casos de remoção exigidos:
            // Caso 1 e 2: Folha (0 filhos) ou apenas 1 filho
            if(atual->esq == NULL || atual->dir == NULL) {
                NoBST* temp = (atual->esq != NULL) ? atual->esq : atual->dir;

                if(ant == NULL) { // Nó a remover era a raiz
                    *raiz = temp;
                } else if(ant->esq == atual) {
                    ant->esq = temp;
                } else {
                    ant->dir = temp;
                }
                free(atual);
            }
            // Caso 3: Possui 2 filhos
            else {
                NoBST* temp = procurar_menor(atual->dir);
                atual->dados = temp->dados;
                remover_no(&(atual->dir), temp->dados.id);
            }
            return 1;
        }
        ant = atual;
        if(id > atual->dados.id) {
            atual = atual->dir;
        } else {
            atual = atual->esq;
        }
    }
    return 0; // ID não encontrado
}

int remover_bst(ArvoreBST* raiz, int id) {
    if(raiz == NULL || *raiz == NULL) return 0;
    return remover_no(raiz, id);
}

int buscar_bst(ArvoreBST* raiz, int id, Rota* r) {
    if(raiz == NULL || *raiz == NULL) return 0;

    NoBST* atual = *raiz;
    while(atual != NULL) {
        if(id == atual->dados.id) {
            if(r != NULL) *r = atual->dados;
            return 1;
        }
        if(id > atual->dados.id) {
            atual = atual->dir;
        } else {
            atual = atual->esq;
        }
    }
    return 0;
}

static void em_ordem(NoBST* raiz) {
    if(raiz == NULL) return;
    em_ordem(raiz->esq);
    printf("ID: %-5d | Local: %-25s | Categoria: %-15s | Bairro: %-15s\n",
           raiz->dados.id, raiz->dados.nome_local, raiz->dados.categoria, raiz->dados.bairro);
    em_ordem(raiz->dir);
}

void listar_em_ordem_bst(ArvoreBST* raiz) {
    if(raiz == NULL || *raiz == NULL) {
        printf("Arvore BST vazia.\n");
        return;
    }
    printf("\n--- REGISTROS CADASTRADOS (BST EM-ORDEM) ---\n");
    em_ordem(*raiz);
    printf("--------------------------------------------\n");
}

int obter_menor_id_bst(ArvoreBST* raiz, int *menor_id) {
    if(raiz == NULL || *raiz == NULL) return 0;
    NoBST* atual = *raiz;
    while(atual->esq != NULL) {
        atual = atual->esq;
    }
    *menor_id = atual->dados.id;
    return 1;
}

int obter_maior_id_bst(ArvoreBST* raiz, int *maior_id) {
    if(raiz == NULL || *raiz == NULL) return 0;
    NoBST* atual = *raiz;
    while(atual->dir != NULL) {
        atual = atual->dir;
    }
    *maior_id = atual->dados.id;
    return 1;
}

static int contar_nos(NoBST* no) {
    if(no == NULL) return 0;
    return 1 + contar_nos(no->esq) + contar_nos(no->dir);
}

static void preencher_vetor(NoBST* no, Rota* vetor, int* idx) {
    if(no == NULL) return;
    preencher_vetor(no->esq, vetor, idx);
    vetor[*idx] = no->dados;
    (*idx)++;
    preencher_vetor(no->dir, vetor, idx);
}

Rota* extrair_registros_bst(ArvoreBST* raiz, int *total) {
    if(raiz == NULL || *raiz == NULL) {
        *total = 0;
        return NULL;
    }
    int qtd = contar_nos(*raiz);
    Rota* vetor = (Rota*) malloc(qtd * sizeof(Rota));
    if(vetor == NULL) {
        *total = 0;
        return NULL;
    }
    int idx = 0;
    preencher_vetor(*raiz, vetor, &idx);
    *total = qtd;
    return vetor;
}
