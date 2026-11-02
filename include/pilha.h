#ifndef PILHA_H
#define PILHA_H

typedef struct pilha Pilha;

Pilha* criar_pilha();
void liberar_pilha(Pilha* pi);
int push_historico(Pilha* pi, const char* acao);
int pop_historico(Pilha* pi);
int peek_historico(Pilha* pi, char* destino);
void imprimir_historico_5(Pilha* pi); // Exibe até 5 ações sem remover

#endif
