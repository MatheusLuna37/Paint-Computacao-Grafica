#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include "../../cabecalhos/eventos/teclado.h"
#include "../../cabecalhos/globais.h"
#include "../../cabecalhos/estruturas/pontos.h"
#include "../../cabecalhos/transformacoes/transformar.h"
#include "../../cabecalhos/eventos/arquivos.h"

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

        case 'u':
            salvar_arquivo("objetos.txt", pontos, linhas, poligonos);
            printf("Objetos salvos em objetos.txt\n");
            break;
        case 'k':
            carregar_arquivo("objetos.txt", pontos, linhas, poligonos);
            glutPostRedisplay();
            printf("Objetos carregados de objetos.txt\n");
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
