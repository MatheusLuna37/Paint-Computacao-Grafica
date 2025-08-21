#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../../cabecalhos/pontos.h"
#include "../../cabecalhos/linhas.h"
#include "../../cabecalhos/poligonos.h"
#include "../../cabecalhos/matrizes.h"
#include "../../cabecalhos/globais.h"
#include "../../cabecalhos/transformar.h"

int selecionar_objeto(float mouseX, float mouseY, Pontos *pontos, Linhas *linhas, Poligonos *poligonos, Pontos *SELECIONADO) {
    *SELECIONADO = NULL;
    resetar_ponto_selecionado();
    resetar_linha_selecionada();
    resetar_poligono_selecionado();
    if (selecionar_ponto(mouseX, mouseY, pontos)) {
        *SELECIONADO = get_ponto_selecionado();
        return 1;
    }
    if (selecionar_linha(mouseX, mouseY, linhas)) {
        *SELECIONADO = get_linha_pontos();
        return 2;
    }
    if (selecionar_poligono(mouseX, mouseY, poligonos)) {
        *SELECIONADO = get_poligono_pontos();
        return 3;
    }
    return -1;
}

int aplicar_transformacao(int action, int objeto, float mouseX, float mouseY, int dir, Pontos ponto_inicial) {
    if (ponto_inicial == NULL) return 0;
    if (action < 2) return 0;
    int qtd;
    if (objeto == 1) qtd = 1;
    if (objeto == 2) qtd = 2;
    if (objeto == 3) qtd = -1;

    Ponto centroide = calcular_centroide(qtd, ponto_inicial);
    if (centroide.x == -1 && centroide.y == -1) {printf("n�o passei"); return 0;}
    float **mat;
    matriz_translacao(TRANSLACAO, 0, 0);
    mat = TRANSLACAO;
    if (action == 2) {
        printf("transladando ");
        matriz_translacao(TRANSLACAO, mouseX-centroide.x, mouseY-centroide.y);
        mat = TRANSLACAO;
    }
    if (action == 3) {
        printf("escalando ");
        if (dir > 0) matriz_escala_centroide(ESCALA_CENTROIDE, 1.1, 1.1, centroide.x, centroide.y);
        if (dir < 0) matriz_escala_centroide(ESCALA_CENTROIDE, 0.9, 0.9, centroide.x, centroide.y);
        if (dir == 0) matriz_escala_centroide(ESCALA_CENTROIDE, 1.0, 1.0, centroide.x, centroide.y);
        mat = ESCALA_CENTROIDE;
    }
    if (action == 4) {
        printf("rotacionando ");
        Ponto centro = centroide;
        if (objeto == 1) centro = (Ponto){0,0};
        Ponto vetor = vetor_ponto_centroide(ponto_inicial, centro.x, centro.y);
        float vx = vetor.x;
        float vy = vetor.y;
        float mousevx = mouseX - centro.x;
        float mousevy = mouseY - centro.y;
        float dot = mousevx * vx + mousevy * vy;
        float cross = -mousevx * vy + mousevy * vx;
        float norm_u = sqrtf(mousevx*mousevx + mousevy*mousevy);
        float norm_v = sqrtf(vx*vx + vy*vy);
        float cos_theta = dot / (norm_u * norm_v);
        float sin_theta = cross / (norm_u * norm_v);
        if (objeto == 1) {
            matriz_rotacao(ROTACAO, sin_theta, cos_theta);
            mat = ROTACAO;
        } else {
            matriz_rotacao_centroide(ROTACAO_CENTROIDE, sin_theta, cos_theta, centroide.x, centroide.y);
            mat = ROTACAO_CENTROIDE;
        }
    }
    if (action == 5) {
        printf("refletindo");
        matriz_escala_centroide(ESCALA_CENTROIDE, -1, 1, centroide.x, centroide.y);
        mat = ESCALA_CENTROIDE;
    }
    if (action == 6) {
        printf("cisalhando");
        if (dir > 0) matriz_cisalhamento_centroide(CISALHAMENTO_CENTROIDE, 0.1, centroide.x, centroide.y);
        if (dir < 0) matriz_cisalhamento_centroide(CISALHAMENTO_CENTROIDE, -0.1, centroide.x, centroide.y);
        if (dir == 0) matriz_cisalhamento_centroide(CISALHAMENTO_CENTROIDE, 0, centroide.x, centroide.y);
        mat = CISALHAMENTO_CENTROIDE;
    }

    printf("quase l�");
    transformar_pontos(mat, qtd, ponto_inicial);

    return 1;
}
