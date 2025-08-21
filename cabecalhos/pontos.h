#ifndef PONTOS_H
#define PONTOS_H

typedef struct {
    float x;
    float y;
} Ponto;

typedef struct PontoEl *Pontos;

// Inicialização e manipulação
Pontos *inicializar_pontos();
int add_ponto(Ponto ponto, Pontos *pontos);
Pontos get_ponto_selecionado();

// Utilidades geométricas
int avaliador_de_linha(Pontos ponto_inicial, float xmin, float xmax, float ymin, float ymax);
int avaliar_linha(Ponto p1, Ponto p2, float xmin, float xmax, float ymin, float ymax);
int contar_arestas_atingidas(float mouseX, float mouseY, Pontos *pontos);

// Renderização
int converter_vertices(Pontos *pontos);
int desenhar_pontos(Pontos *pontos);

// Transformações
Ponto calcular_centroide(int qtd, Pontos ponto_inicial);
int transformar_pontos(float **mat, int qtd, Pontos ponto_inicial);
Ponto vetor_ponto_centroide(Pontos ponto, float xm, float ym);

// Seleção e reset
void resetar_ponto_selecionado();
int selecionar_ponto(float mouseX, float mouseY, Pontos *pontos);

// Exclusão
int excluir_ponto_selecionado(Pontos *pontos);
int excluir_todos_pontos(Pontos *pontos);

#endif // PONTOS_H
