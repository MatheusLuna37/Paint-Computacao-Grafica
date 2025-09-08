#ifndef ANIMAR_H
#define ANIMAR_H

#include <stdbool.h>
#include "../estruturas/pontos.h"
#include "../estruturas/linhas.h"
#include "../estruturas/poligonos.h"

void hsv_para_rgb(float h, float s, float v, float rgb[3]);
void sortear_cor(float cor[3]);
void reiniciar_animacao();
void excluir_tudo();
void calcular_bounding_box
    (
        Pontos pts, 
        float *minX, 
        float *maxX, 
        float *minY, 
        float *maxY
    );
bool checar_colisao_poligonos
    (
        Poligonos p, 
        float minX, 
        float maxX, 
        float minY, 
        float maxY
    );
bool comer_ponto
    (
        float minX, 
        float maxX, 
        float minY, 
        float maxY, 
        Poligonos p
    );
bool comer_linha
    (
        float minX, 
        float maxX, 
        float minY, 
        float maxY, 
        Poligonos p
    );
void desenhar_game_over();
bool colisao_bbox(float minA, float maxA, float minB, float maxB);
void iniciar_animacao();
void parar_animacao();
void animar(int value);

#endif