#ifndef POLIGONOS_H
#define POLIGONOS_H

#include "pontos.h"

// Estrutura de polígono
typedef struct PoligonoEl {
    Pontos vertices;           // lista de pontos do polígono
    struct PoligonoEl *prox;   // próximo polígono
} PoligonoEl;

typedef PoligonoEl* Poligonos;

// Inicializar lista de polígonos
Poligonos inicializar_poligonos();

// Criar novo polígono vazio e adicionar à lista
void adicionar_poligono(Poligonos *lista);

// Adicionar vértice a um polígono
void adicionar_vertice(PoligonoEl *poligono, float x, float y);

// Desenhar todos os polígonos
int desenhar_poligonos(Poligonos lista);

// Apagar todos os polígonos
int excluir_todos_poligonos(Poligonos *lista);

#endif
