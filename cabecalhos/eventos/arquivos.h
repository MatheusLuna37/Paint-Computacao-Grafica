#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "../estruturas/pontos.h"
#include "../estruturas/linhas.h"
#include "../estruturas/poligonos.h"

// Salva os objetos geométricos em um txt
int salvar_arquivo(const char *filepath, Pontos *pontos, Linhas *linhas, Poligonos *poligonos);

// Carrega os objetos geométricos de um txt
int carregar_arquivo(const char *filepath, Pontos *pontos, Linhas *linhas, Poligonos *poligonos);

#endif // ARQUIVOS_H
