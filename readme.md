# Snake 3D - OpenGL Project

 Descrição do Projeto
Este projeto consiste no desenvolvimento de uma simulação tridimensional do clássico jogo "Snake" (popularmente conhecido como o "jogo da cobrinha" dos celulares Nokia tijolão). O objetivo principal do trabalho é demonstrar o domínio prático de conceitos de Computação Gráfica, utilizando a API OpenGL e a biblioteca GLUT para criar animações e renderização em tempo real.

Diferente da versão original em 2D, esta implementação traz uma perspectiva em 3D com câmeras posicionadas, iluminação (modelo de Phong/Gouraud), e transformações geométricas hierárquicas. O projeto conta ainda com uma cena de introdução animada (inspirada na identidade visual do GameCube) que transita para o estado de jogo, aplicando conceitos de máquinas de estado.

 Funcionalidades Implementadas
 Animação de Introdução: Cena orbital com transformações geométricas complexas (rotação e translação simultâneas).
 Jogabilidade Clássica: Controle da cobra, sistema de crescimento ao comer, colisão com paredes e com o próprio corpo.
 Renderização 3D: Uso de primitivas sólidas (glutSolidCube, glutSolidSphere) para construir o cenário e os personagens.
 Iluminação e Materiais: Configuração de luzes (GL_LIGHT0) e materiais coloridos para dar volume à cena.
 Câmera e Projeção: Uso de gluLookAt e gluPerspective para criar uma visão "Top-Down" (vista de cima) confortável para o jogador.
 Sistema de Pontuação: HUD simples desenhado em 2D sobre a cena 3D (glOrtho) para exibir o Score.

 Tecnologias Utilizadas
 Linguagem: C++
 API Gráfica: OpenGL
 Bibliotecas Auxiliares: FreeGLUT (ou GLUT)
 Ambiente de Desenvolvimento: VS Code no Linux (WSL/Ubuntu)

 Como Compilar e Executar

Dependências (Linux/WSL):
Certifique-se de ter as bibliotecas instaladas executando:
sudo apt-get update
sudo apt-get install build-essential freeglut3-dev

Compilação:
Como o projeto é modularizado, utilize o g++ linkando os arquivos principais e as bibliotecas do OpenGL:
g++ Snake.cpp GameplaySnake.cpp -o snake -lGL -lGLU -lglut

Execução:
./snake

 Mapa de Controles

[ENTER] - Inicia o jogo (na Intro) ou Reinicia (no Game Over)
[ESC]   - Fecha o jogo
[W] ou [Seta Cima]     - Move para Cima
[S] ou [Seta Baixo]    - Move para Baixo
[A] ou [Seta Esquerda] - Move para Esquerda
[D] ou [Seta Direita]  - Move para Direita

----------------------------------------------------------------
Autores:
 Vitor Luiz dos Santos
 Cassiano Sampaio Campana