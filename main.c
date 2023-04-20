#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define MAX_LINES 50
#define MAX_POINTS 50
#define MAX_POLYGONS 30
#define MAX_POINTS_POLYGON 10

/* ------------- Definindo estrutura dos objetos -------------*/
typedef struct{
    float x;
    float y;
}Point;

typedef struct{
    Point start;
    Point end;
}Line;

typedef struct{
    Point points[MAX_POINTS_POLYGON];
}Polygon;

/* ------------- Definindo variaveis dos objetos -------------*/
int numLines = 0;
Line lines[MAX_LINES];

int numPoints = 0;
Point points[MAX_POINTS];

int numPolygons = 0;
Polygon polygons[MAX_POLYGONS];

/* ------------- Definindo funções de adiciona e remove objetos -------------*/
void addPoint(float x, float y){
}

void removePoint(float x, float y){
}

void addLine(){
}

void removeLine(){
}

void addPolygon(){
}

void removePolygon(){
}

/* ------------- Funções de desenho de objetos -------------*/
void drawCircle() {
}

void drawPolygon(){
}

void drawLine(){
}

void rotate(){
}

void translate(){
}

void scale(){
}

void menuEvents(int option) {
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

void createMenu() {
    int menu_id = glutCreateMenu(menuEvents);

    glutAddMenuEntry("Linha", 1);
    glutAddMenuEntry("Circulo", 2);
    glutAddMenuEntry("Poligono", 3);
    glutAddMenuEntry("Rotacionar objeto", 4);
    glutAddMenuEntry("Transladar objeto", 5);
    glutAddMenuEntry("Escala de objeto", 6);

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

    createMenu();

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

