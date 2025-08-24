#include <stdio.h>
#include <stdlib.h>
#include "../../cabecalhos/eventos/arquivos.h"

int salvar_arquivo(const char *nome, Pontos *pontos, Linhas *linhas, Poligonos *poligonos) {
    FILE *f = fopen(nome, "w");
    if (!f) return 0;
    // Pontos
    int quantidadeDePontos = 0;
    PontoEl *p = *pontos;
    while (p != NULL) { quantidadeDePontos++; p = p->prox; }
    fprintf(f, "%d\n", quantidadeDePontos);
    p = *pontos;
    while (p != NULL) {
        fprintf(f, "%f %f\n", p->ponto.x, p->ponto.y);
        p = p->prox;
    }
    // Linhas
    int quantidadeDeLinhas = 0;
    LinhaEl *l = (linhas && *linhas) ? *linhas : NULL;
    while (l != NULL) { quantidadeDeLinhas++; l = l->prox; }
    fprintf(f, "%d\n", quantidadeDeLinhas);
    l = (linhas && *linhas) ? *linhas : NULL;
    while (l != NULL) {
        int pontosLinha = 0;
        PontoEl *pl = *(l->linha.pontos);
        while (pl != NULL) { pontosLinha++; pl = pl->prox; }
        fprintf(f, "%d\n", pontosLinha);
        pl = *(l->linha.pontos);
        while (pl != NULL) {
            fprintf(f, "%f %f\n", pl->ponto.x, pl->ponto.y);
            pl = pl->prox;
        }
        l = l->prox;
    }
    // Poligonos
    int quantidadeDePoligonos = 0;
    PoligonoEl *g = (poligonos && *poligonos) ? *poligonos : NULL;
    while (g != NULL) { quantidadeDePoligonos++; g = g->prox; }
    fprintf(f, "%d\n", quantidadeDePoligonos);
    g = (poligonos && *poligonos) ? *poligonos : NULL;
    while (g != NULL) {
        int pontosPoligono = 0;
        PontoEl *pg = *(g->poligono.pontos);
        while (pg != NULL) { pontosPoligono++; pg = pg->prox; }
        fprintf(f, "%d\n", pontosPoligono);
        pg = *(g->poligono.pontos);
        while (pg != NULL) {
            fprintf(f, "%f %f\n", pg->ponto.x, pg->ponto.y);
            pg = pg->prox;
        }
        g = g->prox;
    }
    fclose(f);
    return 1;
}

int carregar_arquivo(const char *nome, Pontos *pontos, Linhas *linhas, Poligonos *poligonos) {
    FILE *f = fopen(nome, "r");
    if (!f) return 0;
   
    int qtdP = 0;
    fscanf(f, "%d\n", &qtdP);
    for (int i = 0; i < qtdP; i++) {
        float x, y;
        fscanf(f, "%f %f\n", &x, &y);
        add_ponto((Ponto){x, y}, pontos);
    }
    int qtdL = 0;
    fscanf(f, "%d\n", &qtdL);
    for (int i = 0; i < qtdL; i++) {
        int n = 0;
        fscanf(f, "%d\n", &n);
        Pontos *lp = inicializar_pontos();
        for (int j = 0; j < n; j++) {
            float x, y;
            fscanf(f, "%f %f\n", &x, &y);
            add_ponto((Ponto){x, y}, lp);
        }
        add_linha((Linha){lp}, linhas);
    }
    int qtdG = 0;
    fscanf(f, "%d\n", &qtdG);
    for (int i = 0; i < qtdG; i++) {
        int n = 0;
        fscanf(f, "%d\n", &n);
        Pontos *gp = inicializar_pontos();
        for (int j = 0; j < n; j++) {
            float x, y;
            fscanf(f, "%f %f\n", &x, &y);
            add_ponto((Ponto){x, y}, gp);
        }
        add_poligono((Poligono){gp}, poligonos);
    }
    fclose(f);
    return 1;
}
