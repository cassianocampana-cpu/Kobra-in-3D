#include <GL/freeglut.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include<cmath>
#include "Game.cpp"


//CONFIGURAÇÕES
const int MAX_TAMANHO = 100; //tamanho máximo da cobrinha
const int GRID_SIZE = 20; //tamanho do tabuleiro 
const float CELL = 1.0;

Estado EstadoAtual = JOGO;
int score = 0;

std::vector<ponto> snake;
ponto direct = {1, 0}; 
ponto food;
bool samePoints(const ponto& a, const ponto& b) {
    return a.x == b.x && a.z == b.z;
}
void createFood(){
    bool val = false;
    while(!val){
        food.x = rand()%GRID_SIZE - GRID_SIZE/2;
        food.z = rand()%GRID_SIZE - GRID_SIZE/2;
        val = true;
        for (auto&p:snake){
            if (samePoints(p, food)){
                val = false;
                break;
            }
        }
    }
}
void resetarJogo(){
    snake.clear();
    snake.push_back({0,0});
    snake.push_back({-1,0});
    snake.push_back({-2,0});
    direct = {1,0};
    score = 0;
    createFood();
    EstadoAtual = JOGO;
}
void InitJogo() {
    srand(time(NULL));
    resetarJogo();
}
void UpdateJogo(){
    if (EstadoAtual==Intro||EstadoAtual==GAME_OVER){
        return;
    }
    ponto head = snake.front();
    head.x = head.x + direct.x;
    head.z = head.z + direct.z;
    if (std::abs(head.x) > GRID_SIZE/2 ||
        std::abs(head.z) > GRID_SIZE/2) {
        EstadoAtual = GAME_OVER;
        return;
    }
    for (int i = 0; i < (int)snake.size(); i++) {
        if (samePoints(head, snake[i])) {
            EstadoAtual = GAME_OVER;
            return;
        }
    }
    snake.insert(snake.begin(), head);
    if (samePoints(head, food)){
        score = score+1;
        createFood();
    } else {
            snake.pop_back();
        }
}
void desenharCenaJogo() {

}
void processarInputJogo(unsigned char key) {
    if (EstadoAtual == GAME_OVER && key == 'r') {
        resetarJogo();
        return;
    }
    if (key == 27) exit(0); 
}
void processarInputSetas(int key) {
    if (EstadoAtual==Intro||EstadoAtual==GAME_OVER) return;

    if (key == GLUT_KEY_UP && (direct.z != 1)) direct = {0, -1};
    if (key == GLUT_KEY_DOWN && (direct.z != -1)) direct = {0,  1};
    if (key == GLUT_KEY_LEFT && (direct.x != 1)) direct = {-1, 0};
    if (key == GLUT_KEY_RIGHT && (direct.x != -1)) direct = {1,  0};
}

void main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
}
