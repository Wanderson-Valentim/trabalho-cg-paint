#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

// Coloca as funções de inicialização
// Inicializa os parâmetros do rendering
int init(void){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_POLYGON);
        glVertex2i(10, 140);
        glVertex2i(190, 140);
        glVertex2i(190, 10);
        glVertex2i(10, 10);
    glEnd();

    glFlush();
}

int main(int argc, char** argv){
    glutInit(&argc,argv); //inicializa o GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // configura o modo de display
    glutInitWindowSize(400, 300); // configura a largura e altura da janela de exibição
    glutInitWindowPosition(200, 200); // posição da janela na tela
    glutCreateWindow("ATIVIDADE 2 CG"); // cria a janela de exibição

    init();
    glutDisplayFunc(display); // estabele a função "display como a função callback de exibição"
    glutMainLoop(); // mostre tudo e espere

    return 0;
}
