#include <GL/glut.h>
#include <stdbool.h>
#include <windows.h>

int screenWidth = 1280;
int screenHeight = 720;

// Estado
bool pivotDefinido = false;
bool desenhando = false;

int objeto = -1;

// Coordenadas
float pivotX, pivotY;
float mouseX, mouseY;

// Converte coordenadas da janela para coordenadas OpenGL (-1 a 1)
float toGLX(int x, int width) {
    return (2.0f * x / width) - 1.0f;
}

float toGLY(int y, int height) {
    return 1.0f - (2.0f * y / height);
}

// Callback de clique do mouse
void mouse(int button, int state, int x, int y) {
    /*
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        for (int i=0; i<3; i++) {
            if (x > (screenWidth/25)*(i+3) && x < (screenWidth/25)*(i+4) && y > screenHeight*19/20 && y < screenHeight) {
                desenhando = true;
                objeto = i;
            }
        }
    }
    */

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!pivotDefinido) {
            // Primeiro clique: define o pivô
            pivotX = x;
            pivotY = screenHeight-y;
            pivotDefinido = true;
            desenhando = true;
        } else {
            // Segundo clique: desenha linha definitiva
            desenhando = false;
            glBegin(GL_LINES);
                glVertex2f(pivotX, pivotY);
                glVertex2f(mouseX, mouseY);
            glEnd();
            glFlush();
            pivotDefinido = false; // Reseta para próximo desenho
        }
    }
}

// Callback de movimento do mouse com botão pressionado
void motion(int x, int y) {
    if (desenhando) {
        mouseX = x;
        mouseY = screenHeight-y;
        glutPostRedisplay(); // Re-renderiza
    }
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Linha de pré-visualização
    if (desenhando) {
        glColor3f(1.0, 0.0, 0.0); // Vermelho
        glBegin(GL_LINES);
            glVertex2f(pivotX, pivotY);
            glVertex2f(mouseX, mouseY);
        glEnd();
    }

    glColor3f(0.0, 0.0, 0.0); // Preto
    glBegin(GL_LINES);
        glVertex2f(0, screenHeight*19/20);
        glVertex2f(screenWidth, screenHeight*19/20);
        glVertex2f(0, screenHeight-1);
        glVertex2f(screenWidth, screenHeight-1);
    glEnd();

    for(int i=0; i<4; i++) {
        glBegin(GL_LINES);
            glVertex2f((screenWidth/25)*(i+3), screenHeight*19/20);
            glVertex2f((screenWidth/25)*(i+3), screenHeight);
        glEnd();

        switch (i) {
            case 0:
                glBegin(GL_POINTS);
                    glVertex2f((screenWidth/25)*(i+3.5), screenHeight*39/40);
                glEnd();
                break;
            case 1:
                glBegin(GL_LINES);
                    glVertex2f((screenWidth/25)*(i+3.5)-screenWidth/100, screenHeight*39/40+screenWidth/100);
                    glVertex2f((screenWidth/25)*(i+3.5)+screenWidth/100, screenHeight*39/40-screenWidth/100);
                glEnd();
                break;
            case 2:
                glBegin(GL_POLYGON);
                    glVertex2f((screenWidth/25)*(i+3.5)-screenWidth/100, screenHeight*39/40+screenWidth/250);
                    glVertex2f((screenWidth/25)*(i+3.5), screenHeight*39/40+screenWidth/100);
                    glVertex2f((screenWidth/25)*(i+3.5)+screenWidth/100, screenHeight*39/40+screenWidth/250);
                    glVertex2f((screenWidth/25)*(i+3.5)+screenWidth/150, screenHeight*39/40-screenWidth/100);
                    glVertex2f((screenWidth/25)*(i+3.5)-screenWidth/150, screenHeight*39/40-screenWidth/100);
                glEnd();
            default:
                break;
        }
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
    glutMotionFunc(motion);
    glutPassiveMotionFunc(motion);

    glutMainLoop();

    return 0;
}
