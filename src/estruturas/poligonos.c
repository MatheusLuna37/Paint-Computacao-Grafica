#include <stdio.h>
#include <stdlib.h>
//#include <windows.h>
#include <stdbool.h>
#include <GL/glut.h>
#include "../../cabecalhos/poligonos.h"

typedef struct PoligonoEl {
    Poligono poligono;
    struct PoligonoEl *prox;
} PoligonoEl;

static Poligonos POLIGONO_SELECIONADO = NULL;
static PoligonoEl *POLIGONO_EM_CONSTRUCAO = NULL;
static Ponto PRIMEIRO_PONTO = {0,0};
static Ponto ULTIMO_PONTO = {0,0};
static int desenhando = 0;
static float TOLERANCIA = 10;
static int transladando = 0;
static int rotacionando = 0;
static int escalando = 0;

Poligonos *inicializar_poligonos() {
    Poligonos *poligonos = (Poligonos *) malloc(sizeof(Poligonos));
    *poligonos = NULL;
    return poligonos;
}

// Adiciona um pol�gono j� pronto � lista principal (fun��o interna)
int add_poligono(Poligono poligono, Poligonos *poligonos) {
    if (poligonos == NULL) return 0;
    PoligonoEl *novo = (PoligonoEl *) malloc(sizeof(PoligonoEl));
    if (novo == NULL) return 0;
    novo->poligono = poligono;
    novo->prox = *poligonos;
    *poligonos = novo;
    return 1;
}

Pontos get_poligono_pontos() {
    if (POLIGONO_SELECIONADO == NULL) return NULL;
    return *(POLIGONO_SELECIONADO->poligono.pontos);
}

int add_vertice_poligono_atual(float mouseX, float mouseY, Poligonos *poligonos) {
    Ponto p = {mouseX, mouseY};
    if (!desenhando) {
        POLIGONO_EM_CONSTRUCAO = (PoligonoEl *) malloc(sizeof(PoligonoEl));
        if (POLIGONO_EM_CONSTRUCAO == NULL) return 0;
        POLIGONO_EM_CONSTRUCAO->poligono.pontos = inicializar_pontos();
        POLIGONO_EM_CONSTRUCAO->prox = NULL;
        add_ponto(p, POLIGONO_EM_CONSTRUCAO->poligono.pontos);
        ULTIMO_PONTO = p;
        PRIMEIRO_PONTO = p;
        desenhando = 1;
        return 1;
    }
    if (PRIMEIRO_PONTO.x >= mouseX-TOLERANCIA && PRIMEIRO_PONTO.x <= mouseX+TOLERANCIA &&
        PRIMEIRO_PONTO.y >= mouseY-TOLERANCIA && PRIMEIRO_PONTO.y <= mouseY+TOLERANCIA) {
        finalizar_poligono_atual(poligonos);
        return 0;
    }
    add_ponto(p, POLIGONO_EM_CONSTRUCAO->poligono.pontos);
    ULTIMO_PONTO = p;
    return 1;
}

int finalizar_poligono_atual(Poligonos *poligonos) {
    if (!desenhando) return 0;
    POLIGONO_EM_CONSTRUCAO->prox = *poligonos;
    *poligonos = POLIGONO_EM_CONSTRUCAO;
    POLIGONO_EM_CONSTRUCAO = NULL;
    desenhando = 0;
    return 1;
}

int cancelar_poligono_atual() {
    if (!desenhando) return 0;
    excluir_todos_pontos(POLIGONO_EM_CONSTRUCAO->poligono.pontos);
    free(POLIGONO_EM_CONSTRUCAO);
    POLIGONO_EM_CONSTRUCAO = NULL;
    desenhando = 0;
    return 1;
}

int desenhar_previa_poligono(float mouseX, float mouseY) {
    if (!desenhando) return 0;

    glColor3f(0.5, 0.5, 0.5); // Cor cinza para a pr�via
    glBegin(GL_LINE_STRIP);
        converter_vertices(POLIGONO_EM_CONSTRUCAO->poligono.pontos); // Reusa a fun��o para desenhar os v�rtices
    glEnd();
    // Desenha a linha do �ltimo ponto at� o mouse
    glBegin(GL_LINES);
        glVertex2f(ULTIMO_PONTO.x, ULTIMO_PONTO.y);
        glVertex2f(mouseX, mouseY);
    glEnd();

    glutPostRedisplay();
    return 1;
}

// Desenha todos os pol�gonos finalizados
int desenhar_poligonos(Poligonos *poligonos) {
    if (poligonos == NULL) return 0;
    PoligonoEl *aux = *poligonos;
    while (aux != NULL) {
        if (aux == POLIGONO_SELECIONADO) {
            glColor3f(0, 1, 0); // Cor para pol�gono selecionado
        }

        // Para desenhar o pol�gono, iteramos por sua lista de pontos
        glBegin(GL_LINE_LOOP); // GL_LINE_LOOP conecta o �ltimo ponto ao primeiro
            converter_vertices(aux->poligono.pontos);
        glEnd();

        aux = aux->prox;
        glColor3f(0, 0, 0); // Reseta para a cor padr�o
    }
    return 1;
}

void resetar_poligono_selecionado() {
    POLIGONO_SELECIONADO = NULL;
}

int selecionar_poligono(float mouseX, float mouseY, Poligonos *poligonos) {
    if (poligonos == NULL) return 0;
    int count = 0;
    PoligonoEl *buscador = *poligonos;
    while (buscador != NULL) {
        count = contar_arestas_atingidas(mouseX, mouseY, buscador->poligono.pontos);
        if (count % 2 == 1) {
            POLIGONO_SELECIONADO = buscador;
            return 1;
        }
        buscador = buscador->prox;
    }
    POLIGONO_SELECIONADO = NULL;
    return 0;
}

int excluir_poligono_selecionado(Poligonos *poligonos) {
    if (poligonos == NULL || POLIGONO_SELECIONADO == NULL) return 0;

    // Libera a mem�ria da lista de pontos ANTES de liberar o pol�gono
    excluir_todos_pontos(POLIGONO_SELECIONADO->poligono.pontos);

    if (POLIGONO_SELECIONADO == *poligonos) {
        *poligonos = POLIGONO_SELECIONADO->prox;
        free(POLIGONO_SELECIONADO);
        POLIGONO_SELECIONADO = NULL;
        return 1;
    }

    PoligonoEl *buscador = (*poligonos)->prox;
    PoligonoEl *anterior = *poligonos;
    while (buscador != NULL && buscador != POLIGONO_SELECIONADO) {
        anterior = buscador;
        buscador = buscador->prox;
    }

    if (buscador == NULL) {
        POLIGONO_SELECIONADO = NULL;
        return 0;
    }

    anterior->prox = buscador->prox;
    free(buscador);
    POLIGONO_SELECIONADO = NULL;
    return 1;
}

int excluir_todos_poligonos(Poligonos *poligonos) {
    if (poligonos == NULL) return 0;
    PoligonoEl *aux = *poligonos;
    while (aux != NULL) {
        *poligonos = aux->prox;
        excluir_todos_pontos(aux->poligono.pontos);
        free(aux);
        aux = *poligonos;
    }
    free(*poligonos);
    return 1;
}
