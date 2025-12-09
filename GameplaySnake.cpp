#include "Game.h"
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

// --- VARIÁVEIS GLOBAIS (DEFINIÇÃO) ---
// Note que as variáveis score e EstadoAtual são definidas no Snake.cpp
const int GRID_SIZE = 20;

std::vector<ponto> cobra;
ponto direct = {1, 0}; // Direção de movimento inicial
ponto food;

// --- FUNÇÕES DE SUPORTE ---
bool samePoints(const ponto& a, const ponto& b) {
    return a.x == b.x && a.z == b.z;
}

void createFood(){
    bool val = false;
    while(!val){
        food.x = rand()%GRID_SIZE - GRID_SIZE/2;
        food.z = rand()%GRID_SIZE - GRID_SIZE/2;
        val = true;
        for (auto&p:cobra){
            if (samePoints(p, food)){
                val = false;
                break;
            }
        }
    }
}

// --- FUNÇÕES PÚBLICAS ---

void resetarJogo(){
    cobra.clear();
    cobra.push_back({0,0});
    cobra.push_back({-1,0});
    cobra.push_back({-2,0});
    direct = {1,0};
    score = 0;
    createFood();
    estadoAtual = JOGANDO; // Garante que o estado seja de JOGO
}

void InitJogo() {
    srand(time(NULL));
    resetarJogo();
}

void UpdateJogo(){
    if (estadoAtual==INTRO||estadoAtual==GAME_OVER){
        return;
    }
    
    // 1. Move a Cabeça
    ponto head = cobra.front();
    head.x += direct.x;
    head.z += direct.z;

    // 2. Checa Colisões
    if (std::abs(head.x) > GRID_SIZE/2 || std::abs(head.z) > GRID_SIZE/2) {
        estadoAtual = GAME_OVER;
        return;
    }
    for (int i = 0; i < (int)cobra.size(); i++) {
        if (samePoints(head, cobra[i])) {
            estadoAtual = GAME_OVER;
            return;
        }
    }
    
    // 3. Insere a Nova Cabeça
    cobra.insert(cobra.begin(), head);
    
    // 4. Checa Comida
    if (samePoints(head, food)){
        score = score+1;
        createFood();
    } else {
        cobra.pop_back(); // Remove o rabo
    }
}

void desenharCenaJogo() {
    // Implemente a renderização do Grid, Cobra e Comida aqui (Top-down)
    
    // Exemplo de Câmera de Jogo
    glLoadIdentity();
    gluLookAt(0.0, 25.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Desenha o Tabuleiro (Grid)
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);
    for(int i = -GRID_SIZE/2; i <= GRID_SIZE/2; i++) {
        glVertex3f((float)i, -0.5, (float)-GRID_SIZE/2); glVertex3f((float)i, -0.5, (float)GRID_SIZE/2);
        glVertex3f((float)-GRID_SIZE/2, -0.5, (float)i); glVertex3f((float)GRID_SIZE/2, -0.5, (float)i);
    }
    glEnd();
    
    // Desenha a Cobra
    for (size_t i = 0; i < cobra.size(); i++) {
        glPushMatrix();
        glTranslatef((float)cobra[i].x, 0.0f, (float)cobra[i].z);
        if (i == 0) glColor3f(0.0f, 1.0f, 0.0f); // Cabeça
        else glColor3f(0.0f, 0.8f, 0.0f); // Corpo
        glutSolidCube(0.9f);
        glPopMatrix();
    }
    
    // Desenha a Comida
    glPushMatrix();
    glTranslatef((float)food.x, 0.0f, (float)food.z);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere(0.4, 10, 10);
    glPopMatrix();
}

void processarInputJogo(unsigned char key) {
    if (estadoAtual == GAME_OVER && key == 'r') {
        resetarJogo();
        return;
    }
    // Lógica de input WASD
    if (estadoAtual == JOGANDO) {
        if (key == 'w' && (direct.z != 1)) direct = {0, -1};
        else if (key == 's' && (direct.z != -1)) direct = {0, 1};
        else if (key == 'a' && (direct.x != 1)) direct = {-1, 0};
        else if (key == 'd' && (direct.x != -1)) direct = {1, 0};
    }
}

void processarInputSetas(int key) {
    // Lógica de input Setas
    if (estadoAtual == JOGANDO) {
        if (key == GLUT_KEY_UP && (direct.z != 1)) direct = {0, -1};
        else if (key == GLUT_KEY_DOWN && (direct.z != -1)) direct = {0, 1};
        else if (key == GLUT_KEY_LEFT && (direct.x != 1)) direct = {-1, 0};
        else if (key == GLUT_KEY_RIGHT && (direct.x != -1)) direct = {1, 0};
    }
}