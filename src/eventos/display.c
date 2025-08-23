#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdbool.h>
#include <stdio.h>

#include "../../cabecalhos/eventos/display.h"
#include "../../cabecalhos/globais.h"

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
