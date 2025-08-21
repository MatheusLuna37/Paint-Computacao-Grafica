#include <stdlib.h>
#include <GL/glut.h>
#include "poligonos.h"
#include "pontos.h"

Poligonos inicializar_poligonos() {
    return NULL;
}

void adicionar_poligono(Poligonos *lista) {
    PoligonoEl *novo = (PoligonoEl*)malloc(sizeof(PoligonoEl));
    if (!novo) return;

    novo->vertices = inicializar_pontos();
    novo->prox = *lista;
    *lista = novo;
}

void adicionar_vertice(PoligonoEl *poligono, float x, float y) {
    if (!poligono) return;
    adicionar_ponto(&poligono->vertices, x, y);
}

int desenhar_poligonos(Poligonos lista) {
    PoligonoEl *aux = lista;
    while (aux != NULL) {
        if (aux->vertices != NULL) {
            glBegin(GL_LINE_LOOP); // conecta todos os pontos e fecha o polígono
            PontoEl *v = aux->vertices;
            while (v != NULL) {
                glVertex2f(v->x, v->y);
                v = v->prox;
            }
            glEnd();
        }
        aux = aux->prox;
    }
    return 1;
}

int excluir_todos_poligonos(Poligonos *lista) {
    if (!lista || !*lista) return 0;

    PoligonoEl *aux = *lista;
    while (aux != NULL) {
        excluir_todos_pontos(&aux->vertices); // apaga todos os vértices
        PoligonoEl *tmp = aux;
        aux = aux->prox;
        free(tmp);
    }

    *lista = NULL;
    return 1;
}
