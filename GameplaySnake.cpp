#include "Game.h"
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>

// --- VARIÁVEIS GLOBAIS (DEFINIÇÃO) ---
const int GRID_SIZE = 20;

std::vector<ponto> cobra;
ponto direct = {1, 0}; 
ponto food;
bool inputBloqueado = false; // <--- VOLTOU: Previne morte instantânea por tecla rápida

// --- FUNÇÕES DE SUPORTE ---
bool samePoints(const ponto& a, const ponto& b) {
    return a.x == b.x && a.z == b.z;
}

// CORREÇÃO DO TRAVAMENTO AQUI
void createFood(){
    bool val = false;
    int tentativas = 0; // Contador de segurança

    while(!val){
        // Sorteia posição
        food.x = rand() % GRID_SIZE - GRID_SIZE/2;
        food.z = rand() % GRID_SIZE - GRID_SIZE/2;
        
        val = true;
        // Verifica colisão com a cobra
        for (auto& p : cobra){
            if (samePoints(p, food)){
                val = false; 
                break;
            }
        }

        // SEGURANÇA: Se tentou 50 vezes e não achou lugar, para o loop.
        // Isso evita que o jogo congele se a cobra for muito grande ou tiver azar.
        tentativas++;
        if (tentativas > 50) {
            val = true; // Força a saída do loop (pode nascer em cima, mas não trava o jogo)
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
    estadoAtual = JOGANDO;
    inputBloqueado = false;
}

void InitJogo() {
    srand(time(NULL));
    resetarJogo();
}

void UpdateJogo(){
    if (estadoAtual == INTRO || estadoAtual == GAME_OVER){
        return;
    }
    
    // 1. Move a Cabeça
    ponto head = cobra.front();
    head.x += direct.x;
    head.z += direct.z;

    // 2. Checa Colisões (Paredes)
    if (std::abs(head.x) > GRID_SIZE/2 || std::abs(head.z) > GRID_SIZE/2) {
        estadoAtual = GAME_OVER;
        return;
    }
    // 3. Checa Colisão (Corpo)
    for (int i = 0; i < (int)cobra.size(); i++) {
        if (samePoints(head, cobra[i])) {
            estadoAtual = GAME_OVER;
            return;
        }
    }
    
    // 4. Insere a Nova Cabeça
    cobra.insert(cobra.begin(), head);
    
    // 5. Checa Comida
    if (samePoints(head, food)){
        score = score + 1;
        createFood();
        // Não faz pop_back, então a cobra cresce
    } else {
        cobra.pop_back(); // Remove o rabo (movimento normal)
    }

    // Libera o teclado para o próximo movimento
    inputBloqueado = false; 
}

void desenharCenaJogo() {
    // Câmera de Jogo
    glLoadIdentity();

// Configura a câmera estática (Top-Down):
// (0.0, 25.0, 15.0) -> Olho elevado e levemente inclinado para trás
// (0.0, 0.0, 0.0)   -> Foco no centro do tabuleiro
// (0.0, 1.0, 0.0)   -> Vetor UP
    gluLookAt(0.0, 25.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Grid
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);
    for(int i = -GRID_SIZE/2; i <= GRID_SIZE/2; i++) {
        glVertex3f((float)i, -0.5, (float)-GRID_SIZE/2); glVertex3f((float)i, -0.5, (float)GRID_SIZE/2);
        glVertex3f((float)-GRID_SIZE/2, -0.5, (float)i); glVertex3f((float)GRID_SIZE/2, -0.5, (float)i);
    }
    glEnd();

    // Borda da Arena (opcional, para visual)
    glPushMatrix();
        glTranslatef(0, -1.0, 0);
        glScalef(GRID_SIZE+1, 1.0, GRID_SIZE+1);
        glColor4f(0.0f, 0.0f, 0.5f, 0.3f);
        glutSolidCube(1.0);
    glPopMatrix();
    
    // Cobra
    for (size_t i = 0; i < cobra.size(); i++) {
        glPushMatrix();
        glTranslatef((float)cobra[i].x, 0.0f, (float)cobra[i].z);
        if (i == 0) glColor3f(0.0f, 1.0f, 0.0f); // Cabeça Verde Claro
        else glColor3f(0.0f, 0.8f, 0.0f);        // Corpo Verde Escuro
        glutSolidCube(0.9f);
        // Contorno preto para ver os gomos
        glColor3f(0.0f, 0.0f, 0.0f);
        glutWireCube(0.91f);
        glPopMatrix();
    }
    
    // Comida
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
    // Lógica de input WASD com Bloqueio
    if (estadoAtual == JOGANDO && !inputBloqueado) {
        bool moveu = false;
        if ((key == 'w' || key == 'W') && direct.z != 1) { direct = {0, -1}; moveu = true; }
        else if ((key == 's' || key == 'S') && direct.z != -1) { direct = {0, 1}; moveu = true; }
        else if ((key == 'a' || key == 'A') && direct.x != 1) { direct = {-1, 0}; moveu = true; }
        else if ((key == 'd' || key == 'D') && direct.x != -1) { direct = {1, 0}; moveu = true; }
        
        if (moveu) inputBloqueado = true;
    }
}

void processarInputSetas(int key) {
    // Lógica de input Setas com Bloqueio
    if (estadoAtual == JOGANDO && !inputBloqueado) {
        bool moveu = false;
        if (key == GLUT_KEY_UP && direct.z != 1) { direct = {0, -1}; moveu = true; }
        else if (key == GLUT_KEY_DOWN && direct.z != -1) { direct = {0, 1}; moveu = true; }
        else if (key == GLUT_KEY_LEFT && direct.x != 1) { direct = {-1, 0}; moveu = true; }
        else if (key == GLUT_KEY_RIGHT && direct.x != -1) { direct = {1, 0}; moveu = true; }
        
        if (moveu) inputBloqueado = true;
    }
}