#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <string>
#include <cstdio> // NECESSÁRIO PARA SPRINTF (TEXTO)
#include "Game.h"

// --- VARIÁVEIS GLOBAIS ---
Estado estadoAtual = INTRO;
int score = 0;

// --- VARIÁVEIS DA INTRO ---
const float RAIO_ORBITA = 3.5f;
const float VELOCIDADE_SNAKE = 3.0f;
float anguloSnake = 0.0f;
float anguloComida = 180.0f;
float anguloCamera = 0.0f;
bool mostrarTexto = true;
int contadorFrame = 0;

// --- FUNÇÕES DA INTRO ---
float normalizarAngulo(float angulo) {
    while (angulo >= 360.0f) angulo -= 360.0f;
    while (angulo < 0.0f) angulo += 360.0f;
    return angulo;
}

void spawnComidaIntro() {
    float deslocamento = (rand() % 180) + 90;
    anguloComida = normalizarAngulo(anguloSnake + deslocamento);
}

void desenharTexto(const char* texto, float x, float y) {
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    
    glColor3f(1.0f, 1.0f, 0.0f);
    glRasterPos2f(x, y);
    for (const char* c = texto; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);

    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
}

void drawIntro() {
    float camX = sin(anguloCamera) * 12.0f;
    float camZ = cos(anguloCamera) * 12.0f;
    
// Configura a câmera orbital:
// (camX, 6.0, camZ) -> Posição do Olho (Gira em círculos)
// (0.0, 0.0, 0.0)   -> Ponto de Foco (Olha para o centro do cubo azul)
// (0.0, 1.0, 0.0)   -> Vetor UP (Indica que o eixo Y é "para cima")
    gluLookAt(camX, 6.0, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Cubo Central
    glPushMatrix();
        glColor4f(0.0f, 0.5f, 1.0f, 0.4f);
        glutSolidCube(3.0);
        glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
        glutWireCube(3.05);
    glPopMatrix();

    // Snake e Comida Orbitais...
    glPushMatrix();
        glRotatef(anguloSnake, 0.0f, 1.0f, 0.0f);
        glTranslatef(RAIO_ORBITA, 0.0f, 0.0f);
        glRotatef(anguloSnake * 2.0f, 1.0f, 0.0f, 0.0f);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glutSolidCube(1.0);
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        glutWireCube(1.01);
    glPopMatrix();

    glPushMatrix();
        glRotatef(anguloComida, 0.0f, 1.0f, 0.0f);
        glTranslatef(RAIO_ORBITA, 0.0f, 0.0f);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glutSolidSphere(0.4, 20, 20);
    glPopMatrix();

    desenharTexto("SNAKE 3D REVOLUTION", 320, 500);
    if (mostrarTexto) desenharTexto("PRESS ENTER TO START", 310, 100);
}

void updateIntro() {
    anguloCamera += 0.005f;
    contadorFrame++;
    if (contadorFrame % 40 == 0) mostrarTexto = !mostrarTexto;
    
    float diff = anguloComida - anguloSnake;
    if (diff > 180.0f) diff -= 360.0f; if (diff < -180.0f) diff += 360.0f;

    if (abs(diff) < VELOCIDADE_SNAKE) {
        anguloSnake = anguloComida;
        spawnComidaIntro();
    } else {
        if (diff > 0) anguloSnake += VELOCIDADE_SNAKE; else anguloSnake -= VELOCIDADE_SNAKE;
    }
    anguloSnake = normalizarAngulo(anguloSnake);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (estadoAtual == INTRO) {
        drawIntro();
    } 
    else if (estadoAtual == JOGANDO) {
        desenharCenaJogo();
        // MOSTRA O SCORE
        char buffer[50];
        sprintf(buffer, "SCORE: %d", score);
        desenharTexto(buffer, 20, 570);
    } 
    else if (estadoAtual == GAME_OVER) {
        desenharCenaJogo();
        desenharTexto("GAME OVER", 350, 300);
        
        char buffer[50];
        sprintf(buffer, "FINAL SCORE: %d", score);
        desenharTexto(buffer, 330, 260);
        
        desenharTexto("PRESS ENTER TO RESTART", 300, 220);
    }
    glutSwapBuffers();
}

void timer(int value) {
    if (estadoAtual == INTRO) {
        updateIntro();
        glutTimerFunc(16, timer, 0); // Intro rápida (60FPS)
    } 
    else if (estadoAtual == JOGANDO) {
        UpdateJogo();
        glutTimerFunc(100, timer, 0); // Jogo na velocidade da cobra (10FPS)
    } 
    else {
        glutTimerFunc(100, timer, 0); // Game Over loop
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0);
    if (key == 13) { 
        if (estadoAtual == INTRO || estadoAtual == GAME_OVER) {
            InitJogo();
            estadoAtual = JOGANDO;
        }
    }
    if (estadoAtual == JOGANDO) processarInputJogo(key);
}

void specialKeys(int key, int x, int y) {
    if (estadoAtual == JOGANDO) processarInputSetas(key);
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
    glutCreateWindow("Snake 3D Final");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}