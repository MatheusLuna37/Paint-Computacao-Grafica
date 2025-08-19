#ifndef LINHAS_H
#define LINHAS_H

typedef struct LinhaEl {
    float x1, y1;
    float x2, y2;
    struct LinhaEl *prox;
} LinhaEl;

typedef LinhaEl* Linhas;

Linhas inicializar_linhas();
void adicionar_linha(Linhas *lista, float x1, float y1, float x2, float y2);
int desenhar_linhas(Linhas lista);
Linhas selecionar_linha(float mouseX, float mouseY, Linhas lista, float tolerancia);
int excluir_todas_linhas(Linhas *linhas);

#endif
