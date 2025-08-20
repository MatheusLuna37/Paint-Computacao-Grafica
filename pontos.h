#ifndef PONTOS_H_INCLUDED
#define PONTOS_H_INCLUDED

typedef struct Ponto {
    float x, y;
} Ponto;

typedef struct PontoEl *Pontos;

Pontos *inicializar_pontos();
int add_ponto(Ponto ponto, Pontos *pontos);
int excluir_todos_pontos(Pontos *pontos);
int desenhar_pontos(Pontos *pontos);
int selecionar_ponto(float mouseX, float mouseY, Pontos *pontos);
int excluir_ponto_selecionado(Pontos *pontos);
int converter_vertices(Pontos *pontos);
int contar_arestas_atingidas (float mouseX, float mouseY, Pontos *pontos);

#endif
