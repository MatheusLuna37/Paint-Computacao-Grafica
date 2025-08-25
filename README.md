# 🎨 Paint

Este projeto é um **Paint** desenvolvido em linguagem **C** utilizando as
ferramentas **OpenGL** e **FreeGLUT**.\
O objetivo é fornecer uma aplicação gráfica simples para desenho de
pontos, linhas e polígonos, além de permitir transformações geométricas.
E, **utilizando essas estruturas**, criar uma pequena **animação** para consolidar
as habilidades adquiridas durante o processo.

## 🎯 Objetivo do Projeto

O projeto foi desenvolvido para composição da primeira nota da disciplina de
computação gráfica e foi implementado com o intuito de colocar em prática
conhecimentos teóricos adquiridos durante as aulas. Aprender o uso
da biblioteca **OpenGL** para criação de interfaces gráficas. Implementar 
estruturas de dados e **manipulação gráfica**. E explorar transformações como 
**translação**, **rotação** e **escala**.

## ⚙️ Funcionalidades

-   Desenho de **pontos**, **linhas** e **polígonos**.
-   Aplicação de transformações geométricas.
-   Estrutura modular para organização do código em múltiplos arquivos.
-   Uso da biblioteca **FreeGLUT** para gerenciamento de janelas e
    eventos.
-   Animação utilizando as estruturas criadas.

## 🛠️ Tecnologias Utilizadas

-   Linguagem C
-   OpenGL
-   FreeGLUT

## 📂 Estrutura do Projeto

    ├── src/
    │   ├── main.c
    │   ├── globais.c
    │   ├── init.c
    │   ├── estruturas/
    │   │   ├── pontos.c
    │   │   ├── linhas.c
    │   │   ├── poligonos.c
    │   │   └── matrizes.c
    │   └── transformacoes/
    │   │   ├── animar.c
    |   |   ├── motion.c
    │   │   └── transformar.c
    │   ├── eventos/
    │   │   ├── display.c
    |   |   ├── arquivos.c
    │   │   └── teclado.c
    ├── cabecalhos/

-   **main.c**: ponto de entrada do programa.
-   **estruturas/**: contém implementações de pontos, linhas, polígonos
    e matrizes.
-   **transformacoes/**: funções responsáveis por aplicar transformações
    geométricas.
-   **eventos**: funções responsáveis por capturar eventos de interatividade
    do usuário com o sistema através do teclado e mouse.
-   A estrutura é a mesma para os cabecalhos

## 🚀 Como Compilar e Executar

**Certifique-se de ter as bibliotecas OpenGL e FreeGLUT instaladas:**

**No Ubuntu/Debian, use:**
```sh
sudo apt-get install freeglut3-dev
```

1. **Compile o programa**  
No diretório raiz do projeto, execute:

```sh
gcc -o paint [main.c](http://_vscodecontentref_/0) [globais.c](http://_vscodecontentref_/1) [init.c](http://_vscodecontentref_/2) \
    [pontos.c](http://_vscodecontentref_/3) [linhas.c](http://_vscodecontentref_/4) [poligonos.c](http://_vscodecontentref_/5) [matrizes.c](http://_vscodecontentref_/6) \
    [animar.c](http://_vscodecontentref_/7) [transformar.c](http://_vscodecontentref_/8) [motion.c](http://_vscodecontentref_/9) \
    [display.c](http://_vscodecontentref_/10) [teclado.c](http://_vscodecontentref_/11) [arquivos.c](http://_vscodecontentref_/12) \
    -I./cabecalhos -lGL -lGLU -lglut -lm
```

2. Execute o programa:
```sh
./paint
```

**No Windows:**

1. **Instale o MinGW-w64 e as bibliotecas FreeGLUT para Windows.**

2. **Coloque os arquivos .h de FreeGLUT em include/, .a em lib/ e .dll em bin/ do MinGW.**

3. **Compile assim (ajuste os caminhos conforme sua instalação):**
```sh
gcc -o paint.exe [main.c](http://_vscodecontentref_/13) [globais.c](http://_vscodecontentref_/14) [init.c](http://_vscodecontentref_/15) ^
    [pontos.c](http://_vscodecontentref_/16) [linhas.c](http://_vscodecontentref_/17) [poligonos.c](http://_vscodecontentref_/18) [matrizes.c](http://_vscodecontentref_/19) ^
    [animar.c](http://_vscodecontentref_/20) [transformar.c](http://_vscodecontentref_/21) [motion.c](http://_vscodecontentref_/22) ^
    [display.c](http://_vscodecontentref_/23) [teclado.c](http://_vscodecontentref_/24) [arquivos.c](http://_vscodecontentref_/25) ^
    -I./cabecalhos -lfreeglut -lopengl32 -lglu32 -lm
```

4. Execute o programa:
```sh
./paint.exe
```

## 🕹️ Funcionamento

O programa permite desenhar **pontos**, **linhas** e **polígonos** utilizando o mouse, além de aplicar diversas transformações geométricas sobre esses objetos. Você pode selecionar, transformar, animar e salvar os objetos criados. Todas as interações são feitas por meio do teclado e do mouse, tornando o uso intuitivo e dinâmico. Os objetos podem ser salvos e carregados do arquivo `objetos.txt`, permitindo continuar o trabalho posteriormente.

### Principais comandos do teclado

- **a**: Iniciar animação do polígono selecionado
- **c**: Cisalhamento
- **d**: Modo desenho  
    - **1**: Ponto  
    - **2**: Linha  
    - **3**: Polígono
- **e**: Escala
- **i**: Aplicar transformação selecionada
- **k**: Carregar objetos de `objetos.txt`
- **p**: Resetar modo, objeto ou ação
- **r**: Rotação
- **s**: Selecionar objeto
- **t**: Translação
- **u**: Salvar todos os objetos em `objetos.txt`
- **x**: Excluir objeto selecionado
- **z**: Reiniciar animação

### Observações

- Pontos, linhas e polígonos são desenhados com o mouse.
- Transformações geométricas podem ser aplicadas nos objetos selecionados.
- O arquivo `objetos.txt` armazena todos os objetos desenhados para uso futuro.

## 👨‍💻 Autores

Projeto desenvolvido por **Irlan Barros, Valney Maia, Miguel Wagner e Matheus Luna** para a disciplina de **Computação Gráfica** no curso de **Ciência da Computação**.
