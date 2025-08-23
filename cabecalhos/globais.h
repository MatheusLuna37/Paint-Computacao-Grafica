#ifndef GLOBAIS_H_INCLUDED
#define GLOBAIS_H_INCLUDED

#include "pontos.h"
#include "linhas.h"
#include "poligonos.h"

extern float **TRANSLACAO;
extern float **ROTACAO;
extern float **ESCALA;
extern float **CISALHAMENTO;
extern float **ROTACAO_CENTROIDE;
extern float **ESCALA_CENTROIDE;
extern float **CISALHAMENTO_CENTROIDE;
extern float **PONTO_HOMOGENEO;
extern float **RESULTADO;
extern float **RESULTADOP;
extern int objeto;
extern int modo;
extern int action;
extern Pontos *pontos;
extern Linhas *linhas;
extern Poligonos *poligonos;
extern float mouseX, mouseY;
extern int esquerdo;
extern int direito;
extern int dir;
extern Pontos SELECIONADO;
extern int screenWidth, screenHeight;

extern int screenWidth;
extern int screenHeight;

#endif
