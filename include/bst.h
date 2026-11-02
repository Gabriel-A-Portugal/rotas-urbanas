#ifndef BST_H
#define BST_H

#include "rotas.h"

typedef struct noBST NoBST;
typedef NoBST* ArvoreBST;

ArvoreBST* criar_bst();
void liberar_bst(ArvoreBST* raiz);
int bst_vazia(ArvoreBST* raiz);

int inserir_bst(ArvoreBST* raiz, Rota r);
int remover_bst(ArvoreBST* raiz, int id);
int buscar_bst(ArvoreBST* raiz, int id, Rota* r);

void listar_em_ordem_bst(ArvoreBST* raiz);
int obter_menor_id_bst(ArvoreBST* raiz, int *menor_id);
int obter_maior_id_bst(ArvoreBST* raiz, int *maior_id);

#endif
