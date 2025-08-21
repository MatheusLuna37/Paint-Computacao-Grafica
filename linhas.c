#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <GL/glut.h>
#include <math.h>
#include "pontos.h"
#include "linhas.h"
#include "matrizes.h"

static Linhas LINHA_SELECIONADA = NULL;
static Ponto PIVO = {0,0};
static float TOLERANCIA = 10;
static int desenhando = 0;
static int transladando = 0;
static int rotacionando = 0;
static int escalando = 0;

typedef struct LinhaEl {
    Linha linha;
    struct LinhaEl *prox;
} LinhaEl;

Linhas *inicializar_linhas() {
    Linhas *linhas = (Linhas *) malloc(sizeof(Linhas));
    *linhas = NULL;
    return linhas;
}

int add_linha(Linha linha, Linhas *linhas) {
    if (linhas == NULL) return 0;
    LinhaEl *novo = (LinhaEl *) malloc(sizeof(LinhaEl));
    if (novo == NULL) return 0;
    novo->linha = linha;
    novo->prox = *linhas;
    *linhas = novo;
    return 1;
}

void resetar_linha_selecionada() {
    LINHA_SELECIONADA = NULL;
}

Pontos get_linha_pontos() {
    if (LINHA_SELECIONADA == NULL) return NULL;
    return *(LINHA_SELECIONADA->linha.pontos);
}

int criar_linha(float mouseX, float mouseY, Linhas *linhas) {
    if (!desenhando) {
        PIVO = (Ponto){mouseX, mouseY};
        desenhando = 1;
        return 1;
    }
    desenhando = 0;
    Pontos *pontos = inicializar_pontos();
    add_ponto(PIVO, pontos);
    add_ponto((Ponto){mouseX, mouseY}, pontos);
    add_linha((Linha){pontos}, linhas);
    return 1;
}

int desenhar_previa_linha(float mouseX, float mouseY) {
    if (!desenhando) return 0;
    glBegin(GL_LINES);
        glVertex2f(PIVO.x, PIVO.y);
        glVertex2f(mouseX, mouseY);
    glEnd();
    glutPostRedisplay();
    return 1;
}

int cancelar_linha_atual() {
    if (!desenhando) return 0;
    desenhando = 0;
    return 1;
}

int desenhar_linhas(Linhas *linhas) {
    if (linhas == NULL) return 0;
    LinhaEl *aux = *linhas;
    while (aux != NULL) {
        if (aux == LINHA_SELECIONADA) {
            glColor3f(0, 1, 0); // Cor para linha selecionada
        }
        glBegin(GL_LINES);
            converter_vertices(aux->linha.pontos);
        glEnd();
        aux = aux->prox;
        glColor3f(0, 0, 0); // Reseta para a cor padr�o
    }
    return 1;
}

int selecionar_linha(float mouseX, float mouseY, Linhas *linhas) {
    if (linhas == NULL) return 0;
    float xmin = mouseX-TOLERANCIA, ymin = mouseY-TOLERANCIA, xmax = mouseX+TOLERANCIA, ymax = mouseY+TOLERANCIA;
    LinhaEl* buscador = *linhas;
    while (buscador != NULL) {
        if(avaliador_de_linha(*(buscador->linha.pontos), xmin, xmax, ymin, ymax)) {
            LINHA_SELECIONADA = buscador;
            return 1;
        }
        buscador = buscador->prox;
    }
    LINHA_SELECIONADA = NULL;
    return 0;
}

// NOVO: Fun��o para excluir a linha atualmente em LINHA_SELECIONADA
int excluir_linha_selecionada(Linhas *linhas) {
    if (linhas == NULL) return 0;
    if (LINHA_SELECIONADA == NULL) return 0;

    excluir_todos_pontos(LINHA_SELECIONADA->linha.pontos);

    // Caso 1: A linha selecionada � o primeiro elemento da lista
    if (LINHA_SELECIONADA == *linhas) {
        *linhas = LINHA_SELECIONADA->prox;
        free(LINHA_SELECIONADA);
        LINHA_SELECIONADA = NULL;
        return 1;
    }

    // Caso 2: A linha selecionada est� no meio ou no fim da lista
    LinhaEl *buscador = (*linhas)->prox;
    LinhaEl *anterior = *linhas;
    while (buscador != NULL && buscador != LINHA_SELECIONADA) {
        anterior = buscador;
        buscador = buscador->prox;
    }

    // Se n�o encontrou a linha na lista (salvaguarda)
    if (buscador == NULL) {
        LINHA_SELECIONADA = NULL;
        return 0;
    }

    // Remove o elemento da lista
    anterior->prox = buscador->prox;
    free(buscador);
    LINHA_SELECIONADA = NULL;
    return 1;
}

int excluir_todas_linhas(Linhas *linhas) {
    if (linhas == NULL) return 0;
    LinhaEl *aux = *linhas;
    while (aux != NULL) {
        excluir_todos_pontos(aux->linha.pontos);
        *linhas = aux->prox;
        free(aux);
        aux = *linhas;
    }
    free(*linhas);
    return 1;
}
