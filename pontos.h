#ifndef PONTOS_H
#define PONTOS_H

typedef struct PontoEl {
    float x, y;
    struct PontoEl *prox;
} PontoEl;

typedef PontoEl* Pontos;

Pontos inicializar_pontos();
void adicionar_ponto(Pontos *lista, float x, float y);
int desenhar_pontos(Pontos lista);
Pontos selecionar_ponto(float mouseX, float mouseY, Pontos lista);
int excluir_todos_pontos(Pontos *pontos);

#endif
