#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <GL/glut.h>
#include "pontos.h"
#include "linhas.h"

static float TOLERANCIA = 2;
static Pontos SELECIONADO = NULL;

typedef struct PontoEl {
    Ponto ponto;
    struct PontoEl *prox;
} PontoEl;

Pontos *inicializar_pontos() {
    Pontos *pontos = (Pontos *) malloc(sizeof(Pontos));
    *pontos = NULL;
    return pontos;
}

int add_ponto(Ponto ponto, Pontos *pontos) {
    if (pontos == NULL) return 0;
    PontoEl *novo = (PontoEl *) malloc(sizeof(PontoEl));
    if (novo == NULL) return 0;
    novo->ponto = ponto;
    novo->prox = *pontos;
    *pontos = novo;
    return 1;
}

int contar_arestas_atingidas (float mouseX, float mouseY, Pontos *pontos) {
    if (pontos == NULL || *pontos == NULL) return -1;
    int count = 0;
    PontoEl *aux = *pontos;
    Ponto p1, p2;
    float xi;
    while (aux != NULL) {
        p1 = aux->ponto;
        if (aux->prox != NULL) {
            p2 = aux->prox->ponto;
        } else {
            if (aux != *pontos) {
                p2 = (*pontos)->ponto;
            } else {
                return -1;
            }
        }
        if (!((p1.y > mouseY && p2.y > mouseY)
                     || (p1.y < mouseY && p2.y < mouseY)
                     || (p1.x < mouseX && p2.x < mouseX))){
            if ((p1.x > mouseX && p2.x > mouseX) && ((mouseY-p1.y)*(mouseY-p2.y) < 0)) {
                count += 1;
            } else {
                if (p1.y != p2.y) {
                    xi = p1.x + (mouseY-p1.y)*(p2.x-p1.x)/(p2.y-p1.y);
                    if (xi > mouseX && xi != p2.x) {
                        count += 1;
                    }
                }
            }
        }
        aux = aux->prox;
    }
    return count;
}

int converter_vertices(Pontos *pontos) {
    if (pontos == NULL) return 0;
    PontoEl *aux = *pontos;
    while (aux != NULL) {
        glVertex2f(aux->ponto.x, aux->ponto.y);
        aux = aux->prox;
    }
    return 1;
}

int desenhar_pontos(Pontos *pontos) {
    if (pontos == NULL) return 0;
    PontoEl *aux = *pontos;
    while (aux != NULL) {
        if (aux == SELECIONADO) {
            glColor3f(0, 1, 0);
        }
        glBegin(GL_POINTS);
            glVertex2f(aux->ponto.x, aux->ponto.y);
        glEnd();
        aux = aux->prox;
        glColor3f(0, 0, 0);
    }
    return 1;
}

int selecionar_ponto(float mouseX, float mouseY, Pontos *pontos) {
    if (pontos == NULL) return 0;
    PontoEl* buscador = *pontos;
    while (buscador != NULL) {
        if (buscador->ponto.x >= mouseX-TOLERANCIA && buscador->ponto.x <= mouseX+TOLERANCIA &&
            buscador->ponto.y >= mouseY-TOLERANCIA && buscador->ponto.y <= mouseY+TOLERANCIA) {
            SELECIONADO = buscador;
            return 1;
        }
        buscador = buscador->prox;
    }

    SELECIONADO = NULL;

    return 0;
}

int excluir_ponto_selecionado(Pontos *pontos) {
    if (pontos == NULL) return 0;
    if (SELECIONADO == NULL) return 0;
    if (SELECIONADO == *pontos) {
        *pontos = SELECIONADO->prox;
        free(SELECIONADO);
        SELECIONADO = NULL;
        return 1;
    }
    PontoEl *buscador = (*pontos)->prox;
    PontoEl *anterior = *pontos;
    while (buscador != NULL && buscador != SELECIONADO) {
        anterior = buscador;
        buscador = buscador->prox;
    }
    if (buscador == NULL) {
        SELECIONADO = NULL;
        return 0;
    }
    anterior->prox = buscador->prox;
    free(buscador);
    SELECIONADO = NULL;
    return 1;
}

int excluir_todos_pontos(Pontos *pontos) {
    if (pontos == NULL) return 0;
    PontoEl *aux = *pontos;
    while (aux != NULL) {
        *pontos = aux->prox;
        free(aux);
        aux = *pontos;
    }
    free(*pontos);
    return 1;
}

