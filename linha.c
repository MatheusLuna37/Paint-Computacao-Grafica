#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <GL/glut.h>
#include "linhas.h"

Linhas SELECIONADA = NULL;
Ponto PIVO = {0,0};
int desenhando = 0;

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
        desenhando = 1;
        PIVO = {mouseX, mouseY};
        glBegin(GL_LINES)
    }
}

int desenhar_linhas(Linhas *linhas) {
    if (linhas == NULL) return 0;
    LinhaEl *aux = *linhas;
    while (aux != NULL) {
        if (aux == SELECIONADA) {
            glColor3f(0, 1, 0); // Cor para linha selecionada
        }
        glBegin(GL_LINES);
            glVertex2f(aux->p1.x, aux->p1.y);
            glVertex2f(aux->p2.x, aux->p2.y);
        glEnd();
        aux = aux->prox;
        glColor3f(0, 0, 0); // Reseta para a cor padrão
    }
    return 1;
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
