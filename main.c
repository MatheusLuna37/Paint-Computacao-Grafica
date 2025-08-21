#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include "pontos.h"
#include "linhas.h"
#include "poligonos.h"

int screenWidth = 1280;
int screenHeight = 720;

Pontos *pontos;
Linhas *linhas;
Poligonos *poligonos;

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
3: polígono
*/
int action = -1;

float mouseX, mouseY;

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);

    pontos = inicializar_pontos();
    linhas = inicializar_linhas();
    poligonos = inicializar_poligonos();
}

// Callback de clique do mouse
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
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
            resetar_linha_selecionada();
            resetar_poligono_selecionado();
            resetar_ponto_selecionado();
            if (selecionar_ponto(mouseX, mouseY, pontos)) {}
            else if (selecionar_linha(mouseX, mouseY, linhas)) {}
            else if (selecionar_poligono(mouseX, mouseY, poligonos)) {}
            if (action == 2) {
                parar_rotacao();
                parar_rotacao_linha();
                parar_escala_linha();
                iniciar_translado();
                iniciar_translado_linha();
            } else if (action == 3) {
                parar_translado();
                parar_translado_linha();
                parar_rotacao();
                parar_rotacao_linha();
                iniciar_escala_linha();
            } else if (action == 4) {
                parar_translado();
                parar_translado_linha();
                parar_escala_linha();
                iniciar_rotacao();
                iniciar_rotacao_linha();
            }
        }
        glutPostRedisplay();
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        parar_translado();
        parar_rotacao(mouseX, mouseY);
        parar_translado_linha();
        parar_rotacao_linha();
        parar_escala_linha();
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if (modo == 1) {
            if (objeto == 3) {
                if (cancelar_poligono_atual());
            }
        }
    }
}

void motion(int x, int y) {
    mouseX = x;
    mouseY = screenHeight - y;

    if (transladar_selecionado(mouseX, mouseY));
    if (rotacionar_selecionado(mouseX, mouseY));
    if (transladar_selecionada(mouseX, mouseY));
    if (rotacionar_selecionada(mouseX, mouseY));
    if (escalar_selecionada(mouseX, mouseY));

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
            objeto = 3; //polígono
            break;
        case 's':
            modo = 0; //selecionar
            break;
        case 'd':
            modo = 1; //desenhar
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
        case 'c':
            action = 5;
            break;
        case 'p':
            modo = -1;
            objeto = -1;
            action = -1;
        default:
            objeto = -1; //nenhum
            modo = -1; //nenhum
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

    if (desenhar_previa_linha(mouseX, mouseY, linhas));
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

    glutMainLoop();

    excluir_todos_pontos(pontos);
    excluir_todas_linhas(linhas);
    excluir_todos_poligonos(poligonos);
    return 0;
}
