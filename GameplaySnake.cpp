#include <GL/freeglut.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include<cmath>

//CONFIGURAÇÕES
const int MAX_TAMANHO = 100; //tamanho máximo da cobrinha
const int GRID_SIZE = 20; //tamanho do tabuleiro 

void main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
}