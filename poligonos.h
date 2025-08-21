#ifndef POLIGONOS_H_INCLUDED
#define POLIGONOS_H_INCLUDED

#include "pontos.h"

typedef struct Poligono {
    Pontos *pontos;
} Poligono;

typedef struct PoligonoEl *Poligonos;

Poligonos *inicializar_poligonos();
int excluir_todos_poligonos(Poligonos *poligonos);
int desenhar_poligonos(Poligonos *poligonos);
int excluir_poligono_selecionado(Poligonos *poligonos);

int add_vertice_poligono_atual(float mouseX, float mouseY, Poligonos *poligonos);
int finalizar_poligono_atual(Poligonos *poligonos);
int desenhar_previa_poligono(float mouseX, float mouseY);
int cancelar_poligono_atual();
int selecionar_poligono(float mouseX, float mouseY, Poligonos *poligonos);
void resetar_poligono_selecionado();

#endif
