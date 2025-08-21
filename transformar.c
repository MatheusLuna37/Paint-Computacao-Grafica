#include <stdlib.h>
#include <stdio.h>
#include "pontos.h"
#include "linhas.h"
#include "poligonos.h"
#include "matrizes.h"
#include "globais.h"
#include "transformar.h"

int selecionar_objeto(float mouseX, float mouseY, Pontos *pontos, Linhas *linhas, Poligonos *poligonos, Pontos SELECIONADO) {
    SELECIONADO = NULL;
    resetar_ponto_selecionado();
    resetar_linha_selecionada();
    resetar_poligono_selecionado();
    if (selecionar_ponto(mouseX, mouseY, pontos)) {
        SELECIONADO = get_ponto_selecionado();
        return 1;
    }
    if (selecionar_linha(mouseX, mouseY, linhas)) {
        SELECIONADO = get_linha_pontos();
        return 2;
    }
    if (selecionar_poligono(mouseX, mouseY, poligonos)) {
        SELECIONADO = get_poligono_pontos();
        return 3;
    }
    return -1;
}

int aplicar_transformacao(int action, int objeto, int esquerdo, float mouseX, float mouseY, int dir, Pontos ponto_inicial) {
    int qtd;
    if (objeto == 1) qtd = 1;
    if (objeto == 2) qtd = 2;
    if (objeto == 3) qtd = -1;

    Ponto centroide = calcular_centroide(qtd, ponto_inicial);
    float **mat = alocar_matriz(3, 3);
    if (action == 2 && esquerdo) {
        matriz_translacao(TRANSLACAO, mouseX-centroide.x, mouseY-centroide.y);
        mat = TRANSLACAO;
    }
    if (action == 3) {
        if (dir > 0) matriz_escala_centroide(ESCALA_CENTROIDE, 1.1, 1.1, centroide.x, centroide.y);
        if (dir < 0) matriz_escala_centroide(ESCALA_CENTROIDE, 0.9, 0.9, centroide.x, centroide.y);
        if (dir == 0) matriz_escala_centroide(ESCALA_CENTROIDE, 1.0, 1.0, centroide.x, centroide.y);
        mat = ESCALA_CENTROIDE;
    }
    if (action == 4 && esquerdo) {
        float vx = mouseX - centroide.x;
        float vy = mouseY - centroide.y;
        float mousevx = mouseX - centroide.x;
        float mousevy = mouseY - centroide.y;
        float dot   = mousevx * vx + mousevy * vy;
        float cross = -mousevx * vy + mousevy * vx;
        float norm_u = sqrtf(mousevx*mousevx + mousevy*mousevy);
        float norm_v = sqrtf(vx*vx + vy*vy);
        float cos_theta = dot / (norm_u * norm_v);
        float sin_theta = cross / (norm_u * norm_v);
        matriz_rotacao_centroide(ROTACAO_CENTROIDE, sin_theta, cos_theta, centroide.x, centroide.y);
        mat = ROTACAO_CENTROIDE;
    }
    if (action == 5) {
        matriz_escala_centroide(ESCALA_CENTROIDE, -1.0, -1.0, centroide.x, centroide.y);
        mat = ESCALA_CENTROIDE;
    }
    if (action == 6) {
        if (dir > 0) matriz_cisalhamento_centroide(CISALHAMENTO_CENTROIDE, 0.1, centroide.x, centroide.y);
        if (dir < 0) matriz_cisalhamento_centroide(CISALHAMENTO_CENTROIDE, -0.1, centroide.x, centroide.y);
        if (dir == 0) matriz_cisalhamento_centroide(CISALHAMENTO_CENTROIDE, 0, centroide.x, centroide.y);
        mat = CISALHAMENTO_CENTROIDE;
    }

    transformar_pontos(mat, qtd, ponto_inicial);
}
