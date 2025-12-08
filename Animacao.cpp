#include <GL/freeglut.h>
#include <vector>
#include <unistd.h>

// --- Estruturas ---
struct Ponto { float x, z; };

// --- Variáveis Globais ---
Ponto cuboPos = { -1.0f, 1.0f }; 
std::vector<Ponto> rastro;       

// Variáveis de Animação
float anguloRolagem = 0.0f;
bool estaRolando = false;
int direcaoAtual = 0;
int passoAtual = 0;

//Cada número significa uma direção onde o bloco vai 0: direita; 1: esquerda; 2: baixo; 3: cima
std::vector<int> roteiro = {
    2, 2, // Direita
    0, 0, 0, 0, 0, // Baixo
    3, 3, 3,// Esquerda
    1, 1,   // Cima
    2     // Direita
};

// Variáveis de Pivô
float pivotX, pivotY, pivotZ;
float eixoX, eixoY, eixoZ;

void init(void) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    // Transparência dos rastros que o bloco deixa para trás
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLfloat luzPos[] = { 5.0f, 10.0f, 10.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, luzPos);
    
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f); 
}

void desenharCubo(float alpha) {
    glColor4f(0.4f, 0.0f, 0.8f, alpha); 
    glutSolidCube(1.0);
    glColor4f(0.0f, 0.0f, 0.0f, alpha);
    glutWireCube(1.01);
}

//o pivo peduz para onde cada parte dos eixos precisam ir
void configurarPivo(int direcao) {
    pivotY = -0.5f; 
    switch(direcao) {
        case 0: // Direita
            pivotX = 0.5f;
            pivotZ = 0.0f;
            eixoX = 0.0f; 
            eixoY = 0.0f; 
            eixoZ = -1.0f; 
            break;
        case 1: // Esquerda
            pivotX = -0.5f; 
            pivotZ = 0.0f; 
            eixoX = 0.0f; 
            eixoY = 0.0f; 
            eixoZ = 1.0f; 
            break;
        case 2: // Baixo
            pivotX = 0.0f; 
            pivotZ = 0.5f; 
            eixoX = 1.0f; 
            eixoY = 0.0f; 
            eixoZ = 0.0f; break;
        case 3: // Cima
            pivotX = 0.0f; 
            pivotZ = -0.5f; 
            eixoX = -1.0f; 
            eixoY = 0.0f; 
            eixoZ = 0.0f; 
            break;
    }
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); // Muda para a lente da câmera
    glLoadIdentity();
    gluPerspective(45.0f, (float)w/h, 0.1f, 100.0f); // Configura o 3D
    glMatrixMode(GL_MODELVIEW); // Volta para mover os objetos
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    //posicionamento da câmera em relação ao "mundo"
    gluLookAt(6.0, 8.0, 6.0,  
              1.0, 0.0, 4.0,  
              0.0, 1.0, 0.0);

    // Desenha o Rastro
    for (Ponto p : rastro) {
        glPushMatrix();
        glTranslatef(p.x, 0.0f, p.z);
        desenharCubo(0.4f); 
        glPopMatrix();
    }

    // Desenha o Cubo Ativo
    glPushMatrix();
        glTranslatef(cuboPos.x, 0.0f, cuboPos.z);
        if (estaRolando) {
            glTranslatef(pivotX, pivotY, pivotZ);
            glRotatef(anguloRolagem, eixoX, eixoY, eixoZ);
            glTranslatef(-pivotX, -pivotY, -pivotZ);
        }
        desenharCubo(1.0f); 
    glPopMatrix();

    glutSwapBuffers();
}

void timer(int value) {
    if (estaRolando) {
        float velocidade = 5.0f + (passoAtual * 1.5f); 
        anguloRolagem += velocidade;

        if (anguloRolagem >= 90.0f) {
            anguloRolagem = 0.0f;
            estaRolando = false;
            rastro.push_back(cuboPos);

            if (direcaoAtual == 0) cuboPos.x += 1.0f;
            else if (direcaoAtual == 1) cuboPos.x -= 1.0f;
            else if (direcaoAtual == 2) cuboPos.z += 1.0f;
            else if (direcaoAtual == 3) cuboPos.z -= 1.0f;
            
            passoAtual++;
        }
    } else {
        if (passoAtual < roteiro.size()) {
            if (passoAtual == 0) usleep(500000); // 500ms no Linux (usleep usa microsegundos)
            
            direcaoAtual = roteiro[passoAtual];
            configurarPivo(direcaoAtual);
            estaRolando = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //função para fazer animação em 
    glutInitWindowSize(800, 600);
    glutCreateWindow("GameCube Intro Animation");
    
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(1000, timer, 0); //tempo de ativação da animação 
    
    glutMainLoop();
    return 0;
}