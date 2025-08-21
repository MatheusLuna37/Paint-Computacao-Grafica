#ifndef TRANSFORMAR_H_INCLUDED
#define TRANSFORMAR_H_INCLUDED

int selecionar_objeto(float mouseX, float mouseY, Pontos *pontos, Linhas *linhas, Poligonos *poligonos, Pontos *SELECIONADO);
int aplicar_transformacao(int action, int objeto, float mouseX, float mouseY, int dir, Pontos ponto_inicial);

#endif
