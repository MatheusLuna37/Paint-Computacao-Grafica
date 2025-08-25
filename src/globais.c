#include <stdio.h>
#include <stdlib.h>
#include "../cabecalhos/globais.h"
#include "../cabecalhos/estruturas/pontos.h"
#include "../cabecalhos/estruturas/linhas.h"
#include "../cabecalhos/estruturas/poligonos.h"

float **TRANSLACAO = NULL;
float **ROTACAO = NULL;
float **ESCALA = NULL;
float **CISALHAMENTO = NULL;
float **ROTACAO_CENTROIDE = NULL;
float **ESCALA_CENTROIDE = NULL;
float **CISALHAMENTO_CENTROIDE = NULL;
float **PONTO_HOMOGENEO = NULL;
float **RESULTADO = NULL;
float **RESULTADOP = NULL;
float cor_animado[3] = {1.0f, 0.0f, 0.0f};
int objeto = -1;
int modo = -1;
int action = -1;
Pontos SELECIONADO = NULL;
Pontos *pontos = NULL;
Linhas *linhas = NULL;
Poligonos *poligonos = NULL;
Poligonos poligono_animado = NULL;
float mouseX = 0;
float mouseY = 0;
int esquerdo = 0;
int direito = 0;
int dir = 0;

int screenWidth = 1280;
int screenHeight = 720;
