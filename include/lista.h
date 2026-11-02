#ifndef LISTA_H
#define LISTA_H

typedef struct noLista NoLista;
typedef struct lista Lista;

Lista* criar_lista();
int inserir_lista_final(Lista* li, int id);
void imprimir_lista(Lista* li);
void liberar_lista(Lista* li);

#endif
