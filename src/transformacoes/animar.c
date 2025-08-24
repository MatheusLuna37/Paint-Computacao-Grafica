#include <GL/glut.h>
#include <stdbool.h>
#include <math.h>
#include "../../cabecalhos/globais.h"
#include "../../cabecalhos/estruturas/linhas.h"
#include "../../cabecalhos/estruturas/poligonos.h"
#include "../../cabecalhos/estruturas/pontos.h"
#include "../../cabecalhos/estruturas/matrizes.h"
#include "../../cabecalhos/transformacoes/animar.h"

Poligonos poligono_animado = NULL;

// Controle da animação DVD
static float dvd_dx = 4, dvd_dy = 4;
static bool animando = false;
bool game_over = false;

void reiniciar_animacao() {
    dvd_dx = 4;
    dvd_dy = 4;
    animando = false;
    game_over = false;
    poligono_animado = NULL;
    glutPostRedisplay();
}

void excluir_tudo() {
    if (pontos) excluir_todos_pontos(pontos);
    if (linhas) excluir_todas_linhas(linhas);
    if (poligonos) excluir_todos_poligonos(poligonos);
}

void desenhar_game_over() {
    glColor3f(1, 0, 0);
    glRasterPos2f(screenWidth/2 - 60, screenHeight/2);
    const char* msg = "GAME OVER";
    for (const char* c = msg; *c; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

bool colisao_bbox
    (
        float minA, 
        float maxA, 
        float minB, 
        float maxB
    ) {
    return (minA <= maxB && maxA >= minB);
}

// Calcula a bounding box de uma lista de pontos
void calcular_bounding_box
    (
        Pontos pts, 
        float *minX, 
        float *maxX, 
        float *minY, 
        float *maxY
    ) {
    *minX = *minY = 1e9;
    *maxX = *maxY = -1e9;
    for (Pontos v = pts; v != NULL; v = v->prox) {
        if (v->ponto.x < *minX) *minX = v->ponto.x;
        if (v->ponto.x > *maxX) *maxX = v->ponto.x;
        if (v->ponto.y < *minY) *minY = v->ponto.y;
        if (v->ponto.y > *maxY) *maxY = v->ponto.y;
    }
}

// Verifica colisão do polígono animado com outros polígonos
bool checar_colisao_poligonos(
    Poligonos poligono_animado,
    float minX, float maxX, float minY, float maxY
) {
    for (Poligonos q = (poligonos ? *poligonos : NULL); q != NULL; q = q->prox) {
        if (q == poligono_animado) continue; // Não colide consigo mesmo
        float qminX, qmaxX, qminY, qmaxY;
        calcular_bounding_box(*(q->poligono.pontos), &qminX, &qmaxX, &qminY, &qmaxY);
        if (colisao_bbox(minX, maxX, qminX, qmaxX) && colisao_bbox(minY, maxY, qminY, qmaxY)) {
            return true;
        }
    }
    return false;
}

// Tenta comer um ponto, retorna true se comeu
bool comer_ponto
    (
        float minX, 
        float maxX, 
        float minY, 
        float maxY, 
        Poligonos p
    ) {
    if (pontos && *pontos) {
        Pontos *pptr = pontos;
        Pontos prev = NULL, curr = *pptr;
        while (curr != NULL) {
            float px = curr->ponto.x, py = curr->ponto.y;
            if (px >= minX && px <= maxX && py >= minY && py <= maxY) {
                if (prev == NULL) {
                    *pptr = curr->prox;
                } else {
                    prev->prox = curr->prox;
                }
                Pontos to_free = curr;
                curr = curr->prox;
                free(to_free);
                // Escala em 10% em torno do centroide
                Ponto centro = calcular_centroide(-1, *(p->poligono.pontos));
                float **mat = alocar_matriz(3, 3);
                matriz_escala_centroide(mat, 1.1f, 1.1f, centro.x, centro.y);
                transformar_pontos(mat, -1, *(p->poligono.pontos));
                liberar_matriz(mat, 3);
                return true;
            } else {
                prev = curr;
                curr = curr->prox;
            }
        }
    }
    return false;
}

// Tenta comer uma linha, retorna true se comeu
bool comer_linha
    (
        float minX, 
        float maxX, 
        float minY, 
        float maxY, 
        Poligonos p
    ) {
    if (linhas && *linhas) {
        Linhas *lptr = linhas;
        Linhas prevL = NULL, currL = *lptr;
        while (currL != NULL) {
            float lminX, lmaxX, lminY, lmaxY;
            calcular_bounding_box(*(currL->linha.pontos), &lminX, &lmaxX, &lminY, &lmaxY);
            if (colisao_bbox(minX, maxX, lminX, lmaxX) && colisao_bbox(minY, maxY, lminY, lmaxY)) {
                if (prevL == NULL) {
                    *lptr = currL->prox;
                } else {
                    prevL->prox = currL->prox;
                }
                excluir_todos_pontos(currL->linha.pontos);
                Linhas to_free = currL;
                currL = currL->prox;
                free(to_free);
                // Diminui em 10% em torno do centroide
                Ponto centro = calcular_centroide(-1, *(p->poligono.pontos));
                float **mat = alocar_matriz(3, 3);
                matriz_escala_centroide(mat, 0.9f, 0.9f, centro.x, centro.y);
                transformar_pontos(mat, -1, *(p->poligono.pontos));
                liberar_matriz(mat, 3);
                return true;
            } else {
                prevL = currL;
                currL = currL->prox;
            }
        }
    }
    return false;
}

void animar(int value) {
    if (!animando || game_over) return;

    Poligonos p = poligono_animado;
    if (p) {
        float minX, maxX, minY, maxY;
        calcular_bounding_box(*(p->poligono.pontos), &minX, &maxX, &minY, &maxY);

        // Colisão com bordas
        if (minX + dvd_dx < 0 || maxX + dvd_dx > screenWidth)
            dvd_dx = -dvd_dx;
        if (minY + dvd_dy < 0 || maxY + dvd_dy > screenHeight)
            dvd_dy = -dvd_dy;

        // Colisão com outros polígonos (GAME OVER)
        if (checar_colisao_poligonos(poligono_animado, minX, maxX, minY, maxY)) {
            animando = false;
            game_over = true;
            excluir_tudo();
            glutPostRedisplay();
            return;
        }

        // Colisão com pontos: "come" o ponto e cresce
        comer_ponto(minX, maxX, minY, maxY, p);

        // Colisão com linhas: "come" a linha e diminui o polígono
        comer_linha(minX, maxX, minY, maxY, p);

        // Verifica se o polígono ficou pequeno demais (morre)
        calcular_bounding_box(*(p->poligono.pontos), &minX, &maxX, &minY, &maxY);
        float largura = maxX - minX;
        float altura  = maxY - minY;
        if (largura < 1.0f || altura < 1.0f) {
            animando = false;
            game_over = true;
            excluir_tudo();
            glutPostRedisplay();
            return;
        }

        // Aplica translação ao polígono animado
        float mat[3][3] = {
            {1, 0, dvd_dx},
            {0, 1, dvd_dy},
            {0, 0, 1}
        };
        float *mat_ptr[3] = {mat[0], mat[1], mat[2]};
        transformar_pontos(mat_ptr, -1, *(p->poligono.pontos));
    }

    glutPostRedisplay();
    glutTimerFunc(33, animar, 0);
}

void iniciar_animacao() {
    if (!animando) {
        // Se ainda não foi definido, pega o primeiro polígono atual
        if (poligono_animado == NULL && poligonos && *poligonos)
            poligono_animado = *poligonos;
        animando = true;
        glutTimerFunc(33, animar, 0);
    }
}

void parar_animacao() {
    animando = false;
}