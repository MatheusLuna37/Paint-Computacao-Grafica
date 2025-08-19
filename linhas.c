#include <stdio.h>
#include <stdlib.h>
// #include <windows.h>
#include <math.h>
#include "linhas.h"
#include <GL/glut.h>  // Para funções de desenho

Linhas inicializar_linhas() {
    return NULL;
}

void adicionar_linha(Linhas *lista, float x1, float y1, float x2, float y2) {
    LinhaEl *novo = (LinhaEl*)malloc(sizeof(LinhaEl));
    if (!novo) return;

    novo->x1 = x1;
    novo->y1 = y1;
    novo->x2 = x2;
    novo->y2 = y2;
    novo->prox = *lista;
    *lista = novo;
}

int desenhar_linhas(Linhas lista) {
    LinhaEl *aux = lista;
    while (aux != NULL) {
        glBegin(GL_LINES);
            glVertex2f(aux->x1, aux->y1);
            glVertex2f(aux->x2, aux->y2);
        glEnd();
        aux = aux->prox;
    }
    return 1;
}

Linhas selecionar_linha(float mouseX, float mouseY, Linhas lista, float tolerancia) {
    LinhaEl *aux = lista;
    while (aux != NULL) {
        // Checa se o ponto do mouse está próximo da linha (distância ponto-linha)
        float dx = aux->x2 - aux->x1;
        float dy = aux->y2 - aux->y1;
        float d = fabs(dy*mouseX - dx*mouseY + aux->x2*aux->y1 - aux->y2*aux->x1) / sqrt(dx*dx + dy*dy);
        if (d <= tolerancia) {
            LinhaEl *resultado = (LinhaEl*)malloc(sizeof(LinhaEl));
            *resultado = *aux;
            resultado->prox = NULL;
            return resultado;
        }
        aux = aux->prox;
    }
    return NULL;
}

int excluir_todas_linhas(Linhas *linhas) {
    if (linhas == NULL || *linhas == NULL) return 0;

    LinhaEl *aux = *linhas;
    while (aux != NULL) {
        *linhas = aux->prox;
        free(aux);
        aux = *linhas;
    }

    return 1;
}
