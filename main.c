#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

// Funções a serem feitas
void drawCircle() {}

void drawPoligono(){}

void criaLinha(){}

void rotacionar(){}

void transladar(){}

void eventosDoMenu(int option) {
    switch(option) {
        case 1:
            //criar linha
            break;
        case 2:
            //circulo
            break;
        case 3:
            //poligono
            break;
        case 4:
            //rotacionar
            break;
        case 5:
            //transladar 
            break;
    }
}

void criarMenu() {
    int menu_id = glutCreateMenu(eventosDoMenu);
    glutAddMenuEntry("Linha", 1);
    glutAddMenuEntry("Circulo", 2);
    glutAddMenuEntry("Poligono", 3);
    glutAddMenuEntry("Rotacionar objeto", 4);
    glutAddMenuEntry("Transladar objeto", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Paint bugado");
    glClearColor(1.0, 1.0, 1.0, 0.0);
    criarMenu();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

