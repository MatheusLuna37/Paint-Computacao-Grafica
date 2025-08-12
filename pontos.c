#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <GL/glut.h>
#include "pontos.h"

float TOLERANCIA = 0.2;

typedef struct PontoEl {
    Ponto ponto;
    bool selecionado;
    struct PontoEl *prox;
} PontoEl;


Pontos *inicializar_pontos() {
    Pontos *pontos = (Pontos *) malloc(sizeof(Pontos));
    *pontos = NULL;
    return pontos;
}

int add_ponto(Ponto ponto, Pontos *pontos) {
    if (pontos == NULL) return 0;
    PontoEl *novo = (PontoEl *) malloc(sizeof(PontoEl));
    if (novo == NULL) return 0;
    novo->ponto = ponto;
    novo->selecionado = false;
    novo->prox = *pontos;
    *pontos = novo;
    return 1;
}

int desenhar_pontos(Pontos *pontos) {
    if (pontos == NULL) return 0;
    PontoEl *aux = *pontos;
    while (aux != NULL) {
        if (aux->selecionado) {
            glColor3f(0, 1, 0);
        }
        glBegin(GL_POINTS);
            glVertex2f(aux->ponto.x, aux->ponto.y);
        glEnd();
        aux = aux->prox;
        glColor3f(0, 0, 0);
    }
    return 1;
}

/*
NULL: não conseguiu selecionar nenhum ponto;
Caso contrário: retorna o ponteiro para o elemento anterior ao ponto selecionado, exceto se foi encontrado na primeira posição;
(Para utilizar esse método, devemos sempre verificar primeiro se retorna NULL ou se anterior->prox = NULL);

modo: 0-> retorna ponteiro para o selecionado, 1-> retorna ponteiro para o anterior ao selecionado
*/
Pontos selecionar_ponto(float mouseX, float mouseY, Pontos *pontos) {
    if (pontos == NULL) return NULL;
    PontoEl* buscador = *pontos;
    PontoEl* anterior = *pontos;
    while (buscador != NULL) {
        if (buscador->ponto.x >= mouseX-TOLERANCIA && buscador->ponto.x <= mouseX+TOLERANCIA &&
            buscador->ponto.y >= mouseY-TOLERANCIA && buscador->ponto.y <= mouseY+TOLERANCIA) {
            buscador->selecionado = true;
            break;
        }
        anterior = buscador;
        buscador = buscador->prox;
    }
    return anterior;
}

/*
int teste_selecionar_ponto(float mouse_x, float mouse_y, Pontos *pontos) {
    PontoEl *resultado = selecionar_ponto(mouse_x, mouse_y, pontos);
    if (resultado == NULL) return 0;
    return 1;
}*/

int excluir_todos_pontos(Pontos *pontos) {
    if (pontos == NULL) return 0;
    PontoEl *aux = *pontos;
    while (aux != NULL) {
        *pontos = aux->prox;
        free(aux);
        aux = *pontos;
    }
    free(*pontos);
    return 1;
}

