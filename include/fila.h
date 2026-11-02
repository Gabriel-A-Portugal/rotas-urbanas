#ifndef FILA_H
#define FILA_H

typedef struct fila Fila;

Fila* criar_fila();
void liberar_fila(Fila* fi);
int enfileirar(Fila* fi, int id);
int desenfileirar(Fila* fi, int *id);
int fila_vazia(Fila* fi);

#endif
