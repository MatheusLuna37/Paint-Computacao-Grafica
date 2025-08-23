#ifndef MATRIZES_H_INCLUDED
#define MATRIZES_H_INCLUDED

#include "../estruturas/pontos.h"

float** alocar_matriz(int linhas, int colunas);
void liberar_matriz(float** matriz, int linhas);
void imprimir_matriz(float** matriz, int linhas, int colunas);

void multiplicar_matrizes(float **Resultado,
                          float** matrizA, int linhasA, int colunasA,
                          float** matrizB, int linhasB, int colunasB);

void matriz_translacao(float **mat, float tx, float ty);
void matriz_escala(float **mat, float sx, float sy);
void matriz_rotacao(float **mat, float s, float c);
void matriz_cisalhamento(float **mat, float sh);

void matriz_escala_centroide(float **mat, float sx, float sy, float xm, float ym);
void matriz_rotacao_centroide(float **mat, float s, float c, float xm, float ym);
void matriz_cisalhamento_centroide(float **mat, float sh, float xm, float ym);

void ponto_homogeneo(float **mat, Ponto p);

#endif