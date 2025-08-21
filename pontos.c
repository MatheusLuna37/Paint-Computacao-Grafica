#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <GL/glut.h>
#include <math.h>
#include "pontos.h"
#include "linhas.h"
#include "matrizes.h"
#include "globais.h"

static float TOLERANCIA = 2;
static Pontos PONTO_SELECIONADO = NULL;
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

Pontos get_ponto_selecionado() {
    return PONTO_SELECIONADO;
}

int avaliador_de_linha(Pontos ponto_inicial, float xmin, float xmax, float ymin, float ymax) {
    Ponto p1 = ponto_inicial->ponto;
    Ponto p2 = ponto_inicial->prox->ponto;
    return avaliar_linha(p1, p2, xmin, xmax, ymin, ymax);
}

unsigned int codificador(Ponto p, float xmin, float xmax, float ymin, float ymax) {
    unsigned int codigo = 0b0000;
    if (p.x < xmin) {
        codigo |= 0b1000;
    } else if (p.x > xmax) {
        codigo |= 0b0100;
    }
    if (p.y < ymin) {
        codigo |= 0b0010;
    } else if (p.y > ymax) {
        codigo |= 0b0001;
    }
    return codigo;
}

int avaliar_linha(Ponto p1, Ponto p2, float xmin, float xmax, float ymin, float ymax) {
    unsigned int codigo1, codigo2, and_codigos, esq, dir, ab, ac;
    codigo1 = codificador(p1, xmin, xmax, ymin, ymax);
    codigo2 = codificador(p2, xmin, xmax, ymin, ymax);
    and_codigos = codigo1 & codigo2;
    esq = codigo1 & 0b1000;
    dir = codigo1 & 0b0100;
    ab = codigo1 & 0b0010;
    ac = codigo1 & 0b0001;
    if (codigo1 == 0b0000 || codigo2 == 0b0000) {
        return 1;
    } else if (and_codigos == 0b0000) {
        if (esq) {
            return avaliar_linha((Ponto){xmin, p1.y + (xmin-p1.x)*(p2.y-p1.y)/(p2.x-p1.x)}, p2, xmin, xmax, ymin, ymax);
        } else if (dir) {
            return avaliar_linha((Ponto){xmax, p1.y + (xmax-p1.x)*(p2.y-p1.y)/(p2.x-p1.x)}, p2, xmin, xmax, ymin, ymax);
        } else if (ab) {
            return avaliar_linha((Ponto){p1.x + (ymin-p1.y)*(p2.x-p1.x)/(p2.y-p1.y), ymin}, p2, xmin, xmax, ymin, ymax);
        } else {
            return avaliar_linha((Ponto){p1.x + (ymax-p1.y)*(p2.x-p1.x)/(p2.y-p1.y), ymax}, p2, xmin, xmax, ymin, ymax);
        }
    }
    return 0;
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
        if (aux == PONTO_SELECIONADO) {
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

Ponto calcular_centroide(int qtd, Pontos ponto_inicial) {
    if (ponto_inicial == NULL) {printf("pronto"); return (Ponto){-1, -1};}
    float xm = 0, ym = 0;
    int count = 0;
    int count_aux = 0;
    if (qtd == -1) count_aux = -2;
    PontoEl *buscador = ponto_inicial;
    while (count_aux < qtd && buscador != NULL) {
        xm += buscador->ponto.x;
        ym += buscador->ponto.y;
        count++;
        if (qtd != -1) count_aux++;
        buscador = buscador->prox;
    }
    xm /= count; ym /= count;
    return (Ponto){xm, ym};
}

int transformar_pontos(float **mat, int qtd, Pontos ponto_inicial) {
    if (ponto_inicial == NULL) return 0;
    int count = 0;
    if (qtd == -1) count = -2;
    PontoEl *buscador = ponto_inicial;
    while (count < qtd && buscador != NULL) {
        ponto_homogeneo(PONTO_HOMOGENEO, buscador->ponto);
        multiplicar_matrizes(RESULTADOP, mat, 3, 3, PONTO_HOMOGENEO, 3, 1);
        buscador->ponto.x = RESULTADOP[0][0];
        buscador->ponto.y = RESULTADOP[1][0];
        buscador = buscador->prox;
        if (qtd != -1) count++;
    }

    return 1;
}

Ponto vetor_ponto_centroide(Pontos ponto, float xm, float ym) {
    float x = ponto->ponto.x, y = ponto->ponto.y;
    return (Ponto){x-xm,y-ym};
}

void resetar_ponto_selecionado() {
    PONTO_SELECIONADO = NULL;
}

int selecionar_ponto(float mouseX, float mouseY, Pontos *pontos) {
    if (pontos == NULL) return 0;
    PontoEl* buscador = *pontos;
    while (buscador != NULL) {
        if (buscador->ponto.x >= mouseX-TOLERANCIA && buscador->ponto.x <= mouseX+TOLERANCIA &&
            buscador->ponto.y >= mouseY-TOLERANCIA && buscador->ponto.y <= mouseY+TOLERANCIA) {
            PONTO_SELECIONADO = buscador;
            return 1;
        }
        buscador = buscador->prox;
    }

    PONTO_SELECIONADO = NULL;

    return 0;
}
/*
int transladar_selecionado(float mouseX, float mouseY) {
    if (PONTO_SELECIONADO == NULL || !transladando) return 0;
    float **mat_t = matriz_translacao(mouseX-PONTO_SELECIONADO->ponto.x, mouseY-PONTO_SELECIONADO->ponto.y);
    float **mat_p = ponto_homogeneo(PONTO_SELECIONADO->ponto);
    float **resultado = multiplicar_matrizes(mat_t, 3, 3,
                                       mat_p, 3, 1);
    liberar_matriz(mat_t, 3);
    liberar_matriz(mat_p, 3);

    PONTO_SELECIONADO->ponto.x = resultado[0][0];
    PONTO_SELECIONADO->ponto.y = resultado[1][0];

    liberar_matriz(resultado, 3);
    return 1;
}

int rotacionar_selecionado(float mouseX, float mouseY) {
    if (PONTO_SELECIONADO == NULL || !rotacionando) return 0;
    float x = PONTO_SELECIONADO->ponto.x, y = PONTO_SELECIONADO->ponto.y;
    float dot   = mouseX * x + mouseY * y;
    float cross = -mouseX * y + mouseY * x;
    float norm_u = sqrtf(mouseX*mouseX + mouseY*mouseY);
    float norm_v = sqrtf(x*x + y*y);

    float cos_theta = dot / (norm_u * norm_v);
    float sin_theta = cross / (norm_u * norm_v);
    float **mat_r = matriz_rotacao(sin_theta, cos_theta);
    float **mat_p = ponto_homogeneo(PONTO_SELECIONADO->ponto);
    float **resultado = multiplicar_matrizes(mat_r, 3, 3,
                                       mat_p, 3, 1);
    liberar_matriz(mat_r, 3);
    liberar_matriz(mat_p, 3);

    PONTO_SELECIONADO->ponto.x = resultado[0][0];
    PONTO_SELECIONADO->ponto.y = resultado[1][0];

    liberar_matriz(resultado, 3);
    return 1;
}
*/
int excluir_ponto_selecionado(Pontos *pontos) {
    if (pontos == NULL) return 0;
    if (PONTO_SELECIONADO == NULL) return 0;
    if (PONTO_SELECIONADO == *pontos) {
        *pontos = PONTO_SELECIONADO->prox;
        free(PONTO_SELECIONADO);
        PONTO_SELECIONADO = NULL;
        return 1;
    }
    PontoEl *buscador = (*pontos)->prox;
    PontoEl *anterior = *pontos;
    while (buscador != NULL && buscador != PONTO_SELECIONADO) {
        anterior = buscador;
        buscador = buscador->prox;
    }
    if (buscador == NULL) {
        PONTO_SELECIONADO = NULL;
        return 0;
    }
    anterior->prox = buscador->prox;
    free(buscador);
    PONTO_SELECIONADO = NULL;
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

