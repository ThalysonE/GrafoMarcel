#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define NUMERO_USUARIOS 20
#define BRANCO 0
#define CINZA 1
#define PRETO 2

typedef struct Usuario {
    int id;
    char nome[50];
} Usuario;

typedef struct NoAdjacente {
    struct Usuario* usuario;
    struct NoAdjacente* proximo;
} NoAdjacente;

typedef struct Grafo {
    Usuario** usuarios;
    NoAdjacente** listasAdjacente;
} Grafo;

Grafo* criarGrafo();
void liberarGrafo(Grafo* grafo);
Usuario* criarUsuario(int id, char* nome);
NoAdjacente* criarNovoNode(Usuario* user);
void adicionarUsuario(Grafo* grafo, char* nome, int numero);
bool existeConexao(NoAdjacente* lista, int destinoId);
void addConexao(Grafo* grafo, int origem, int destino);
void gerarConexoesAleatorias(Grafo* grafo, unsigned semente);
void imprimirListaAdjacencia(Grafo* grafo);
//Busca em Largura
void BFS(Grafo* grafo, int origemId, int destinoId, int* anterior);
void encontrarCaminhoMaisCurto(Grafo* grafo, int origemId, int destinoId);
//Busca em Profundidade
void DFS(Grafo* grafo, int atualId, int destinoId, int* cor, int* caminho, int* indexCaminho, int* caminhoMaisLongo, int* tamanhoCaminhoMaisLongo);
void encontrarCaminhoMaisLongo(Grafo* grafo, int origemId, int destinoId);

int main() {

    Grafo* grafo = criarGrafo();
    char* nomes[NUMERO_USUARIOS] = {"Carolina", "Luan", "Fernanda", "Yasmin", "Enzo", "Tomas",
                                    "Julia", "Rafaela", "Felipe", "Renan", "Daniel", "Bianca",
                                    "Guilherme", "Kaua", "Estevan", "Melissa", "Diego",
                                    "Carlos", "Giovana", "Gabriel"};

    for (int i = 0; i < NUMERO_USUARIOS; i++) {
        adicionarUsuario(grafo, nomes[i], i);
    }

    gerarConexoesAleatorias(grafo, time(NULL));

    printf("\nLista de Adjacencia:\n");
    imprimirListaAdjacencia(grafo);

    printf("\n");
    int usuarioOrigem;
    int usuarioDestino;
    int isUsuarioOrigemValido = 0;
    int isUsuarioDestinoValido = 0;
    do {
        printf("Informe o ID do usuario de origem: ");
        scanf("%d", &usuarioOrigem);
        if (usuarioOrigem >= 0 && usuarioOrigem < NUMERO_USUARIOS) {
            isUsuarioOrigemValido = 1;
        } else {
            printf("Usuario invalido. Tente digitar um ID entre 0 e 19.\n");
        }
    } while (!isUsuarioOrigemValido);

    do {
        printf("Informe o ID do usuario de destino: ");
        scanf("%d", &usuarioDestino);
        if (usuarioDestino >= 0 && usuarioDestino < NUMERO_USUARIOS && usuarioOrigem != usuarioDestino) {
            isUsuarioDestinoValido = 1;
        }else {
            printf("Usuario invalido. Tente digitar um ID entre 0 e 19 e que seja diferente do ID de origem.\n");
        }
    } while (!isUsuarioDestinoValido);

    encontrarCaminhoMaisCurto(grafo, usuarioOrigem, usuarioDestino);
    encontrarCaminhoMaisLongo(grafo, usuarioOrigem, usuarioDestino);

    liberarGrafo(grafo);
    return 0;
}

Grafo* criarGrafo() {
    Grafo* novoGrafo = (Grafo*)malloc(sizeof(Grafo));
    novoGrafo->usuarios = (Usuario**)malloc(NUMERO_USUARIOS * sizeof(Usuario*));
    novoGrafo->listasAdjacente = (NoAdjacente**)malloc(NUMERO_USUARIOS * sizeof(NoAdjacente*));

    for (int i = 0; i < NUMERO_USUARIOS; i++) {
        novoGrafo->usuarios[i] = NULL;
        novoGrafo->listasAdjacente[i] = NULL;
    }
    return novoGrafo;
}

