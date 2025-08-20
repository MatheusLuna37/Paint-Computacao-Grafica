#include <stdio.h>
#include <stdlib.h>

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

float** multiplicar_matrizes(float** matrizA, int linhasA, int colunasA,
                             float** matrizB, int linhasB, int colunasB) {

    if (colunasA != linhasB) {
        fprintf(stderr, "Erro: As dimensões das matrizes são incompatíveis para multiplicação.\n");
        return NULL;
    }

    // 2. Alocação da matriz resultado
    // A matriz resultado terá dimensões: linhasA x colunasB
    int linhasR = linhasA;
    int colunasR = colunasB;
    float** matrizResultado = alocar_matriz(linhasR, colunasR);
    if (matrizResultado == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para a matriz resultado.\n");
        return NULL;
    }

    // 3. Realizar a multiplicação
    // C[i][j] = soma(A[i][k] * B[k][j])
    for (int i = 0; i < linhasR; i++) {
        for (int j = 0; j < colunasR; j++) {
            float soma = 0.0f;
            // O laço interno percorre as colunas de A e as linhas de B
            for (int k = 0; k < colunasA; k++) {
                soma += matrizA[i][k] * matrizB[k][j];
            }
            matrizResultado[i][j] = soma;
        }
    }

    // 4. Retornar a matriz calculada
    return matrizResultado;
}
