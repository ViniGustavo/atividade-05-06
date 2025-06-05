#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USUARIOS 100
#define TAM_NOME 50

//estruturas

typedef struct Usuario {
    int id;
    char nome[TAM_NOME];
} Usuario;

typedef struct No {
    int id_amigo;
    struct No* prox;
} No;

typedef struct {
    Usuario usuarios[MAX_USUARIOS];
    No* lista_adj[MAX_USUARIOS];  
    int qtd_usuarios;
} Grafo;

//manipulação grafo

Grafo* criarGrafo() {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->qtd_usuarios = 0;
    for (int i = 0; i < MAX_USUARIOS; i++) {
        grafo->lista_adj[i] = NULL;
    }
    return grafo;
}

void adicionarUsuario(Grafo* grafo, char* nome) {
    if (grafo->qtd_usuarios >= MAX_USUARIOS) {
        printf("Limite máximo de usuários atingido.\n");
        return;
    }

    grafo->usuarios[grafo->qtd_usuarios].id = grafo->qtd_usuarios;
    strncpy(grafo->usuarios[grafo->qtd_usuarios].nome, nome, TAM_NOME);
    grafo->lista_adj[grafo->qtd_usuarios] = NULL;
    grafo->qtd_usuarios++;
    printf("Usuário '%s' adicionado com ID %d.\n", nome, grafo->qtd_usuarios - 1);
}

void adicionarAmizade(Grafo* grafo, int id1, int id2) {
    if (id1 >= grafo->qtd_usuarios || id2 >= grafo->qtd_usuarios || id1 == id2) {
        printf("IDs de usuários inválidos.\n");
        return;
    }

    No* novo1 = (No*)malloc(sizeof(No));
    novo1->id_amigo = id2;
    novo1->prox = grafo->lista_adj[id1];
    grafo->lista_adj[id1] = novo1;

    No* novo2 = (No*)malloc(sizeof(No));
    novo2->id_amigo = id1;
    novo2->prox = grafo->lista_adj[id2];
    grafo->lista_adj[id2] = novo2;

    printf("Amizade criada entre '%s' e '%s'.\n", grafo->usuarios[id1].nome, grafo->usuarios[id2].nome);
}

void mostrarAmigos(Grafo* grafo, int id) {
    if (id >= grafo->qtd_usuarios) {
        printf("Usuário não encontrado.\n");
        return;
    }

    printf("Amigos de %s:\n", grafo->usuarios[id].nome);
    No* temp = grafo->lista_adj[id];
    if (!temp) {
        printf("Nenhum amigo cadastrado.\n");
        return;
    }

    while (temp) {
        printf(" - %s (ID: %d)\n", grafo->usuarios[temp->id_amigo].nome, temp->id_amigo);
        temp = temp->prox;
    }
}

//bfs

void buscaLargura(Grafo* grafo, int inicio) {
    int visitado[MAX_USUARIOS] = {0};
    int fila[MAX_USUARIOS];
    int frente = 0, fim = 0;

    visitado[inicio] = 1;
    fila[fim++] = inicio;

    printf("\nExploração em largura (BFS) a partir de %s:\n", grafo->usuarios[inicio].nome);

    while (frente < fim) {
        int atual = fila[frente++];
        printf("Visitando: %s\n", grafo->usuarios[atual].nome);

        No* temp = grafo->lista_adj[atual];
        while (temp) {
            if (!visitado[temp->id_amigo]) {
                visitado[temp->id_amigo] = 1;
                fila[fim++] = temp->id_amigo;
            }
            temp = temp->prox;
        }
    }
}

//dfs

void dfsRecursiva(Grafo* grafo, int atual, int visitado[]) {
    visitado[atual] = 1;
    printf("Visitando: %s\n", grafo->usuarios[atual].nome);

    No* temp = grafo->lista_adj[atual];
    while (temp) {
        if (!visitado[temp->id_amigo]) {
            dfsRecursiva(grafo, temp->id_amigo, visitado);
        }
        temp = temp->prox;
    }
}

void buscaProfundidade(Grafo* grafo, int inicio) {
    int visitado[MAX_USUARIOS] = {0};
    printf("\nExploração em profundidade (DFS) a partir de %s:\n", grafo->usuarios[inicio].nome);
    dfsRecursiva(grafo, inicio, visitado);
}

//sugestão de amigos

void sugerirAmigos(Grafo* grafo, int id) {
    int diretos[MAX_USUARIOS] = {0};
    int sugeridos[MAX_USUARIOS] = {0};

    No* temp = grafo->lista_adj[id];
    while (temp) {
        diretos[temp->id_amigo] = 1;
        temp = temp->prox;
    }

    temp = grafo->lista_adj[id];
    while (temp) {
        No* amigoDoAmigo = grafo->lista_adj[temp->id_amigo];
        while (amigoDoAmigo) {
            if (amigoDoAmigo->id_amigo != id &&
                !diretos[amigoDoAmigo->id_amigo] &&
                !sugeridos[amigoDoAmigo->id_amigo]) {
                
                printf("Sugestão de amizade para %s: %s (ID: %d)\n",
                       grafo->usuarios[id].nome,
                       grafo->usuarios[amigoDoAmigo->id_amigo].nome,
                       amigoDoAmigo->id_amigo);
                sugeridos[amigoDoAmigo->id_amigo] = 1;
            }
            amigoDoAmigo = amigoDoAmigo->prox;
        }
        temp = temp->prox;
    }
}

//menu

void mostrarMenu() {
    printf("\n====== REDE SOCIAL ======\n");
    printf("1. Adicionar usuário\n");
    printf("2. Criar amizade\n");
    printf("3. Ver amigos\n");
    printf("4. Explorar (BFS)\n");
    printf("5. Explorar (DFS)\n");
    printf("6. Sugerir amigos\n");
    printf("0. Sair\n");
    printf("=========================\n");
    printf("Escolha uma opção: ");
}

//função principal

int main() {
    Grafo* grafo = criarGrafo();
    int opcao, id1, id2;
    char nome[TAM_NOME];

    while (1) {
        mostrarMenu();
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                printf("Digite o nome do usuário: ");
                fgets(nome, TAM_NOME, stdin);
                nome[strcspn(nome, "\n")] = '\0';
                adicionarUsuario(grafo, nome);
                break;

            case 2:
                printf("Digite o ID do primeiro usuário: ");
                scanf("%d", &id1);
                printf("Digite o ID do segundo usuário: ");
                scanf("%d", &id2);
                adicionarAmizade(grafo, id1, id2);
                break;

            case 3:
                printf("Digite o ID do usuário: ");
                scanf("%d", &id1);
                mostrarAmigos(grafo, id1);
                break;

            case 4:
                printf("Digite o ID do usuário inicial: ");
                scanf("%d", &id1);
                buscaLargura(grafo, id1);
                break;

            case 5:
                printf("Digite o ID do usuário inicial: ");
                scanf("%d", &id1);
                buscaProfundidade(grafo, id1);
                break;

            case 6:
                printf("Digite o ID do usuário: ");
                scanf("%d", &id1);
                sugerirAmigos(grafo, id1);
                break;

            case 0:
                printf("Encerrando o programa...\n");
                exit(0);

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}
