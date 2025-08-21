#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdbool.h>
#include <stdio.h>
//#include <windows.h>
#include "../cabecalhos/pontos.h"
#include "../cabecalhos/linhas.h"
#include "../cabecalhos/poligonos.h"
#include "../cabecalhos/globais.h"
#include "../cabecalhos/transformar.h"
#include "../cabecalhos/matrizes.h"

Pontos *pontos;
Linhas *linhas;
Poligonos *poligonos;

static Pontos SELECIONADO = NULL;

// Estado
int modo = -1;
/*
-1: nenhum
0: selecionar
1: desenhar
*/
int objeto = -1;
/*
-1: nenhum
1: ponto
2: linha
3: pol�gono
*/
int action = -1;

int esquerdo = 0;
int direito = 0;

int dir = 0;

float mouseX, mouseY;

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

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        esquerdo = 1;
        dir = 0;
        mouseX = x;
        mouseY = screenHeight - y;
        if (modo == 1) {
            if (objeto == 1) {
                add_ponto((Ponto){mouseX,mouseY}, pontos);
            } else if (objeto == 2) {
                criar_linha(mouseX, mouseY, linhas);
            } else if (objeto == 3) {
                add_vertice_poligono_atual(mouseX, mouseY, poligonos);
            }
        } else if (modo == 0) {
            objeto = selecionar_objeto(mouseX, mouseY, pontos, linhas, poligonos, &SELECIONADO);
        }
        glutPostRedisplay();
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        esquerdo = 0;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if (modo == 1) {
            if (objeto == 3) {
                if (cancelar_poligono_atual());
            } else if (objeto == 2) {
                if (cancelar_linha_atual());
            }
        }
    }
}

void motion(int x, int y) {
    mouseX = x;
    mouseY = screenHeight - y;

    if (modo == 0 && (action == 2 || action == 4) && esquerdo) {
        aplicar_transformacao(action, objeto, mouseX, mouseY, 0, SELECIONADO);
    }

    glutPostRedisplay();
}

void wheel(int button, int direction, int x, int y) {
    dir = direction;
    if (action == 3 || action == 6) {
        aplicar_transformacao(action, objeto, mouseX, mouseY, dir, SELECIONADO);
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1':
            objeto = 1; //ponto
            break;
        case '2':
            objeto = 2; //linha
            break;
        case '3':
            objeto = 3; //pol�gono
            break;
        case 's':
            modo = 0; //selecionar
            objeto = -1;
            action = -1;
            break;
        case 'd':
            modo = 1; //desenhar
            action = -1;
            break;
        case 'x':
            if (excluir_ponto_selecionado(pontos));
            if (excluir_linha_selecionada(linhas));
            if (excluir_poligono_selecionado(poligonos));
            glutPostRedisplay();
            break;
        case 't':
            action = 2;
            break;
        case 'e':
            action = 3;
            break;
        case 'r':
            action = 4;
            break;
        case 'i':
            action = 5;
            aplicar_transformacao(action, objeto, mouseX, mouseY, 0, SELECIONADO);
            glutPostRedisplay();
            break;
        case 'c':
            action = 6;
            break;
        case 'p':
            modo = -1;
            objeto = -1;
            action = -1;
        default:
            objeto = -1; //nenhum
            modo = -1; //nenhum
            action = -1;
    }

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(5);

    glColor3f(0.0, 0.0, 0.0); // Preto
    glBegin(GL_LINES);
    glEnd();

    desenhar_pontos(pontos);
    desenhar_linhas(linhas);
    desenhar_poligonos(poligonos);

    if (desenhar_previa_linha(mouseX, mouseY));
    if (desenhar_previa_poligono(mouseX, mouseY));

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(screenWidth, screenHeight);
    glutCreateWindow("Pain(t)");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutPassiveMotionFunc(motion);
    glutMouseWheelFunc(wheel);

    glutMainLoop();

    destruction();

    return 0;
}
