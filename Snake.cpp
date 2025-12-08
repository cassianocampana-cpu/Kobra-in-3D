#include <GL/freeglut.h>
#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

// --- Configurações ---
const float RAIO_ORBITA = 3.5f; // Distância do centro
const float VELOCIDADE_SNAKE = 3.0f; // Graus por frame

// --- Estado do Jogo ---
float anguloSnake = 0.0f;
float anguloComida = 180.0f; // Começa oposto
bool jogoIniciado = false; // Controla se sai da intro

// --- Texto Pisca-Pisca ---
bool mostrarTexto = true;
int contadorFrame = 0;

// Variáveis de Câmera (Giro suave automático)
float anguloCamera = 0.0f;

// --- Funções Auxiliares ---

// Normaliza o ângulo para ficar entre 0 e 360
float normalizarAngulo(float angulo) {
    while (angulo >= 360.0f) angulo -= 360.0f;
    while (angulo < 0.0f) angulo += 360.0f;
    return angulo;
}

// Gera nova posição para a comida (longe da cobra)
void spawnComida() {
    float deslocamento = (rand() % 180) + 90; // Entre 90 e 270 graus de distância
    anguloComida = normalizarAngulo(anguloSnake + deslocamento);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    // Transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    GLfloat luzPos[] = { 0.0f, 10.0f, 10.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, luzPos);
    
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f); // Fundo espacial
    srand(time(NULL));
}

// --- Desenho de Texto 2D ---
void desenharTexto(const char* texto, float x, float y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // Define resolução lógica 2D
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST); // Garante que o texto fique na frente de tudo
    
    glColor3f(1.0f, 1.0f, 0.0f); // Amarelo Arcade
    glRasterPos2f(x, y);
    for (const char* c = texto; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // Câmera Orbital Suave (Gira devagar em torno da cena)
    float camX = sin(anguloCamera) * 12.0f;
    float camZ = cos(anguloCamera) * 12.0f;
    
    gluLookAt(camX, 6.0, camZ,  // Posição
              0.0, 0.0, 0.0,    // Alvo
              0.0, 1.0, 0.0);   // Cima

    // 1. Cubo Central (Azul Transparente)
    glPushMatrix();
        glColor4f(0.0f, 0.5f, 1.0f, 0.4f); // Azul vidro
        glutSolidCube(3.0);
        glColor4f(1.0f, 1.0f, 1.0f, 0.3f); // Bordas brancas suaves
        glutWireCube(3.05);
    glPopMatrix();

    // 2. Snake (Cubo Branco Orbitando)
    glPushMatrix();
        // A matemática da órbita: Girar -> Afastar
        glRotatef(anguloSnake, 0.0f, 1.0f, 0.0f); 
        glTranslatef(RAIO_ORBITA, 0.0f, 0.0f);
        
        // Efeito visual: Snake gira sobre si mesma enquanto orbita
        glRotatef(anguloSnake * 2.0f, 1.0f, 0.0f, 0.0f); 

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Branco Sólido
        glutSolidCube(1.0);
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f); // Borda preta
        glutWireCube(1.01);
    glPopMatrix();

    // 3. Comida (Esfera Vermelha Orbitando)
    glPushMatrix();
        glRotatef(anguloComida, 0.0f, 1.0f, 0.0f);
        glTranslatef(RAIO_ORBITA, 0.0f, 0.0f);
        
        // Comida pulsa ou gira devagar
        glRotatef(contadorFrame * 5.0f, 0.0f, 1.0f, 0.0f);

        glColor4f(1.0f, 0.0f, 0.0f, 1.0f); // Vermelho
        glutSolidSphere(0.4, 20, 20); // Esfera lisa
    glPopMatrix();

    // 4. Interface (HUD)
    if (mostrarTexto) {
        desenharTexto("PRESS ENTER TO START", 310, 100);
    }
    
    desenharTexto("SNAKE 3D", 360, 500); // Título no topo

    glutSwapBuffers();
}

void timer(int value) {
    // Atualiza câmera
    anguloCamera += 0.005f; // Rotação bem lenta da câmera

    // Pisca texto
    contadorFrame++;
    if (contadorFrame % 40 == 0) mostrarTexto = !mostrarTexto;

    // --- Lógica de Perseguição ---
    // A Snake sempre tenta diminuir a diferença angular para a comida
    
    // Calcula a diferença considerando o ciclo 0-360
    float diff = anguloComida - anguloSnake;
    // Corrige para pegar o caminho mais curto
    if (diff > 180.0f) diff -= 360.0f;
    if (diff < -180.0f) diff += 360.0f;

    // Move
    if (abs(diff) < VELOCIDADE_SNAKE) {
        // Colisão! (Está perto o suficiente)
        anguloSnake = anguloComida; // Gruda para não tremer
        spawnComida(); // Nova posição
    } else {
        // Anda na direção correta
        if (diff > 0) anguloSnake += VELOCIDADE_SNAKE;
        else anguloSnake -= VELOCIDADE_SNAKE;
    }

    anguloSnake = normalizarAngulo(anguloSnake);

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // 60 FPS
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 13) { // ENTER
        jogoIniciado = true;
        std::cout << "Jogo Iniciado! (Aqui voce carregaria a fase)" << std::endl;
        // exit(0); // Pode fechar ou trocar de tela aqui
    }
    if (key == 27) exit(0); // ESC para sair
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w/h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Snake 3D - Abertura");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    
    glutMainLoop();
    return 0;
}