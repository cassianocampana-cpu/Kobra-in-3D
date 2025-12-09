#ifndef GAME_H
#define GAME_H

#include <GL/freeglut.h>
#include <vector>

// Estruturas
enum Estado { INTRO, JOGANDO, GAME_OVER };
struct ponto { int x, z; };

// Variáveis Globais Compartilhadas
extern Estado estadoAtual;
extern int score;

// Declaração das Funções (Promessas)
void InitJogo();
void UpdateJogo();
void desenharCenaJogo();
void resetarJogo();
void processarInputJogo(unsigned char key);
void processarInputSetas(int key);

#endif