Usuario* criarUsuario(int id, char* nome) {
    Usuario* novoUsuario = (Usuario*)malloc(sizeof(Usuario));
    novoUsuario->id = id;
    strcpy(novoUsuario->nome, nome);
    return novoUsuario;
}

NoAdjacente* criarNovoNode(Usuario* user) {
    NoAdjacente* novoNode = (NoAdjacente*)malloc(sizeof(NoAdjacente));
    novoNode->usuario = user;
    novoNode->proximo = NULL;
    return novoNode;
}

void adicionarUsuario(Grafo* grafo, char* nome, int numero) {
    Usuario* novoUsuario = criarUsuario(numero, nome);
    grafo->usuarios[numero] = novoUsuario;
    grafo->listasAdjacente[numero] = NULL;
}

bool existeConexao(NoAdjacente* lista, int destinoId) {
    NoAdjacente* adjacente = lista;
    while (adjacente != NULL) {
        if (adjacente->usuario->id == destinoId) {
            return true;
        }
        adjacente = adjacente->proximo;
    }
    return false;
}

void addConexao(Grafo* grafo, int origem, int destino) {
    if (origem >= NUMERO_USUARIOS || destino >= NUMERO_USUARIOS || origem == destino) {
        return;
    }

    if (!existeConexao(grafo->listasAdjacente[origem], destino) && !existeConexao(grafo->listasAdjacente[destino], origem)) {
        NoAdjacente* novoNode = criarNovoNode(grafo->usuarios[destino]);
        novoNode->proximo = grafo->listasAdjacente[origem];
        grafo->listasAdjacente[origem] = novoNode;

        novoNode = criarNovoNode(grafo->usuarios[origem]);
        novoNode->proximo = grafo->listasAdjacente[destino];
        grafo->listasAdjacente[destino] = novoNode;
    }
}

void gerarConexoesAleatorias(Grafo* grafo, unsigned semente) {
    srand(semente);
    int numConexoes = rand() % (NUMERO_USUARIOS * 2) + NUMERO_USUARIOS;
    for (int i = 0; i < numConexoes; i++) {
        int origem = rand() % NUMERO_USUARIOS;
        int destino = rand() % NUMERO_USUARIOS;
        addConexao(grafo, origem, destino);
    }
}


void imprimirListaAdjacencia(Grafo* grafo) {
    for (int i = 0; i < NUMERO_USUARIOS; i++) {
        Usuario* usuario = grafo->usuarios[i];
        if (usuario != NULL) {
            printf("%d (%s): ", usuario->id, usuario->nome);
            NoAdjacente* adjacente = grafo->listasAdjacente[i];
            int contador = 0;
            while (adjacente != NULL) {
                if (contador > 0) printf(", ");
                printf("%d (%s)", adjacente->usuario->id, adjacente->usuario->nome);
                adjacente = adjacente->proximo;
                contador++;
            }
            printf("\n");
        }
    }
}

void BFS(Grafo* grafo, int origemId, int destinoId, int* anterior) {
    int cor[NUMERO_USUARIOS];
    for (int i = 0; i < NUMERO_USUARIOS; i++) {
        cor[i] = BRANCO;
        anterior[i] = -1;
    }

    int fila[NUMERO_USUARIOS];
    int cabeca = 0, cauda = 0;
    fila[cauda++] = origemId;
    cor[origemId] = CINZA;

    while (cabeca < cauda) {
        int atual = fila[cabeca++];
        NoAdjacente* adjacente = grafo->listasAdjacente[atual];
        while (adjacente != NULL) {
            int adjacenteId = adjacente->usuario->id;
            if (cor[adjacenteId] == BRANCO) {
                fila[cauda++] = adjacenteId;
                cor[adjacenteId] = CINZA;
                anterior[adjacenteId] = atual;
                if (adjacenteId == destinoId) return;
            }
            adjacente = adjacente->proximo;
        }
        cor[atual] = PRETO;
    }
}

