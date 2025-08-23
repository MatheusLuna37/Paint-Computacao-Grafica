#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdbool.h>
#include <stdio.h>
//#include <windows.h>
#include "../cabecalhos/init.h"
#include "../cabecalhos/globais.h"
#include "../cabecalhos/estruturas/pontos.h"
#include "../cabecalhos/estruturas/linhas.h"
#include "../cabecalhos/estruturas/poligonos.h"
#include "../cabecalhos/estruturas/matrizes.h"
#include "../cabecalhos/eventos/arquivos.h"
#include "../cabecalhos/eventos/teclado.h"
#include "../cabecalhos/eventos/display.h"
#include "../cabecalhos/transformacoes/transformar.h"
#include "../cabecalhos/transformacoes/motion.h"

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
