#include <stdio.h>
#include <stdlib.h>
//#include <windows.h>
#include <stdbool.h>
#include <GL/glut.h>
#include <math.h>
#include "../../cabecalhos/estruturas/poligonos.h"
#include "../../cabecalhos/transformacoes/animar.h"
#include "../../cabecalhos/globais.h"

static Poligonos POLIGONO_SELECIONADO = NULL;
static PoligonoEl *POLIGONO_EM_CONSTRUCAO = NULL;
static Ponto PRIMEIRO_PONTO = {0,0};
static Ponto ULTIMO_PONTO = {0,0};
static int desenhando = 0;
static float TOLERANCIA = 10;
static int transladando = 0;
static int rotacionando = 0;
static int escalando = 0;

Poligonos *inicializar_poligonos() {
    Poligonos *poligonos = (Poligonos *) malloc(sizeof(Poligonos));
    *poligonos = NULL;
    return poligonos;
}

// Adiciona um pol�gono j� pronto � lista principal (fun��o interna)
int add_poligono(Poligono poligono, Poligonos *poligonos) {
    if (poligonos == NULL) return 0;
    PoligonoEl *novo = (PoligonoEl *) malloc(sizeof(PoligonoEl));
    if (novo == NULL) return 0;
    novo->poligono = poligono;
    novo->prox = *poligonos;
    *poligonos = novo;
    return 1;
}

Pontos get_poligono_pontos() {
    if (POLIGONO_SELECIONADO == NULL) return NULL;
    return *(POLIGONO_SELECIONADO->poligono.pontos);
}

int add_vertice_poligono_atual(float mouseX, float mouseY, Poligonos *poligonos) {
    Ponto p = {mouseX, mouseY};
    if (!desenhando) {
        POLIGONO_EM_CONSTRUCAO = (PoligonoEl *) malloc(sizeof(PoligonoEl));
        if (POLIGONO_EM_CONSTRUCAO == NULL) return 0;
        POLIGONO_EM_CONSTRUCAO->poligono.pontos = inicializar_pontos();
        POLIGONO_EM_CONSTRUCAO->prox = NULL;
        add_ponto(p, POLIGONO_EM_CONSTRUCAO->poligono.pontos);
        ULTIMO_PONTO = p;
        PRIMEIRO_PONTO = p;
        desenhando = 1;
        return 1;
    }
    if (PRIMEIRO_PONTO.x >= mouseX-TOLERANCIA && PRIMEIRO_PONTO.x <= mouseX+TOLERANCIA &&
        PRIMEIRO_PONTO.y >= mouseY-TOLERANCIA && PRIMEIRO_PONTO.y <= mouseY+TOLERANCIA) {
        finalizar_poligono_atual(poligonos);
        return 0;
    }
    add_ponto(p, POLIGONO_EM_CONSTRUCAO->poligono.pontos);
    ULTIMO_PONTO = p;
    return 1;
}

int finalizar_poligono_atual(Poligonos *poligonos) {
    if (!desenhando) return 0;
    POLIGONO_EM_CONSTRUCAO->prox = *poligonos;
    *poligonos = POLIGONO_EM_CONSTRUCAO;
    POLIGONO_EM_CONSTRUCAO = NULL;
    desenhando = 0;
    return 1;
}

int cancelar_poligono_atual() {
    if (!desenhando) return 0;
    excluir_todos_pontos(POLIGONO_EM_CONSTRUCAO->poligono.pontos);
    free(POLIGONO_EM_CONSTRUCAO);
    POLIGONO_EM_CONSTRUCAO = NULL;
    desenhando = 0;
    return 1;
}

int desenhar_previa_poligono(float mouseX, float mouseY) {
    if (!desenhando) return 0;

    glColor3f(0.5, 0.5, 0.5); // Cor cinza para a pr�via
    glBegin(GL_LINE_STRIP);
        converter_vertices(POLIGONO_EM_CONSTRUCAO->poligono.pontos); // Reusa a fun��o para desenhar os v�rtices
    glEnd();
    // Desenha a linha do �ltimo ponto at� o mouse
    glBegin(GL_LINES);
        glVertex2f(ULTIMO_PONTO.x, ULTIMO_PONTO.y);
        glVertex2f(mouseX, mouseY);
    glEnd();

    glutPostRedisplay();
    return 1;
}

