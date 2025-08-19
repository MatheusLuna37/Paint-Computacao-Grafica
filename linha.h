typedef struct LinhaEl *Linhas;
typedef struct Linha Linha;

typedef struct Linha {
    Ponto p1, p2;
} Linha;

Linhas *inicializar_linhas();
int add_linha(Linha linha, Linhas *linhas);
int excluir_todas_linhas(Linhas *linhas);
int desenhar_linhas(Linhas *linhas);
int excluir_linha_selecionada(Linhas *linhas);