void encontrarCaminhoMaisCurto(Grafo* grafo, int origemId, int destinoId) {
    int anterior[NUMERO_USUARIOS];
    BFS(grafo, origemId, destinoId, anterior);

    if (anterior[destinoId] == -1) {
        printf("Nao ha conexao entre os usuarios %d e %d\n", origemId, destinoId);
        return;
    }

    int caminho[NUMERO_USUARIOS];
    int tamanhoCaminho = 0;
    for (int i = destinoId; i != -1; i = anterior[i]) {
        caminho[tamanhoCaminho++] = i;
    }

    printf("Caminho mais curto entre %d e %d: ", origemId, destinoId);
    for (int i = tamanhoCaminho - 1; i >= 0; i--) {
        printf("%d (%s) ", caminho[i], grafo->usuarios[caminho[i]]->nome);
        if (i > 0) printf("-> ");
    }
    printf("\n");
}

void DFS(Grafo* grafo, int atualId, int destinoId, int* cor, int* caminho, int* indexCaminho, int* caminhoMaisLongo, int* tamanhoCaminhoMaisLongo) {
    cor[atualId] = CINZA;
    caminho[(*indexCaminho)++] = atualId;

    if (atualId == destinoId) {
        if (*indexCaminho > *tamanhoCaminhoMaisLongo) {
            *tamanhoCaminhoMaisLongo = *indexCaminho;
            for (int i = 0; i < *indexCaminho; i++) {
                caminhoMaisLongo[i] = caminho[i];
            }
        }
    } else {
        NoAdjacente* adjacente = grafo->listasAdjacente[atualId];
        while (adjacente != NULL) {
            int adjacenteId = adjacente->usuario->id;
            if (cor[adjacenteId] == BRANCO) {
                DFS(grafo, adjacenteId, destinoId, cor, caminho, indexCaminho, caminhoMaisLongo, tamanhoCaminhoMaisLongo);
            }
            adjacente = adjacente->proximo;
        }
    }

    (*indexCaminho)--;
    cor[atualId] = PRETO;
}

void encontrarCaminhoMaisLongo(Grafo* grafo, int origemId, int destinoId) {
    int cor[NUMERO_USUARIOS];
    for (int i = 0; i < NUMERO_USUARIOS; i++) {
        cor[i] = BRANCO;
    }

    int caminho[NUMERO_USUARIOS];
    int caminhoMaisLongo[NUMERO_USUARIOS];
    int indexCaminho = 0;
    int tamanhoCaminhoMaisLongo = 0;

    DFS(grafo, origemId, destinoId, cor, caminho, &indexCaminho, caminhoMaisLongo, &tamanhoCaminhoMaisLongo);

    if (tamanhoCaminhoMaisLongo == 0) {
        printf("Nao ha conexao entre os usuarios %d e %d\n", origemId, destinoId);
        return;
    }

    printf("Caminho mais longo entre %d e %d: ", origemId, destinoId);
    for (int i = 0; i < tamanhoCaminhoMaisLongo; i++) {
        printf("%d (%s) ", caminhoMaisLongo[i], grafo->usuarios[caminhoMaisLongo[i]]->nome);
        if (i < tamanhoCaminhoMaisLongo - 1) printf("-> ");
    }
    printf("\n");
}

void liberarGrafo(Grafo* grafo) {
    if (grafo == NULL) {
        return;
    }
    for (int i = 0; i < NUMERO_USUARIOS; i++) {
        NoAdjacente* atual = grafo->listasAdjacente[i];
        while (atual != NULL) {
            NoAdjacente* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
    for (int i = 0; i < NUMERO_USUARIOS; i++) {
        if (grafo->usuarios[i] != NULL) {
            free(grafo->usuarios[i]);
        }
    }
    free(grafo->usuarios);
    free(grafo->listasAdjacente);

    free(grafo);
}
