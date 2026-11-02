#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "rotas.h"
#include "bst.h"
#include "grafo.h"
#include "pilha.h"
#include "ordenacao.h"

// Trata leitura de inteiros positivos e limpa o buffer
static int ler_inteiro_positivo(const char* rotulo, int *val) {
    char buffer[100];
    printf("%s", rotulo);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;

    if (strchr(buffer, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    char *endptr;
    long num = strtol(buffer, &endptr, 10);
    if (endptr == buffer || (*endptr != '\n' && *endptr != '\0')) {
        printf("Erro: Entrada invalida. Digite um numero inteiro positivo.\n");
        return 0;
    }
    if (num <= 0) {
        printf("Erro: O ID deve ser um inteiro positivo maior que zero.\n");
        return 0;
    }
    *val = (int)num;
    return 1;
}

// Trata leitura de strings com até 79 caracteres úteis e descarta estouro de buffer
static int ler_string_segura(const char* rotulo, char* dest, int max_len) {
    char buffer[200];
    printf("%s", rotulo);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;

    char *newline = strchr(buffer, '\n');
    if (newline == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Erro: Texto excedeu o limite de %d caracteres.\n", max_len - 1);
        return 0;
    }
    *newline = '\0';

    // Validar string não vazia / apenas espaços
    int i = 0, apenas_espacos = 1;
    while (buffer[i] != '\0') {
        if (!isspace((unsigned char)buffer[i])) {
            apenas_espacos = 0;
            break;
        }
        i++;
    }
    if (apenas_espacos) {
        printf("Erro: O campo nao pode ser vazio ou conter apenas espacos.\n");
        return 0;
    }

    strncpy(dest, buffer, max_len - 1);
    dest[max_len - 1] = '\0';
    return 1;
}

int main() {
    ArvoreBST* bst = criar_bst();
    Grafo* grafo = criar_grafo();
    Pilha* historico = criar_pilha();

    int opcao = -1;

    while (opcao != 0) {
        printf("\n=========================================\n");
        printf("   SISTEMA DE ROTAS URBANAS (ED - FATEC) \n");
        printf("=========================================\n");
        printf(" 1 - Cadastrar registro (cria vertice)\n");
        printf(" 2 - Buscar registro (BST)\n");
        printf(" 3 - Remover registro e vertice/arestas\n");
        printf(" 4 - Listar registros (BST em-ordem)\n");
        printf(" 5 - Ordenar e exibir registros por nome\n");
        printf(" 6 - Adicionar conexao (aresta)\n");
        printf(" 7 - Remover conexao (aresta)\n");
        printf(" 8 - Consultar conexao direta\n");
        printf(" 9 - Listar alcancaveis (BFS + lista)\n");
        printf("10 - Verificar caminho entre dois IDs\n");
        printf("11 - Consultar historico (pilha)\n");
        printf(" 0 - Encerrar\n");
        printf("-----------------------------------------\n");

        if (!ler_inteiro_positivo("Escolha uma opcao: ", &opcao)) {
            continue;
        }

        switch (opcao) {
            case 1: {
                Rota r;
                if (!ler_inteiro_positivo("Digite o ID do local (positivo): ", &r.id)) break;

                if (buscar_bst(bst, r.id, NULL)) {
                    printf("Erro: Ja existe um local cadastrado com o ID %d.\n", r.id);
                    break;
                }

                if (!ler_string_segura("Nome do local: ", r.nome_local, 80)) break;
                if (!ler_string_segura("Categoria (ex: Parque, Hospital): ", r.categoria, 80)) break;
                if (!ler_string_segura("Bairro: ", r.bairro, 80)) break;

                // Passo 1: Inserir na BST
                if (!inserir_bst(bst, r)) {
                    printf("Erro ao inserir registro na BST.\n");
                    break;
                }

                // Passo 2: Inserir no Grafo
                if (!inserir_vertice_grafo(grafo, r.id)) {
                    // ROLLBACK: se o Grafo falhar (ex: limite de 100 vértices), desfaz inserção na BST
                    remover_bst(bst, r.id);
                    printf("ROLLBACK EXECUTADO: Cadastro cancelado e removido da BST para manter consistencia.\n");
                    break;
                }

                char msg_hist[100];
                snprintf(msg_hist, sizeof(msg_hist), "Cadastro do ID %d (%s)", r.id, r.nome_local);
                push_historico(historico, msg_hist);
                printf("Sucesso: Local cadastrado e vertice criado no Grafo!\n");
                break;
            }
            case 2: {
                int id;
                if (!ler_inteiro_positivo("Digite o ID a buscar: ", &id)) break;
                Rota r;
                if (buscar_bst(bst, id, &r)) {
                    printf("\n--- REGISTRO ENCONTRADO ---\n");
                    printf("ID: %d | Local: %s | Categoria: %s | Bairro: %s\n",
                           r.id, r.nome_local, r.categoria, r.bairro);
                } else {
                    printf("ID %d nao encontrado na BST.\n", id);
                }
                break;
            }
            case 3: {
                int id;
                if (!ler_inteiro_positivo("Digite o ID do registro a remover: ", &id)) break;

                if (!buscar_bst(bst, id, NULL)) {
                    printf("Erro: ID %d nao existe no sistema.\n", id);
                    break;
                }

                // Remove vértice e todas as arestas incidentes do Grafo
                remover_vertice_grafo(grafo, id);
                // Remove registro da BST
                remover_bst(bst, id);

                char msg_hist[100];
                snprintf(msg_hist, sizeof(msg_hist), "Remocao do ID %d", id);
                push_historico(historico, msg_hist);
                printf("Sucesso: Registro ID %d e suas conexoes foram removidos com sucesso.\n", id);
                break;
            }
            case 4:
                listar_em_ordem_bst(bst);
                break;
            case 5:
                ordenar_e_exibir_locais(bst);
                break;
            case 6: {
                int id1, id2;
                if (!ler_inteiro_positivo("ID do local de origem: ", &id1)) break;
                if (!ler_inteiro_positivo("ID do local de destino: ", &id2)) break;

                if (adicionar_aresta_grafo(grafo, id1, id2)) {
                    char msg_hist[100];
                    snprintf(msg_hist, sizeof(msg_hist), "Conexao adicionada (%d <-> %d)", id1, id2);
                    push_historico(historico, msg_hist);
                    printf("Sucesso: Conexao adicionada entre ID %d e ID %d.\n", id1, id2);
                }
                break;
            }
            case 7: {
                int id1, id2;
                if (!ler_inteiro_positivo("ID do primeiro local: ", &id1)) break;
                if (!ler_inteiro_positivo("ID do segundo local: ", &id2)) break;

                if (remover_aresta_grafo(grafo, id1, id2)) {
                    char msg_hist[100];
                    snprintf(msg_hist, sizeof(msg_hist), "Conexao removida (%d <-> %d)", id1, id2);
                    push_historico(historico, msg_hist);
                    printf("Sucesso: Conexao removida entre ID %d e ID %d.\n", id1, id2);
                }
                break;
            }
            case 8: {
                int id1, id2;
                if (!ler_inteiro_positivo("ID do primeiro local: ", &id1)) break;
                if (!ler_inteiro_positivo("ID do segundo local: ", &id2)) break;

                if (conexao_direta_grafo(grafo, id1, id2)) {
                    printf("Resultado: SIM, existe conexao direta entre ID %d e ID %d.\n", id1, id2);
                } else {
                    printf("Resultado: NAO existe conexao direta entre ID %d e ID %d.\n", id1, id2);
                }
                break;
            }
            case 9: {
                int id_origem;
                if (!ler_inteiro_positivo("ID do local de origem para a BFS: ", &id_origem)) break;

                if (!vertice_existe_grafo(grafo, id_origem)) {
                    printf("Erro: ID %d nao existe no grafo.\n", id_origem);
                    break;
                }

                Lista* alcancaveis = bfs_grafo(grafo, id_origem);
                printf("Locais alcancaveis a partir do ID %d (Ordem BFS): ", id_origem);
                imprimir_lista(alcancaveis);
                liberar_lista(alcancaveis);
                break;
            }
            case 10: {
                int id1, id2;
                if (!ler_inteiro_positivo("ID do local de origem: ", &id1)) break;
                if (!ler_inteiro_positivo("ID do local de destino: ", &id2)) break;

                if (!vertice_existe_grafo(grafo, id1) || !vertice_existe_grafo(grafo, id2)) {
                    printf("Erro: Um ou ambos os IDs nao existem no grafo.\n");
                    break;
                }

                if (existe_caminho_grafo(grafo, id1, id2)) {
                    printf("Resultado: SIM, existe um caminho ligando o ID %d ao ID %d.\n", id1, id2);
                } else {
                    printf("Resultado: NAO existe caminho ligando o ID %d ao ID %d.\n", id1, id2);
                }
                break;
            }
            case 11:
                imprimir_historico_5(historico);
                break;
            case 0:
                printf("Encerrando o sistema e liberando memoria...\n");
                break;
            default:
                printf("Opcao invalida. Digite um numero de 0 a 11.\n");
                break;
        }
    }

    // Liberação total da memória alocada ao encerrar
    liberar_bst(bst);
    liberar_grafo(grafo);
    liberar_pilha(historico);

    return 0;
}
