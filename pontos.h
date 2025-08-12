typedef struct PontoEl *Pontos;
typedef struct Ponto Ponto;

typedef struct Ponto {
    float x, y;
} Ponto;

Pontos *inicializar_pontos();
int add_ponto(Ponto ponto, Pontos *pontos);
int excluir_todos_pontos(Pontos *pontos);
int desenhar_pontos(pontos);
Pontos selecionar_ponto_anterior(float mouseX, float mouseY, Pontos *pontos);
Pontos selecionar_ponto(float mouseX, float mouseY, Pontos *pontos);
