#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

struct grafo {
    int num_vertices;
    int ids[MAX_VERTICES];
    int matriz[MAX_VERTICES][MAX_VERTICES];
};

Grafo* criar_grafo() {
    Grafo* g = (Grafo*) malloc(sizeof(Grafo));
    if(g != NULL) {
        g->num_vertices = 0;
        for(int i = 0; i < MAX_VERTICES; i++) {
            g->ids[i] = -1;
            for(int j = 0; j < MAX_VERTICES; j++) {
                g->matriz[i][j] = 0;
            }
        }
    }
    return g;
}

void liberar_grafo(Grafo* g) {
    if(g != NULL) {
        free(g);
    }
}

static int id_para_indice(Grafo* g, int id) {
    if(g == NULL) return -1;
    for(int i = 0; i < g->num_vertices; i++) {
        if(g->ids[i] == id) return i;
    }
    return -1;
}

int vertice_existe_grafo(Grafo* g, int id) {
    return (id_para_indice(g, id) != -1);
}

int inserir_vertice_grafo(Grafo* g, int id) {
    if(g == NULL) return 0;

    if(g->num_vertices >= MAX_VERTICES) {
        printf("Erro: Capacidade maxima do grafo atingida (%d vertices).\n", MAX_VERTICES);
        return 0; // Provoca o Rollback
    }

    if(vertice_existe_grafo(g, id)) {
        return 0; // ID duplicado
    }

    int idx = g->num_vertices;
    g->ids[idx] = id;

    for(int j = 0; j < MAX_VERTICES; j++) {
        g->matriz[idx][j] = 0;
        g->matriz[j][idx] = 0;
    }

    g->num_vertices++;
    return 1;
}

int remover_vertice_grafo(Grafo* g, int id) {
    if(g == NULL) return 0;

    int idx = id_para_indice(g, id);
    if(idx == -1) return 0;

    // Shift no vetor de IDs e na matriz de adjacência
    for(int i = idx; i < g->num_vertices - 1; i++) {
        g->ids[i] = g->ids[i + 1];
    }

    for(int i = idx; i < g->num_vertices - 1; i++) {
        for(int j = 0; j < g->num_vertices; j++) {
            g->matriz[i][j] = g->matriz[i + 1][j];
        }
    }
    for(int j = idx; j < g->num_vertices - 1; j++) {
        for(int i = 0; i < g->num_vertices; i++) {
            g->matriz[i][j] = g->matriz[i][j + 1];
        }
    }

    g->num_vertices--;
    return 1;
}

int adicionar_aresta_grafo(Grafo* g, int id1, int id2) {
    if(g == NULL) return 0;

    if(id1 == id2) {
        printf("Erro: Nao sao permitidos lacos (conexao de um local com ele mesmo).\n");
        return 0;
    }

    int idx1 = id_para_indice(g, id1);
    int idx2 = id_para_indice(g, id2);

    if(idx1 == -1 || idx2 == -1) {
        printf("Erro: Um ou ambos os IDs de local nao existem no sistema.\n");
        return 0;
    }

    if(g->matriz[idx1][idx2] == 1) {
        printf("Erro: A conexao entre o ID %d e ID %d ja existe.\n", id1, id2);
        return 0;
    }

    g->matriz[idx1][idx2] = 1;
    g->matriz[idx2][idx1] = 1;
    return 1;
}

int remover_aresta_grafo(Grafo* g, int id1, int id2) {
    if(g == NULL) return 0;

    int idx1 = id_para_indice(g, id1);
    int idx2 = id_para_indice(g, id2);

    if(idx1 == -1 || idx2 == -1 || g->matriz[idx1][idx2] == 0) {
        printf("Erro: Conexao nao existente entre os IDs informados.\n");
        return 0;
    }

    g->matriz[idx1][idx2] = 0;
    g->matriz[idx2][idx1] = 0;
    return 1;
}

int conexao_direta_grafo(Grafo* g, int id1, int id2) {
    if(g == NULL) return 0;
    int idx1 = id_para_indice(g, id1);
    int idx2 = id_para_indice(g, id2);
    if(idx1 == -1 || idx2 == -1) return 0;
    return g->matriz[idx1][idx2];
}

Lista* bfs_grafo(Grafo* g, int id_origem) {
    if(g == NULL) return NULL;

    int idx_origem = id_para_indice(g, id_origem);
    if(idx_origem == -1) return NULL;

    Lista* ordem_descoberta = criar_lista();
    Fila* fila_bfs = criar_fila();
    int visitados[MAX_VERTICES] = {0};

    visitados[idx_origem] = 1;
    inserir_lista_final(ordem_descoberta, id_origem);
    enfileirar(fila_bfs, id_origem);

    while(!fila_vazia(fila_bfs)) {
        int id_atual;
        desenfileirar(fila_bfs, &id_atual);
        int idx_atual = id_para_indice(g, id_atual);

        // Coletar vizinhos e ordená-los por ID crescente (exigência do R08)
        int vizinhos_ids[MAX_VERTICES];
        int num_vizinhos = 0;

        for(int j = 0; j < g->num_vertices; j++) {
            if(g->matriz[idx_atual][j] == 1 && !visitados[j]) {
                vizinhos_ids[num_vizinhos++] = g->ids[j];
            }
        }

        // Ordenação simples (Bubble Sort) nos vizinhos para garantir ordem crescente de ID
        for(int i = 0; i < num_vizinhos - 1; i++) {
            for(int j = 0; j < num_vizinhos - i - 1; j++) {
                if(vizinhos_ids[j] > vizinhos_ids[j+1]) {
                    int temp = vizinhos_ids[j];
                    vizinhos_ids[j] = vizinhos_ids[j+1];
                    vizinhos_ids[j+1] = temp;
                }
            }
        }

        for(int i = 0; i < num_vizinhos; i++) {
            int idx_v = id_para_indice(g, vizinhos_ids[i]);
            visitados[idx_v] = 1;
            inserir_lista_final(ordem_descoberta, vizinhos_ids[i]);
            enfileirar(fila_bfs, vizinhos_ids[i]);
        }
    }

    liberar_fila(fila_bfs);
    return ordem_descoberta;
}

int existe_caminho_grafo(Grafo* g, int id_origem, int id_destino) {
    if(g == NULL) return 0;
    if(!vertice_existe_grafo(g, id_origem) || !vertice_existe_grafo(g, id_destino)) {
        return 0;
    }
    if(id_origem == id_destino) return 1;

    Lista* alcancaveis = bfs_grafo(g, id_origem);
    if(alcancaveis == NULL) return 0;

    int encontrou = 0;
    // Percorre a lista de alcançáveis para verificar se o destino está presente
    // A validação completa será realizada na integração final
    liberar_lista(alcancaveis);

    // BFS simplificada para teste direto de caminho
    Fila* q = criar_fila();
    int visitados[MAX_VERTICES] = {0};
    int idx_origem = id_para_indice(g, id_origem);

    visitados[idx_origem] = 1;
    enfileirar(q, id_origem);

    while(!fila_vazia(q)) {
        int curr;
        desenfileirar(q, &curr);
        if(curr == id_destino) {
            encontrou = 1;
            break;
        }
        int idx_c = id_para_indice(g, curr);
        for(int j = 0; j < g->num_vertices; j++) {
            if(g->matriz[idx_c][j] == 1 && !visitados[j]) {
                visitados[j] = 1;
                enfileirar(q, g->ids[j]);
            }
        }
    }
    liberar_fila(q);
    return encontrou;
}
