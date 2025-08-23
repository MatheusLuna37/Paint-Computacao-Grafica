#include <stdio.h>
#include <stdlib.h>
#include "../cabecalhos/globais.h"
#include "../cabecalhos/pontos.h"
#include "../cabecalhos/linhas.h"
#include "../cabecalhos/poligonos.h"

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
int objeto = -1;
int modo = -1;
int action = -1;
Pontos *pontos = NULL;
Linhas *linhas = NULL;
Poligonos *poligonos = NULL;
float mouseX = 0;
float mouseY = 0;
int esquerdo = 0;
int direito = 0;
int dir = 0;
Pontos SELECIONADO = NULL;

int screenWidth = 1280;
int screenHeight = 720;
