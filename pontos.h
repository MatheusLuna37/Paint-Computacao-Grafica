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
int transladar_selecionado(float mouseX, float mouseY);
void iniciar_translado();
void parar_translado();
int rotacionar_selecionado(float mouseX, float mouseY);
void iniciar_rotacao();
void parar_rotacao();
float **matriz_translacao(float tx, float ty);
float **matriz_rotacao(float s, float c);
float **ponto_homogeneo(Ponto p);
void resetar_ponto_selecionado();

#endif