// Desenha todos os pol�gonos finalizados
int desenhar_poligonos(Poligonos *poligonos) {
    if (poligonos == NULL) return 0;
    PoligonoEl *aux = *poligonos;
    while (aux != NULL) {
        if (aux == POLIGONO_SELECIONADO) {
            glColor3f(0, 1, 0); // Cor para pol�gono selecionado
        } else if (aux == poligono_animado) {
            glColor3fv(cor_animado); // Cor animada
        } else {
            glColor3f(0, 0, 0); // Cor padrão para obstáculos
        }

        // Para desenhar o pol�gono, iteramos por sua lista de pontos
        glBegin(GL_LINE_LOOP); // GL_LINE_LOOP conecta o �ltimo ponto ao primeiro
            converter_vertices(aux->poligono.pontos);
        glEnd();

        if (aux == poligono_animado) {
            Ponto centro = calcular_centroide(-1, *(aux->poligono.pontos));
            glColor3fv(cor_animado);
            glRasterPos2f(centro.x - 18, centro.y - 6); 
            const char* msg = "DVD";
            for (const char* c = msg; *c; c++)
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }

        aux = aux->prox;
    }
    return 1;
}

void resetar_poligono_selecionado() {
    POLIGONO_SELECIONADO = NULL;
}

int selecionar_poligono(float mouseX, float mouseY, Poligonos *poligonos) {
    if (poligonos == NULL) return 0;
    int count = 0;
    PoligonoEl *buscador = *poligonos;
    while (buscador != NULL) {
        count = contar_arestas_atingidas(mouseX, mouseY, buscador->poligono.pontos);
        if (count % 2 == 1) {
            POLIGONO_SELECIONADO = buscador;
            return 1;
        }
        buscador = buscador->prox;
    }
    POLIGONO_SELECIONADO = NULL;
    return 0;
}

int excluir_poligono_selecionado(Poligonos *poligonos) {
    if (poligonos == NULL || POLIGONO_SELECIONADO == NULL) return 0;

    // Libera a mem�ria da lista de pontos ANTES de liberar o pol�gono
    excluir_todos_pontos(POLIGONO_SELECIONADO->poligono.pontos);

    if (POLIGONO_SELECIONADO == *poligonos) {
        *poligonos = POLIGONO_SELECIONADO->prox;
        free(POLIGONO_SELECIONADO);
        POLIGONO_SELECIONADO = NULL;
        return 1;
    }

    PoligonoEl *buscador = (*poligonos)->prox;
    PoligonoEl *anterior = *poligonos;
    while (buscador != NULL && buscador != POLIGONO_SELECIONADO) {
        anterior = buscador;
        buscador = buscador->prox;
    }

    if (buscador == NULL) {
        POLIGONO_SELECIONADO = NULL;
        return 0;
    }

    anterior->prox = buscador->prox;
    free(buscador);
    POLIGONO_SELECIONADO = NULL;
    return 1;
}

int excluir_todos_poligonos(Poligonos *poligonos) {
    if (poligonos == NULL) return 0;
    PoligonoEl *aux = *poligonos;
    while (aux != NULL) {
        *poligonos = aux->prox;
        excluir_todos_pontos(aux->poligono.pontos);
        free(aux);
        aux = *poligonos;
    }
    free(*poligonos);
    return 1;
}

/* =================== Funções auxiliares =================== */

// Produto vetorial (p->q) x (p->r)
float cross(Ponto p, Ponto q, Ponto r) {
    return (q.x - p.x) * (r.y - p.y) - (q.y - p.y) * (r.x - p.x);
}

