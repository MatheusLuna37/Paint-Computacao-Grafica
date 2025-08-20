#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <GL/glut.h>
#include "linhas.h"

static Linhas SELECIONADA = NULL;
static Ponto PIVO = {0,0};
static float TOLERANCIA = 10;
static int desenhando = 0;

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

int criar_linha(float mouseX, float mouseY, Linhas *linhas) {
    if (!desenhando) {
        PIVO = (Ponto){mouseX, mouseY};
        desenhando = 1;
        return 1;
    }
    desenhando = 0;
    add_linha((Linha){(Ponto){PIVO.x, PIVO.y}, (Ponto){mouseX, mouseY}}, linhas);
    return 1;
}

int desenhar_previa_linha(float mouseX, float mouseY, Linhas *linhas) {
    if (!desenhando) return 0;
    glBegin(GL_LINES);
        glVertex2f(PIVO.x, PIVO.y);
        glVertex2f(mouseX, mouseY);
    glEnd();
    glutPostRedisplay();
    return 1;
}

int desenhar_linhas(Linhas *linhas) {
    if (linhas == NULL) return 0;
    LinhaEl *aux = *linhas;
    while (aux != NULL) {
        if (aux == SELECIONADA) {
            glColor3f(0, 1, 0); // Cor para linha selecionada
        }
        glBegin(GL_LINES);
            glVertex2f(aux->linha.p1.x, aux->linha.p1.y);
            glVertex2f(aux->linha.p2.x, aux->linha.p2.y);
        glEnd();
        aux = aux->prox;
        glColor3f(0, 0, 0); // Reseta para a cor padrão
    }
    return 1;
}

unsigned int codificador(Ponto p, float xmin, float xmax, float ymin, float ymax) {
    unsigned int codigo = 0b0000;
    if (p.x < xmin) {
        codigo |= 0b1000;
    } else if (p.x > xmax) {
        codigo |= 0b0100;
    }
    if (p.y < ymin) {
        codigo |= 0b0010;
    } else if (p.y > ymax) {
        codigo |= 0b0001;
    }
    return codigo;
}

int avaliar_linha(Ponto p1, Ponto p2, float xmin, float xmax, float ymin, float ymax) {
    unsigned int codigo1, codigo2, and_codigos, esq, dir, ab, ac;
    codigo1 = codificador(p1, xmin, xmax, ymin, ymax);
    codigo2 = codificador(p2, xmin, xmax, ymin, ymax);
    and_codigos = codigo1 & codigo2;
    esq = codigo1 & 0b1000;
    dir = codigo1 & 0b0100;
    ab = codigo1 & 0b0010;
    ac = codigo1 & 0b0001;
    if (codigo1 == 0b0000 || codigo2 == 0b0000) {
        return 1;
    } else if (and_codigos == 0b0000) {
        if (esq) {
            return avaliar_linha((Ponto){xmin, p1.y + (xmin-p1.x)*(p2.y-p1.y)/(p2.x-p1.x)}, p2, xmin, xmax, ymin, ymax);
        } else if (dir) {
            return avaliar_linha((Ponto){xmax, p1.y + (xmax-p1.x)*(p2.y-p1.y)/(p2.x-p1.x)}, p2, xmin, xmax, ymin, ymax);
        } else if (ab) {
            return avaliar_linha((Ponto){p1.x + (ymin-p1.y)*(p2.x-p1.x)/(p2.y-p1.y), ymin}, p2, xmin, xmax, ymin, ymax);
        } else {
            return avaliar_linha((Ponto){p1.x + (ymax-p1.y)*(p2.x-p1.x)/(p2.y-p1.y), ymax}, p2, xmin, xmax, ymin, ymax);
        }
    }
    return 0;
}

int selecionar_linha(float mouseX, float mouseY, Linhas *linhas) {
    if (linhas == NULL) return 0;
    float xmin = mouseX-TOLERANCIA, ymin = mouseY-TOLERANCIA, xmax = mouseX+TOLERANCIA, ymax = mouseY+TOLERANCIA;
    LinhaEl* buscador = *linhas;
    while (buscador != NULL) {
        if(avaliar_linha(buscador->linha.p1, buscador->linha.p2, xmin, xmax, ymin, ymax)) {
            SELECIONADA = buscador;
            return 1;
        }
        buscador = buscador->prox;
    }
    SELECIONADA = NULL;
    return 0;
}

// NOVO: Função para excluir a linha atualmente em SELECIONADA
int excluir_linha_selecionada(Linhas *linhas) {
    if (linhas == NULL) return 0;
    if (SELECIONADA == NULL) return 0;

    // Caso 1: A linha selecionada é o primeiro elemento da lista
    if (SELECIONADA == *linhas) {
        *linhas = SELECIONADA->prox;
        free(SELECIONADA);
        SELECIONADA = NULL;
        return 1;
    }

    // Caso 2: A linha selecionada está no meio ou no fim da lista
    LinhaEl *buscador = (*linhas)->prox;
    LinhaEl *anterior = *linhas;
    while (buscador != NULL && buscador != SELECIONADA) {
        anterior = buscador;
        buscador = buscador->prox;
    }

    // Se não encontrou a linha na lista (salvaguarda)
    if (buscador == NULL) {
        SELECIONADA = NULL;
        return 0;
    }

    // Remove o elemento da lista
    anterior->prox = buscador->prox;
    free(buscador);
    SELECIONADA = NULL;
    return 1;
}

int excluir_todas_linhas(Linhas *linhas) {
    if (linhas == NULL) return 0;
    LinhaEl *aux = *linhas;
    while (aux != NULL) {
        *linhas = aux->prox;
        free(aux);
        aux = *linhas;
    }
    free(*linhas);
    return 1;
}
