#ifndef GAME_H
#define GAME_H

#include<GL/freeglut.h>
#include<vector>

enum  Estado {Intro, JOGO, GAME_OVER};
struct ponto {int x, z;};

//VARIÁVEIS GLOBIAS QUE SE COMPARTILHAM ENTRE SI
//extern é uma variável que descreve que aquela função existe, porém, está em outro arquivo
extern Estado EstadoAtual;
extern int score;

// função da gameplay
void InitJogo();
void UpdateJogo();
void desenharCenaJogo();
void resetarJogo();
void processarInputJogo(unsigned char key);
void processarInputSetas(int key);

#endif