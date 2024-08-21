#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NUM_Usuarios 20
// Estrutura para um usuário
typedef struct Usuario {
    int id;                 // Identificador do usuário
    char nome[50];          // Nome do usuário (opcional)
}Usuario;

// Estrutura para um nó da lista de adjacência
typedef struct AdjNode {
    struct Usuario* usuario;      // Ponteiro para o usuário (vértice) existente
    struct AdjNode* prox;   // Próximo nó na lista de adjacência
} AdjNode;

// Estrutura para o grafo
typedef struct Grafo{
    Usuario** usuarios;           // Array de ponteiros para os usuários guarda todos os usuarios
    AdjNode** adjLists;     // Array de listas de adjacência
} Gr;

//Função para criar grafo

Gr* criarGrafo(){
    Gr* novoGr= (Gr*)malloc(sizeof(Gr));
    if(novoGr == NULL){
        return NULL;
    }
    novoGr->usuarios = (Usuario**)malloc(NUM_Usuarios* sizeof (Usuario*));
    novoGr->adjLists = (AdjNode**)malloc(NUM_Usuarios * sizeof (AdjNode*));

    for(int i=0; i < NUM_Usuarios; i++){
        novoGr->usuarios[i] = NULL;
        novoGr->adjLists[i] = NULL;
    }
    return novoGr;
}
//Função para criar Usuarios
Usuario* criarUsuario(int id, char* nome){
    Usuario* novoUser = (Usuario*)malloc(sizeof(Usuario));
    if(novoUser == NULL){
        return NULL;
    }
    novoUser->id = id;
    strcpy(novoUser->nome,nome);
    return novoUser;
}

//Funcao para criar um novo nó de adjacência
AdjNode * criarNovoNode(Usuario* user){
    AdjNode* novoNode = (AdjNode*) malloc(sizeof(AdjNode));
    if(novoNode == NULL){
        return NULL;
    }
    novoNode ->usuario = user;
    novoNode -> prox = NULL;
    return novoNode;
}

//Funcao para adicionar um usuário ao grafo
void addUsuario(Gr*grafo, char* nome, int num){
    Usuario * novoUser = criarUsuario(num,nome);
    grafo->usuarios[num] = novoUser;
    grafo->adjLists[num] = NULL;
}
//Funcao para adicionar uma conexão entre usuarios
void addConexao(Gr* grafo, int orig, int dest){
    if(orig >= NUM_Usuarios || dest >= NUM_Usuarios){
        return;
    }
    //Adiciona a conexão da origem para o destino
    AdjNode * novoNode = criarNovoNode(grafo->usuarios[dest]);
    novoNode->prox = grafo->adjLists[orig];
    grafo->adjLists[orig] = novoNode;

    //Como o grafo é não direcional adiciona-se a conexão inversa
    novoNode = criarNovoNode(grafo->usuarios[orig]);
    novoNode->prox = grafo->adjLists[dest];
    grafo->adjLists[dest] = novoNode;
}
void imprimirListaAdjacencia(Gr* grafo) {
    for (int i = 0; i < NUM_Usuarios; i++) {
        if(grafo->adjLists[i] != NULL){ // verifica se o usuario possui alguma conexão
            Usuario *usuario = grafo->usuarios[i];
            if (usuario != NULL) {
                printf("Usuario %d (%s): ", usuario->id, usuario->nome);
                AdjNode *adj = grafo->adjLists[i];
                while (adj != NULL) {
                    printf("%d (%s) ", adj->usuario->id, adj->usuario->nome);
                    adj = adj->prox;
                }
                printf("\n");
            }
        }
    }
}
void main (){
    Gr* grafo = criarGrafo();
    char* nomes[NUM_Usuarios] = {"Carolina","Luan","Fernanda","Yasmin","Enzo","Tomas",
                                 "Julia","Rafaela","Felipe","Renan","Daniel","Bianca",
                                 "Guilherme", "Kaua", "Estevan","Melissa","Diego",
                                 "Carlos","Giovana", "Gabriel"};

    for(int i = 0; i < NUM_Usuarios; i++){
        addUsuario(grafo, nomes[i], i);
    }

    //TESTES
    int cont = 0;
    //imprime todos os usuarios e seus respectivos Id
    for(int i =0; i < NUM_Usuarios; i++){
        printf("%d - %s  |  ",grafo->usuarios[i]->id,grafo->usuarios[i]->nome);
        cont++;
        if(cont == 7){
            printf("\n");
            cont = 0;
        }
    }
    addConexao(grafo, 11, 7);
    addConexao(grafo,14,19);
    addConexao(grafo,14,17);
    addConexao(grafo,3,2);
    printf("\n\n");
    imprimirListaAdjacencia(grafo);
}











