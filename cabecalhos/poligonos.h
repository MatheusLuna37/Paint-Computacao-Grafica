#ifndef POLIGONOS_H
#define POLIGONOS_H

#include "pontos.h"

typedef struct {
    Pontos *pontos;
} Poligono;

typedef struct PoligonoEl *Poligonos;

// Inicialização e manipulação
Poligonos *inicializar_poligonos();
int add_poligono(Poligono poligono, Poligonos *poligonos);
Pontos get_poligono_pontos();

// Criação e edição
int add_vertice_poligono_atual(float mouseX, float mouseY, Poligonos *poligonos);
int finalizar_poligono_atual(Poligonos *poligonos);
int cancelar_poligono_atual();
int desenhar_previa_poligono(float mouseX, float mouseY);

// Renderização
int desenhar_poligonos(Poligonos *poligonos);

// Seleção e reset
void resetar_poligono_selecionado();
int selecionar_poligono(float mouseX, float mouseY, Poligonos *poligonos);

// Exclusão
int excluir_poligono_selecionado(Poligonos *poligonos);
int excluir_todos_poligonos(Poligonos *poligonos);

#endif // POLIGONOS_H