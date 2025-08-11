#include <stdio.h>
#include <stdlib.h>
#include "pontos.h"

float TOLERANCIA = 0.2;

typedef struct PontoEl {
    Ponto ponto;
    PontoEl *prox;
} PontoEl;


*Pontos inicializar_pontos() {
    Pontos *pontos = (Pontos *) malloc(sizeof(Pontos));
    *pontos = NULL;
    return pontos;
}

int add_ponto(Ponto ponto, Pontos *pontos) {
    if (pontos == NULL) return 0;
    PontoEl *novo = (PontoEl *) malloc(sizeof(PontoEl));
    if (novo == NULL) return 0;
    novo->ponto = ponto;
    novo->prox = *pontos;
    *pontos = novo;
    return 1;
}

/*
NULL: não conseguiu selecionar nenhum ponto;
Caso contrário: retorna o ponteiro para o elemento anterior ao ponto selecionado, exceto se foi encontrado na primeira posição;
(Para utilizar esse método, devemos sempre verificar primeiro se retorna NULL ou se anterior->prox = NULL);
*/
PontoEl* selecionar_ponto(float mouse_x, float mouse_y, Pontos *pontos) {
    if (pontos == NULL) return NULL;
    PontoEl* buscador = *pontos;
    PontoEl* anterior = *pontos;
    while (*buscador != NULL) {
        if (buscador->ponto.x >= mouse_x-TOLERANCIA && buscador->ponto.x <= mouse_x+TOLERANCIA &&
            buscador->ponto.y >= mouse_y-TOLERANCIA && buscador->ponto.y <= mouse_y+TOLERANCIA) {
            break;
        }
        anterior = buscador;
        buscador = buscador->prox;
    }
    return anterior;
}

int teste_selecionar_ponto(float mouse_x, float mouse_y, Pontos *pontos) {
    PontoEl *resultado = selecionar_ponto(mouse_x, mouse_y, pontos);
    if (resultado == NULL) return 0;
    return 1;
}

