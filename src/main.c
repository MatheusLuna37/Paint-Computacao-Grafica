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
#include "../cabecalhos/arquivos.h"
#include "../cabecalhos/teclado.h"
#include "../cabecalhos/init.h"
#include "../cabecalhos/display.h"
#include "../cabecalhos/motion.h"

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
