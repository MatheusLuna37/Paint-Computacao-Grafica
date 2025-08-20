#ifndef LINHAS_H_INCLUDED
#define LINHAS_H_INCLUDED


#include "pontos.h"

typedef struct Linha {
    Ponto p1, p2;
} Linha;

typedef struct LinhaEl *Linhas;

Linhas *inicializar_linhas();
int add_linha(Linha linha, Linhas *linhas);
int excluir_todas_linhas(Linhas *linhas);
int desenhar_linhas(Linhas *linhas);
int excluir_linha_selecionada(Linhas *linhas);
int criar_linha(float mouseX, float mouseY, Linhas *linhas);
int desenhar_previa_linha(float mouseX, float mouseY, Linhas *linhas);
int selecionar_linha(float mouseX, float mouseY, Linhas *linhas);
unsigned int codificador(Ponto p, float xmin, float xmax, float ymin, float ymax);

#endif
