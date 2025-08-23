#ifndef LINHAS_H
#define LINHAS_H

#include "../estruturas/pontos.h"

typedef struct {
    Pontos *pontos;
} Linha;

typedef struct LinhaEl *Linhas;

// Inicialização e manipulação
Linhas *inicializar_linhas();
int add_linha(Linha linha, Linhas *linhas);
void resetar_linha_selecionada();
Pontos get_linha_pontos();

// Criação e renderização
int criar_linha(float mouseX, float mouseY, Linhas *linhas);
int desenhar_previa_linha(float mouseX, float mouseY);
int cancelar_linha_atual();
int desenhar_linhas(Linhas *linhas);

// Seleção
int selecionar_linha(float mouseX, float mouseY, Linhas *linhas);

// Exclusão
int excluir_linha_selecionada(Linhas *linhas);
int excluir_todas_linhas(Linhas *linhas);

#endif // LINHAS_H