#include <stdio.h>
#include <stdlib.h>


// Estrutura para um usuário
typedef struct Usuario {
    int id;                 // Identificador do usuário
    char name[50];          // Nome do usuário (opcional)
}Usuario;

// Estrutura para um nó da lista de adjacência
typedef struct AdjNode {
    struct Usuario* usuario;      // Ponteiro para o usuário (vértice) existente
    struct AdjNode* prox;   // Próximo nó na lista de adjacência
} AdjNode;

// Estrutura para o grafo
typedef struct Grafo{
    Usuario** Usuarios;           // Array de ponteiros para os usuários guarda todos os usuarios
    AdjNode** adjLists;     // Array de listas de adjacência
} Gr;






