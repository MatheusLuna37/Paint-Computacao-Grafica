#include <stdio.h>
#include <stdlib.h>
#include "globais.h"
#include "matrizes.h"
#include "pontos.h"

float** alocar_matriz(int linhas, int colunas) {
    float** matriz = (float**)malloc(linhas * sizeof(float*));
    if (matriz == NULL) {
        return NULL;
    }
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (float*)malloc(colunas * sizeof(float));
        if (matriz[i] == NULL) {
            for(int k = 0; k < i; k++) {
                free(matriz[k]);
            }
            free(matriz);
            return NULL;
        }
    }
    return matriz;
}

void liberar_matriz(float** matriz, int linhas) {
    if (matriz == NULL) return;
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void imprimir_matriz(float** matriz, int linhas, int colunas) {
    if (matriz == NULL) return;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%8.2f", matriz[i][j]);
        }
        printf("\n");
    }
}

void multiplicar_matrizes(float **Resultado,
                          float** matrizA, int linhasA, int colunasA,
                          float** matrizB, int linhasB, int colunasB) {

    if (colunasA != linhasB) return;
    if (Resultado == NULL) return;

    int linhasR = linhasA;
    int colunasR = colunasB;

    for (int i = 0; i < linhasR; i++) {
        for (int j = 0; j < colunasR; j++) {
            float soma = 0.0f;
            for (int k = 0; k < colunasA; k++) {
                soma += matrizA[i][k] * matrizB[k][j];
            }
            Resultado[i][j] = soma;
        }
    }
}

void matriz_translacao(float **mat, float tx, float ty) {
    if (mat == NULL) {
        return;
    }
    mat[0][0] = 1; mat[0][1] = 0; mat[0][2] = tx;
    mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = ty;
    mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;
}

void matriz_escala(float **mat, float sx, float sy) {
    if (mat == NULL) {
        return;
    }
    mat[0][0] = sx; mat[0][1] = 0; mat[0][2] = 0;
    mat[1][0] = 0; mat[1][1] = sy; mat[1][2] = 0;
    mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;
}

void matriz_rotacao(float **mat, float s, float c) {
    if (mat == NULL) {
        return;
    }
    mat[0][0] = c; mat[0][1] = -s; mat[0][2] = 0;
    mat[1][0] = s; mat[1][1] = c; mat[1][2] = 0;
    mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;
}

void matriz_cisalhamento(float **mat, float sh) {
    if (mat == NULL) {
        return;
    }
    mat[0][0] = 1; mat[0][1] = sh; mat[0][2] = 0;
    mat[1][0] = 0; mat[1][1] = 1; mat[1][2] = 0;
    mat[2][0] = 0; mat[2][1] = 0; mat[2][2] = 1;
}

void matriz_escala_centroide(float **mat, float sx, float sy, float xm, float ym) {
    if (mat == NULL) {
        return;
    }
    matriz_translacao(TRANSLACAO, xm, ym);
    matriz_escala(ESCALA, sx, sy);
    multiplicar_matrizes(RESULTADO, TRANSLACAO,3,3, ESCALA,3,3);
    matriz_translacao(TRANSLACAO, -xm, -ym);
    multiplicar_matrizes(mat, RESULTADO,3,3, TRANSLACAO, 3,3);
}

void matriz_rotacao_centroide(float **mat, float s, float c, float xm, float ym) {
    if (mat == NULL) {
        return;
    }
    matriz_translacao(TRANSLACAO, xm, ym);
    matriz_rotacao(ROTACAO, s, c);
    multiplicar_matrizes(RESULTADO, TRANSLACAO,3,3, ROTACAO,3,3);
    matriz_translacao(TRANSLACAO, -xm, -ym);
    multiplicar_matrizes(mat, RESULTADO,3,3, TRANSLACAO, 3,3);
}

void matriz_cisalhamento_centroide(float **mat, float sh, float xm, float ym) {
    if (mat == NULL) {
        return;
    }
    matriz_translacao(TRANSLACAO, xm, ym);
    matriz_cisalhamento(CISALHAMENTO, sh);
    multiplicar_matrizes(RESULTADO, TRANSLACAO,3,3, CISALHAMENTO,3,3);
    matriz_translacao(TRANSLACAO, -xm, -ym);
    multiplicar_matrizes(mat, RESULTADO,3,3, TRANSLACAO, 3,3);
}

void ponto_homogeneo(float **mat, Ponto p) {
    if (mat == NULL) {
        return;
    }
    mat[0][0] = p.x;
    mat[1][0] = p.y;
    mat[2][0] = 1;
}

