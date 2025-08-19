#include <stdlib.h>
// #include <windows.h>
#include <GL/glut.h>
#include "pontos.h"

Pontos inicializar_pontos() {
    return NULL;
}

void adicionar_ponto(Pontos *lista, float x, float y) {
    PontoEl *novo = (PontoEl*)malloc(sizeof(PontoEl));
    novo->x = x;
    novo->y = y;
    novo->prox = *lista;
    *lista = novo;
}

int desenhar_pontos(Pontos lista) {
    PontoEl *p = lista;
    glPointSize(5.0);
    glBegin(GL_POINTS);
    while (p != NULL) {
        glVertex2f(p->x, p->y);
        p = p->prox;
    }
    glEnd();
    return 1;
}

/*
NULL: não conseguiu selecionar nenhum ponto;
Caso contrário: retorna o ponteiro para o elemento anterior ao ponto selecionado, exceto se foi encontrado na primeira posição;
(Para utilizar esse método, devemos sempre verificar primeiro se retorna NULL ou se anterior->prox = NULL);

modo: 0-> retorna ponteiro para o selecionado, 1-> retorna ponteiro para o anterior ao selecionado
*/
Pontos selecionar_ponto(float mouseX, float mouseY, Pontos lista) {
    PontoEl *p = lista;
    while (p != NULL) {
        if ((mouseX >= p->x - 0.05 && mouseX <= p->x + 0.05) &&
            (mouseY >= p->y - 0.05 && mouseY <= p->y + 0.05)) {
            return p;
        }
        p = p->prox;
    }
    return NULL;
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

