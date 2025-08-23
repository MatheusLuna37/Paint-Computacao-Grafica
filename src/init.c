#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdbool.h>
#include <stdio.h>

#include "../cabecalhos/init.h"
#include "../cabecalhos/globais.h"
#include "../cabecalhos/estruturas/pontos.h"
#include "../cabecalhos/estruturas/linhas.h"
#include "../cabecalhos/estruturas/poligonos.h"
#include "../cabecalhos/estruturas/matrizes.h"

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);

    pontos = inicializar_pontos();
    linhas = inicializar_linhas();
    poligonos = inicializar_poligonos();

    TRANSLACAO = alocar_matriz(3,3);
    ROTACAO = alocar_matriz(3,3);
    ESCALA = alocar_matriz(3,3);
    CISALHAMENTO = alocar_matriz(3,3);
    ROTACAO_CENTROIDE = alocar_matriz(3,3);
    ESCALA_CENTROIDE = alocar_matriz(3,3);
    CISALHAMENTO_CENTROIDE = alocar_matriz(3,3);
    PONTO_HOMOGENEO = alocar_matriz(3,1);
    RESULTADO = alocar_matriz(3,3);
    RESULTADOP = alocar_matriz(3,1);
}

void destruction() {
    excluir_todos_pontos(pontos);
    excluir_todas_linhas(linhas);
    excluir_todos_poligonos(poligonos);

    liberar_matriz(TRANSLACAO,3);
    liberar_matriz(ROTACAO,3);
    liberar_matriz(ESCALA,3);
    liberar_matriz(CISALHAMENTO,3);
    liberar_matriz(ROTACAO_CENTROIDE,3);
    liberar_matriz(ESCALA_CENTROIDE,3);
    liberar_matriz(CISALHAMENTO_CENTROIDE,3);
    liberar_matriz(PONTO_HOMOGENEO,3);
    liberar_matriz(RESULTADO,3);
    liberar_matriz(RESULTADOP,3);
}
