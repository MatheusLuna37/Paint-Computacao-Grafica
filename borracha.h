#ifndef BORRACHA_H
#define BORRACHA_H

#include "pontos.h"
#include "linhas.h"

// Tolerância para apagar (distância máxima do ponto ou linha)
#define BORRACHA_TOLERANCIA 0.02f

// Funções para apagar pontos e linhas próximos do mouse
void apagar_ponto(Pontos *pontos, float mouseX, float mouseY, float tolerancia);
void apagar_linha(Linhas *linhas, float mouseX, float mouseY, float tolerancia);

#endif
