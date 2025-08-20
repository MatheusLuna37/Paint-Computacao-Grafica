#ifndef MATRIZES_H_INCLUDED
#define MATRIZES_H_INCLUDED

float** alocar_matriz(int linhas, int colunas);
void liberar_matriz(float** matriz, int linhas);
void imprimir_matriz(float** matriz, int linhas, int colunas);
float** multiplicar_matrizes(float** matrizA, int linhasA, int colunasA,
                             float** matrizB, int linhasB, int colunasB);


#endif