// Distância quadrada
double dist2(Ponto a, Ponto b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Comparador para ordenar por ângulo polar (usado no Graham Scan)
static Ponto p0; // ponto de referência
int cmpPolar(const void *a, const void *b) {
    Ponto *p1 = (Ponto *)a, *p2 = (Ponto *)b;
    float c = cross(p0, *p1, *p2);
    if (fabs(c) < 1e-9) return (dist2(p0, *p1) < dist2(p0, *p2)) ? -1 : 1;
    return (c > 0) ? -1 : 1;
}

/* =================== Graham Scan =================== */
int grahamScan(Ponto *pontos, int n, Ponto *hull) {
    if (n <= 1) {
        if (n == 1) hull[0] = pontos[0];
        return n;
    }

    // Encontrar ponto mais baixo (e mais à esquerda em caso de empate)
    int ymin = 0;
    for (int i = 1; i < n; i++) {
        if (pontos[i].y < pontos[ymin].y ||
           (pontos[i].y == pontos[ymin].y && pontos[i].x < pontos[ymin].x)) {
            ymin = i;
        }
    }
    // Trocar para posição 0
    Ponto temp = pontos[0];
    pontos[0] = pontos[ymin];
    pontos[ymin] = temp;
    p0 = pontos[0];

    // Ordenar por ângulo polar
    qsort(pontos + 1, n - 1, sizeof(Ponto), cmpPolar);

    // Construir fecho
    int m = 0;
    hull[m++] = pontos[0];
    hull[m++] = pontos[1];
    for (int i = 2; i < n; i++) {
        while (m > 1 && cross(hull[m-2], hull[m-1], pontos[i]) <= 0) {
            m--;
        }
        hull[m++] = pontos[i];
    }
    return m;
}

/* =================== Jarvis March adaptado =================== */
// Encontra o próximo ponto no fecho entre fechos parciais
Ponto nextHullPoint(Ponto **hulls, int *sizes, int k, Ponto current) {
    Ponto best;
    int found = 0;

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < sizes[i]; j++) {
            Ponto candidate = hulls[i][j];

            if (!found) {
                best = candidate;
                found = 1;
            } else {
                float c = cross(current, best, candidate);
                if (c < 0 || (fabs(c) < 1e-9 && dist2(current, candidate) > dist2(current, best))) {
                    best = candidate;
                }
            }
        }
    }
    return best;
}

/* =================== Chan’s Algorithm =================== */
int chanHull(Ponto *pontos, int n, Ponto *hull) {
    int m = 4; // tamanho inicial do bloco

    while (1) {
        int k = (n + m - 1) / m; // número de blocos
        Ponto **partialHulls = malloc(k * sizeof(Ponto *));
        int *sizes = malloc(k * sizeof(int));

        // Rodar Graham Scan em cada bloco
        for (int i = 0; i < k; i++) {
            int start = i * m;
            int end = (i+1) * m;
            if (end > n) end = n;
            int size = end - start;

            partialHulls[i] = malloc(size * sizeof(Ponto));
            sizes[i] = grahamScan(pontos + start, size, partialHulls[i]);
        }

        // Jarvis March sobre os fechos parciais
        int h = 0;
        // Encontrar ponto inicial global (menor y, e menor x em caso de empate)
        int idx = 0;
        for (int i = 1; i < n; i++) {
            if (pontos[i].y < pontos[idx].y ||
               (pontos[i].y == pontos[idx].y && pontos[i].x < pontos[idx].x)) {
                idx = i;
            }
        }
        hull[h++] = pontos[idx];

        for (int step = 0; step < m; step++) {
            Ponto best = nextHullPoint(partialHulls, sizes, k, hull[h-1]);

            // Fecho fechado (voltamos ao início)
            if (best.x == hull[0].x && best.y == hull[0].y) {
                for (int i = 0; i < k; i++) free(partialHulls[i]);
                free(partialHulls);
                free(sizes);
                return h;
            }

            hull[h++] = best;
        }

        // liberar memória
        for (int i = 0; i < k; i++) free(partialHulls[i]);
        free(partialHulls);
        free(sizes);

        // aumentar m e tentar de novo
        if (m >= n) break; // limite
        m *= m;
        if (m > n) m = n;
    }

    return 0; // fallback, não deveria chegar aqui
}


int transformarParaFechoConvexo(PontoEl *ponto_inicial) {
    if (ponto_inicial == NULL) return 0;

    // 1. Contar pontos
    int n = 0;
    PontoEl *aux = ponto_inicial;
    while (aux != NULL) {
        n++;
        aux = aux->prox;
    }

    // 2. Copiar para array
    Ponto *pontos = malloc(n * sizeof(Ponto));
    aux = ponto_inicial;
    for (int i = 0; i < n; i++) {
        pontos[i] = aux->ponto;
        aux = aux->prox;
    }

    // 3. Rodar ChanHull
    Ponto *hull = malloc(n * sizeof(Ponto));
    int h = chanHull(pontos, n, hull);

    // 4. Sobrescrever lista original com o fecho
    aux = ponto_inicial;
    PontoEl *ant = NULL;
    int i = 0;
    while (aux != NULL && i < h) {
        aux->ponto = hull[i];
        ant = aux;
        aux = aux->prox;
        i++;
    }

    // 5. Cortar a lista no ponto certo
    if (ant != NULL) {
        ant->prox = NULL;
    }

    // 6. Liberar nós extras
    while (aux != NULL) {
        ant = aux;
        aux = aux->prox;
        free(ant);
    }

    // 7. Liberar arrays auxiliares
    free(pontos);
    free(hull);

    return 1;
}


