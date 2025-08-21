#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <GL/glut.h>
#include <math.h>
#include "pontos.h"
#include "linhas.h"
#include "matrizes.h"

static float TOLERANCIA = 2;
static Pontos SELECIONADO = NULL;
static int transladando = 0;
static int rotacionando = 0;
static float prevMouseX, prevMouseY;
static float curMouseX, curMouseY;

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

void resetar_ponto_selecionado() {
    SELECIONADO = 0;
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

float **matriz_translacao(float tx, float ty) {
    float **mat = alocar_matriz(3, 3);
    if (mat == NULL) {
        return NULL;
    }
    mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = tx;
    mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = ty;
    mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;

    return mat;
}

float **matriz_escala(float sx, float sy) {
    float **mat = alocar_matriz(3, 3);
    if (mat == NULL) {
        return NULL;
    }
    mat[0][0] = sx; mat[0][1] = 0; mat[0][2] = 0;
    mat[1][0] = 0; mat[1][1] = sy; mat[1][2] = 0;
    mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;

    return mat;
}

float **matriz_rotacao(float s, float c) {
    float **mat = alocar_matriz(3, 3);
    if (mat == NULL) {
        return NULL;
    }
    mat[0][0] = c; mat[0][1] = -s; mat[0][2] = 0;
    mat[1][0] = s; mat[1][1] = c; mat[1][2] = 0;
    mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;

    return mat;
}

float **matriz_cisalhamento(float sh) {
    float **mat = alocar_matriz(3, 3);
    if (mat == NULL) {
        return NULL;
    }
    mat[0][0] = 1; mat[0][1] = sh; mat[0][2] = 0;
    mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0;
    mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;

    return mat;
}

float **ponto_homogeneo(Ponto p) {
    float **mat = alocar_matriz(3, 1);
    if (mat == NULL) {
        return NULL;
    }
    mat[0][0] = p.x;
    mat[1][0] = p.y;
    mat[2][0] = 1;

    return mat;
}

void iniciar_translado() {
    transladando = 1;
}

void parar_translado() {
    transladando = 0;
}

void iniciar_rotacao() {
    rotacionando = 1;
}

void parar_rotacao() {
    rotacionando = 0;
}

int transladar_selecionado(float mouseX, float mouseY) {
    if (SELECIONADO == NULL || !transladando) return 0;
    float **mat_t = matriz_translacao(mouseX-SELECIONADO->ponto.x, mouseY-SELECIONADO->ponto.y);
    float **mat_p = ponto_homogeneo(SELECIONADO->ponto);
    float **resultado = multiplicar_matrizes(mat_t, 3, 3,
                                       mat_p, 3, 1);
    liberar_matriz(mat_t, 3);
    liberar_matriz(mat_p, 3);

    SELECIONADO->ponto.x = resultado[0][0];
    SELECIONADO->ponto.y = resultado[1][0];

    liberar_matriz(resultado, 3);
    return 1;
}

int rotacionar_selecionado(float mouseX, float mouseY) {
    if (SELECIONADO == NULL || !rotacionando) return 0;
    float x = SELECIONADO->ponto.x, y = SELECIONADO->ponto.y;
    float dot   = mouseX * x + mouseY * y;
    float cross = -mouseX * y + mouseY * x;
    float norm_u = sqrtf(mouseX*mouseX + mouseY*mouseY);
    float norm_v = sqrtf(x*x + y*y);

    float cos_theta = dot / (norm_u * norm_v);
    float sin_theta = cross / (norm_u * norm_v);
    float **mat_r = matriz_rotacao(sin_theta, cos_theta);
    float **mat_p = ponto_homogeneo(SELECIONADO->ponto);
    float **resultado = multiplicar_matrizes(mat_r, 3, 3,
                                       mat_p, 3, 1);
    liberar_matriz(mat_r, 3);
    liberar_matriz(mat_p, 3);

    SELECIONADO->ponto.x = resultado[0][0];
    SELECIONADO->ponto.y = resultado[1][0];

    liberar_matriz(resultado, 3);
    return 1;
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

