#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define NUMERO_USUARIOS 20

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
} Gr;

Gr* criarGrafo();
void liberarGrafo(Gr* grafo);
Usuario* criarUsuario(int id, char* nome);
NoAdjacente* criarNovoNode(Usuario* user);
void adicionarUsuario(Gr* grafo, char* nome, int numero);
bool existeConexao(NoAdjacente* lista, int destinoId);
void addConexao(Gr* grafo, int origem, int destino);
void gerarConexoesAleatorias(Gr* grafo, unsigned semente);
void imprimirListaAdjacencia(Gr* grafo);
//Busca em Largura
void BFS(Gr* grafo, int origemId, int destinoId, int* anterior);
void encontrarCaminhoMaisCurto(Gr* grafo, int origemId, int destinoId);
//Busca em Profundidade
void DFS(Gr* grafo, int atualId, int destinoId, bool* visitado, int* caminho, int* indexCaminho, int* caminhoMaisLongo, int* tamanhoCaminhoMaisLongo);
void encontrarCaminhoMaisLongo(Gr* grafo, int origemId, int destinoId);

int main() {

    Gr* grafo = criarGrafo();
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
    int user1;
    int user2;
    int isUser1Valid = 0;
    int isUser2Valid = 0;
    do {
        printf("Informe o ID do usuario de origem: ");
        scanf("%d", &user1);
        if (user1 >= 0 && user1 < NUMERO_USUARIOS) {
            isUser1Valid = 1;
        } else {
            printf("Usuario invalido. Tente digitar um ID entre 0 e 19.\n");
        }
    } while (!isUser1Valid);

    do {
        printf("Informe o ID do usuario de destino: ");
        scanf("%d", &user2);
        if (user2 >= 0 && user2 < NUMERO_USUARIOS && user1 != user2) {
            isUser2Valid = 1;
        }else {
            printf("Usuario invalido. Tente digitar um ID entre 0 e 19 e que seja diferente do ID de origem.\n");
        }
    } while (!isUser2Valid);

    encontrarCaminhoMaisCurto(grafo, user1, user2);
    encontrarCaminhoMaisLongo(grafo, user1, user2);

    liberarGrafo(grafo);
    return 0;
}
Gr* criarGrafo() {
    Gr* novoGrafo = (Gr*)malloc(sizeof(Gr));
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

void adicionarUsuario(Gr* grafo, char* nome, int numero) {
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

void addConexao(Gr* grafo, int origem, int destino) {
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

void gerarConexoesAleatorias(Gr* grafo, unsigned semente) {
    srand(semente);
    int numConexoes = rand() % (NUMERO_USUARIOS * 2) + NUMERO_USUARIOS;
    for (int i = 0; i < numConexoes; i++) {
        int origem = rand() % NUMERO_USUARIOS;
        int destino = rand() % NUMERO_USUARIOS;
        addConexao(grafo, origem, destino);
    }
}


void imprimirListaAdjacencia(Gr* grafo) {
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

void BFS(Gr* grafo, int origemId, int destinoId, int* anterior) {
    bool visitado[NUMERO_USUARIOS];
    for (int i = 0; i < NUMERO_USUARIOS; i++) {
        visitado[i] = false;
        anterior[i] = -1;
    }

    int fila[NUMERO_USUARIOS];
    int cabeca = 0, cauda = 0;
    fila[cauda++] = origemId;
    visitado[origemId] = true;

    while (cabeca < cauda) {
        int atual = fila[cabeca++];
        NoAdjacente* adjacente = grafo->listasAdjacente[atual];
        while (adjacente != NULL) {
            int adjacenteId = adjacente->usuario->id;
            if (!visitado[adjacenteId]) {
                fila[cauda++] = adjacenteId;
                visitado[adjacenteId] = true;
                anterior[adjacenteId] = atual;
                if (adjacenteId == destinoId) return;
            }
            adjacente = adjacente->proximo;
        }
    }
}

void encontrarCaminhoMaisCurto(Gr* grafo, int origemId, int destinoId) {
    int anterior[NUMERO_USUARIOS];
    BFS(grafo, origemId, destinoId, anterior);

    if (anterior[destinoId] == -1) {
        printf("Nao ha conexao entre os usuarios %d e %d\n", origemId, destinoId);
        return;
    }

    int caminho[NUMERO_USUARIOS];
    int tamanhoCaminho = 0;
    for (int at = destinoId; at != -1; at = anterior[at]) {
        caminho[tamanhoCaminho++] = at;
    }

    printf("Caminho mais curto entre %d e %d: ", origemId, destinoId);
    for (int i = tamanhoCaminho - 1; i >= 0; i--) {
        printf("%d (%s) ", caminho[i], grafo->usuarios[caminho[i]]->nome);
        if (i > 0) printf("-> ");
    }
    printf("\n");
}

void DFS(Gr* grafo, int atualId, int destinoId, bool* visitado, int* caminho, int* indexCaminho, int* caminhoMaisLongo, int* tamanhoCaminhoMaisLongo) {
    visitado[atualId] = true;
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
            if (!visitado[adjacenteId]) {
                DFS(grafo, adjacenteId, destinoId, visitado, caminho, indexCaminho, caminhoMaisLongo, tamanhoCaminhoMaisLongo);
            }
            adjacente = adjacente->proximo;
        }
    }

    (*indexCaminho)--;
    visitado[atualId] = false;
}

void encontrarCaminhoMaisLongo(Gr* grafo, int origemId, int destinoId) {
    bool visitado[NUMERO_USUARIOS] = {false};
    int caminho[NUMERO_USUARIOS];
    int caminhoMaisLongo[NUMERO_USUARIOS];
    int indexCaminho = 0;
    int tamanhoCaminhoMaisLongo = 0;

    DFS(grafo, origemId, destinoId, visitado, caminho, &indexCaminho, caminhoMaisLongo, &tamanhoCaminhoMaisLongo);

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

void liberarGrafo(Gr* grafo) {
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

    free(grafo);// liberar grafo
}
