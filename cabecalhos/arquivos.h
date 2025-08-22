#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "pontos.h"
#include "linhas.h"
#include "poligonos.h"

// Salva todos os objetos geométricos em um arquivo texto
int salvar_arquivo(const char *filepath, Pontos *pontos, Linhas *linhas, Poligonos *poligonos);

// Carrega todos os objetos geométricos de um arquivo texto
int carregar_arquivo(const char *filepath, Pontos *pontos, Linhas *linhas, Poligonos *poligonos);

#endif // ARQUIVOS_H
