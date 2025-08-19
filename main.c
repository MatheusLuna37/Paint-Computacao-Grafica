#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include "pontos.h"

int screenWidth = 1280;
int screenHeight = 720;

Pontos *pontos;

// Estado
bool DESENHANDO_LINHA = false;
bool DESENHANDO_POLIGONO = false;
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
                if (!DESENHANDO_LINHA) {
                    DESENHANDO_LINHA = true;
                } else {
                    DESENHANDO_LINHA = false;
                }
            }
        } else if (modo == 0) {
            if (selecionar_ponto(mouseX, mouseY, pontos));
        }
        glutPostRedisplay();
    }
}

void motion(int button, int state, int x, int y) {

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

/* Callback de movimento do mouse com botão pressionado
void motion(int x, int y) {
    if (desenhando) {
        mouseX = x;
        mouseY = screenHeight-y;
        glutPostRedisplay(); // Re-renderiza
    }
}
*/

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(5);

    glColor3f(0.0, 0.0, 0.0); // Preto
    glBegin(GL_LINES);
    glEnd();

    desenhar_pontos(pontos);

    if (DESENHANDO_LINHA) {
        desenhar_linha_preview();
    }

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
    //glutMotionFunc(motion);
    //glutPassiveMotionFunc(motion);

    glutMainLoop();

    excluir_todos_pontos(pontos);
    return 0;
}
