#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdbool.h>
#include <stdio.h>

#include "../../cabecalhos/transformacoes/motion.h"
#include "../../cabecalhos/transformacoes/transformar.h"
#include "../../cabecalhos/globais.h"

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
