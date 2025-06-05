#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_CIDADES 100
#define NOME_TAM 50
#define INFINITO 999999

//rota
typedef struct Rota {
    int destino;
    int custo;
    struct Rota* prox;
} Rota;

//vértice
typedef struct Cidade {
    int id;
    char nome[NOME_TAM];
} Cidade;

//estrutura do grafo
typedef struct Grafo {
    Cidade cidades[MAX_CIDADES];
    Rota* adj[MAX_CIDADES]; //lista de adjacência
    int qtd_cidades;
} Grafo;

//criação e cadastro

Grafo* criarGrafo() {
    Grafo* g = (Grafo*)malloc(sizeof(Grafo));
    g->qtd_cidades = 0;
    for (int i = 0; i < MAX_CIDADES; i++) {
        g->adj[i] = NULL;
    }
    return g;
}

void cadastrarCidade(Grafo* g, char* nome) {
    if (g->qtd_cidades >= MAX_CIDADES) {
        printf("Limite de cidades atingido.\n");
        return;
    }
    g->cidades[g->qtd_cidades].id = g->qtd_cidades;
    strncpy(g->cidades[g->qtd_cidades].nome, nome, NOME_TAM);
    g->qtd_cidades++;
    printf("Cidade '%s' cadastrada com ID %d.\n", nome, g->qtd_cidades - 1);
}

void cadastrarRota(Grafo* g, int origem, int destino, int custo) {
    if (origem >= g->qtd_cidades || destino >= g->qtd_cidades) {
        printf("ID de cidade inválido.\n");
        return;
    }

    Rota* novaRota = (Rota*)malloc(sizeof(Rota));
    novaRota->destino = destino;
    novaRota->custo = custo;
    novaRota->prox = g->adj[origem];
    g->adj[origem] = novaRota;

    // Se for bidirecional
    Rota* rotaVolta = (Rota*)malloc(sizeof(Rota));
    rotaVolta->destino = origem;
    rotaVolta->custo = custo;
    rotaVolta->prox = g->adj[destino];
    g->adj[destino] = rotaVolta;
}

//Dijkstra

void dijkstra(Grafo* g, int origem, int destino) {
    int dist[MAX_CIDADES];
    int visitado[MAX_CIDADES] = {0};
    int anterior[MAX_CIDADES];
    
    for (int i = 0; i < g->qtd_cidades; i++) {
        dist[i] = INFINITO;
        anterior[i] = -1;
    }

    dist[origem] = 0;

    for (int i = 0; i < g->qtd_cidades; i++) {
        int u = -1;

        for (int j = 0; j < g->qtd_cidades; j++) {
            if (!visitado[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        if (dist[u] == INFINITO)
            break;

        visitado[u] = 1;

        Rota* adj = g->adj[u];
        while (adj) {
            int v = adj->destino;
            int peso = adj->custo;

            if (dist[u] + peso < dist[v]) {
                dist[v] = dist[u] + peso;
                anterior[v] = u;
            }
            adj = adj->prox;
        }
    }

    //resultado
    if (dist[destino] == INFINITO) {
        printf("Não existe caminho entre %s e %s.\n",
               g->cidades[origem].nome, g->cidades[destino].nome);
        return;
    }

    printf("\nCusto mínimo de %s para %s: %d\n",
           g->cidades[origem].nome, g->cidades[destino].nome, dist[destino]);

    //caminho inverso
    int caminho[MAX_CIDADES];
    int tam = 0;
    for (int at = destino; at != -1; at = anterior[at]) {
        caminho[tam++] = at;
    }

    printf("Caminho: ");
    for (int i = tam - 1; i >= 0; i--) {
        printf("%s", g->cidades[caminho[i]].nome);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

//menu 

void menu() {
    printf("\n==== SISTEMA DE ROTAS ====\n");
    printf("1. Cadastrar cidade\n");
    printf("2. Cadastrar rota\n");
    printf("3. Ver menor caminho (Dijkstra)\n");
    printf("0. Sair\n");
    printf("==========================\n");
    printf("Escolha uma opção: ");
}

//função principal 

int main() {
    Grafo* grafo = criarGrafo();
    int opcao, origem, destino, custo;
    char nome[NOME_TAM];

    while (1) {
        menu();
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Nome da cidade: ");
                fgets(nome, NOME_TAM, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                cadastrarCidade(grafo, nome);
                break;

            case 2:
                printf("ID da cidade origem: ");
                scanf("%d", &origem);
                printf("ID da cidade destino: ");
                scanf("%d", &destino);
                printf("Custo da rota: ");
                scanf("%d", &custo);
                cadastrarRota(grafo, origem, destino, custo);
                break;

            case 3:
                printf("ID da cidade origem: ");
                scanf("%d", &origem);
                printf("ID da cidade destino: ");
                scanf("%d", &destino);
                dijkstra(grafo, origem, destino);
                break;

            case 0:
                printf("Encerrando o sistema...\n");
                exit(0);

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}
