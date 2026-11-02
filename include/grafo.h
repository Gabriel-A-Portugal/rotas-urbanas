#ifndef GRAFO_H
#define GRAFO_H

#include "lista.h"
#include "fila.h"

#define MAX_VERTICES 100

typedef struct grafo Grafo;

Grafo* criar_grafo();
void liberar_grafo(Grafo* g);

int vertice_existe_grafo(Grafo* g, int id);
int inserir_vertice_grafo(Grafo* g, int id);
int remover_vertice_grafo(Grafo* g, int id);

int adicionar_aresta_grafo(Grafo* g, int id1, int id2);
int remover_aresta_grafo(Grafo* g, int id1, int id2);
int conexao_direta_grafo(Grafo* g, int id1, int id2);

Lista* bfs_grafo(Grafo* g, int id_origem);
int existe_caminho_grafo(Grafo* g, int id_origem, int id_destino);

#endif
