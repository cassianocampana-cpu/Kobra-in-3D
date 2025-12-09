#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <string>
#include "Game.h" // <--- Importante: Conecta com o GameplaySnake.cpp

// --- VARIÁVEIS DE ESTADO DO SISTEMA ---
Estado estadoAtual = INTRO;
int score = 0;

// ==========================================
//      VARIÁVEIS DA INTRO (Seu código)
// ==========================================
const float RAIO_ORBITA = 3.5f;
const float VELOCIDADE_SNAKE = 3.0f;

float anguloSnake = 0.0f;
float anguloComida = 180.0f;
float anguloCamera = 0.0f;
bool mostrarTexto = true;
int contadorFrame = 0;

// --- Funções Auxiliares da Intro ---

float normalizarAngulo(float angulo) {
    while (angulo >= 360.0f) angulo -= 360.0f;
    while (angulo < 0.0f) angulo += 360.0f;
    return angulo;
}

void spawnComidaIntro() {
    float deslocamento = (rand() % 180) + 90;
    anguloComida = normalizarAngulo(anguloSnake + deslocamento);
}

// Desenha texto na tela (Serve para Intro e Jogo)
void desenharTexto(const char* texto, float x, float y) {
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    
    glColor3f(1.0f, 1.0f, 0.0f); // Amarelo
    glRasterPos2f(x, y);
    for (const char* c = texto; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);

    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
}

// ==========================================
//           LÓGICA DA INTRO
// ==========================================

void drawIntro() {
    // Câmera Orbital Suave
    float camX = sin(anguloCamera) * 12.0f;
    float camZ = cos(anguloCamera) * 12.0f;
    gluLookAt(camX, 6.0, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // 1. Cubo Central (Azul)
    glPushMatrix();
        glColor4f(0.0f, 0.5f, 1.0f, 0.4f);
        glutSolidCube(3.0);
        glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
        glutWireCube(3.05);
    glPopMatrix();

    // 2. Snake (Branco)
    glPushMatrix();
        glRotatef(anguloSnake, 0.0f, 1.0f, 0.0f);
        glTranslatef(RAIO_ORBITA, 0.0f, 0.0f);
        glRotatef(anguloSnake * 2.0f, 1.0f, 0.0f, 0.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glutSolidCube(1.0);
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glutWireCube(1.01);
    glPopMatrix();

    // 3. Comida (Vermelha)
    glPushMatrix();
        glRotatef(anguloComida, 0.0f, 1.0f, 0.0f);
        glTranslatef(RAIO_ORBITA, 0.0f, 0.0f);
        glRotatef(contadorFrame * 5.0f, 0.0f, 1.0f, 0.0f);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glutSolidSphere(0.4, 20, 20);
    glPopMatrix();

    // UI
    desenharTexto("SNAKE 3D", 360, 500);
    if (mostrarTexto) desenharTexto("PRESS ENTER TO START", 310, 100);
}

void updateIntro() {
    anguloCamera += 0.005f;
    contadorFrame++;
    if (contadorFrame % 40 == 0) mostrarTexto = !mostrarTexto;

    // Lógica de perseguição orbital
    float diff = anguloComida - anguloSnake;
    if (diff > 180.0f) diff -= 360.0f;
    if (diff < -180.0f) diff += 360.0f;

    if (abs(diff) < VELOCIDADE_SNAKE) {
        anguloSnake = anguloComida;
        spawnComidaIntro();
    } else {
        if (diff > 0) anguloSnake += VELOCIDADE_SNAKE;
        else anguloSnake -= VELOCIDADE_SNAKE;
    }
    anguloSnake = normalizarAngulo(anguloSnake);
}

// ==========================================
//          SISTEMA PRINCIPAL (GLUT)
// ==========================================

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // O "CÉREBRO" QUE ESCOLHE O QUE DESENHAR
    if (estadoAtual == INTRO) {
        drawIntro(); // Desenha a sua intro bonita
    } 
    else if (estadoAtual == JOGANDO) {
        desenharCenaJogo(); // <--- Chama o GameplaySnake.cpp
    }
    else if (estadoAtual == GAME_OVER) {
        desenharCenaJogo(); // Mostra o jogo parado
        desenharTexto("GAME OVER", 350, 300);
        desenharTexto("PRESS ENTER TO RESTART", 300, 250);
    }

    glutSwapBuffers();
}

void timer(int value) {
    if (estadoAtual == INTRO) {
        updateIntro();
        glutTimerFunc(16, timer, 0); // Intro roda a 60 FPS
    } 
    else if (estadoAtual == JOGANDO) {
        UpdateJogo(); // <--- Chama o GameplaySnake.cpp
        glutTimerFunc(100, timer, 0); // Jogo roda mais lento (estilo Snake)
    } 
    else {
        glutTimerFunc(100, timer, 0); // Game over
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC

    if (key == 13) { // ENTER
        if (estadoAtual == INTRO || estadoAtual == GAME_OVER) {
            InitJogo(); // <--- Reseta variáveis lá no GameplaySnake.cpp
            estadoAtual = JOGANDO;
        }
    }

    // Se estiver jogando, manda o input para o outro arquivo
    if (estadoAtual == JOGANDO) {
        processarInputJogo(key);
    }
}

void specialKeys(int key, int x, int y) {
    if (estadoAtual == JOGANDO) {
        processarInputSetas(key);
    }
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0f, (float)w/h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLfloat luzPos[] = { 0.0f, 10.0f, 10.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, luzPos);
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
    srand(time(NULL));
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Snake 3D - Final");
    
    init(); // Configura OpenGL
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);
    
    glutMainLoop();
    return 0;
}