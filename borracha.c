#include <stdlib.h>
#include <math.h>
#include "borracha.h"

// Apaga um ponto próximo do mouse
void apagar_ponto(Pontos *pontos, float mouseX, float mouseY, float tolerancia) {
    if (!pontos || !*pontos) return;

    PontoEl *prev = NULL;
    PontoEl *curr = *pontos;

    while (curr != NULL) {
        float dx = curr->x - mouseX;
        float dy = curr->y - mouseY;
        float dist = sqrtf(dx*dx + dy*dy);

        if (dist <= tolerancia) {
            if (prev)
                prev->prox = curr->prox;
            else
                *pontos = curr->prox;

            free(curr);
            return; // apaga apenas o primeiro encontrado
        }

        prev = curr;
        curr = curr->prox;
    }
}

// Apaga uma linha próxima do mouse
void apagar_linha(Linhas *linhas, float mouseX, float mouseY, float tolerancia) {
    if (!linhas || !*linhas) return;

    LinhaEl *prev = NULL;
    LinhaEl *curr = *linhas;

    while (curr != NULL) {
        float dx = curr->x2 - curr->x1;
        float dy = curr->y2 - curr->y1;
        float d = fabsf(dy*mouseX - dx*mouseY + curr->x2*curr->y1 - curr->y2*curr->x1) / sqrtf(dx*dx + dy*dy);

        if (d <= tolerancia) {
            if (prev)
                prev->prox = curr->prox;
            else
                *linhas = curr->prox;

            free(curr);
            return; // apaga apenas a primeira linha encontrada
        }

        prev = curr;
        curr = curr->prox;
    }
}
