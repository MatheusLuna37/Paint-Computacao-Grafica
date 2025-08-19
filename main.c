#include <GL/glut.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
// #include <windows.h>
#include "pontos.h"
#include "linhas.h"
#include "borracha.h"
#include "poligonos.h"

// Dimensões da tela
int screenWidth = 1280;
int screenHeight = 720;

// Instâncias de ponto, linha e polígono
Pontos pontos;
Pontos pontoSelecionado;
Linhas linhas;
Poligonos poligonos;
PoligonoEl *poligonoAtual = NULL;

// Para controle se é desenho de linha
float startX, startY;
int isDragging = 0;
const float DRAG_THRESHOLD = 0.005f; // distância mínima para considerar arraste

// Estado
bool pivotDefinido = false;
Pontos ponto_selecionado = NULL;
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

float mouseX, mouseY;

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(5.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    pontos = inicializar_pontos();
    linhas = inicializar_linhas();
    poligonos = inicializar_poligonos();

}

void mouse(int button, int state, int x, int y) {
    float mouseX = (float)x / glutGet(GLUT_WINDOW_WIDTH);
    float mouseY = 1.0f - (float)y / glutGet(GLUT_WINDOW_HEIGHT);

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Armazena posição inicial e marca que o botão foi pressionado
            startX = mouseX;
            startY = mouseY;
            isDragging = 1;
        }
        else if (state == GLUT_UP && isDragging) {
            // Verifica distância do arraste
            float dx = mouseX - startX;
            float dy = mouseY - startY;
            float dist = sqrtf(dx*dx + dy*dy);

            if (dist < DRAG_THRESHOLD) {
                // Clique rápido -> adiciona ponto
                adicionar_ponto(&pontos, mouseX, mouseY);
            } else {
                // Arraste -> adiciona linha
                adicionar_linha(&linhas, startX, startY, mouseX, mouseY);
            }
            isDragging = 0;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        pontoSelecionado = selecionar_ponto(mouseX, mouseY, pontos);
    }

    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        float mouseX = (float)x / glutGet(GLUT_WINDOW_WIDTH);
        float mouseY = 1.0f - (float)y / glutGet(GLUT_WINDOW_HEIGHT);

        apagar_ponto(&pontos, mouseX, mouseY, BORRACHA_TOLERANCIA);
        apagar_linha(&linhas, mouseX, mouseY, BORRACHA_TOLERANCIA);

        glutPostRedisplay();
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
            objeto = 3; //polígono
            break;
        case 's':
            modo = 0; //selecionar
            break;
        case 'd':
            modo = 1; //desenhar
            break;
        case 'r':
            modo = -1; //nenhum
            objeto = -1; //nenhum
            break;
        default:
            objeto = -1; //nenhum
            modo = -1; //nenhum
    }

}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(5);
    glColor3f(0.0, 0.0, 0.0); // Preto

    // Desenha linhas
    glColor3f(1.0, 0.0, 0.0); // vermelho para linhas
    desenhar_linhas(linhas);

    // Desenha pontos
    glColor3f(0.0, 0.0, 0.0); // preto para pontos
    desenhar_pontos(pontos);

    // Desenha polígonos
    glColor3f(0.0, 0.0, 1.0); // azul para polígonos
    desenhar_poligonos(poligonos);

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

    excluir_todos_pontos(&pontos);
    excluir_todas_linhas(&linhas);
    return 0;
}
