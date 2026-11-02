#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ordenacao.h"

static void insertion_sort_locais(Rota* v, int n) {
    for (int i = 1; i < n; i++) {
        Rota chave = v[i];
        int j = i - 1;
        while (j >= 0) {
            int comp = strcmp(v[j].nome_local, chave.nome_local);
            // Ordenação alfabética por nome_local; em caso de empate, desempata por ID crescente
            if (comp > 0 || (comp == 0 && v[j].id > chave.id)) {
                v[j + 1] = v[j];
                j--;
            } else {
                break;
            }
        }
        v[j + 1] = chave;
    }
}

void ordenar_e_exibir_locais(ArvoreBST* raiz) {
    int total = 0;
    Rota* vetor = extrair_registros_bst(raiz, &total);

    if (vetor == NULL || total == 0) {
        printf("Nenhum registro cadastrado para ordenar.\n");
        return;
    }

    insertion_sort_locais(vetor, total);

    printf("\n--- REGISTROS ORDENADOS POR NOME DO LOCAL (INSERTION SORT) ---\n");
    for (int i = 0; i < total; i++) {
        printf("[%d] ID: %-5d | Local: %-25s | Categoria: %-15s | Bairro: %-15s\n",
               i + 1, vetor[i].id, vetor[i].nome_local, vetor[i].categoria, vetor[i].bairro);
    }
    printf("-------------------------------------------------------------\n");

    free(vetor); // Liberação da memória auxiliar alocada
}
