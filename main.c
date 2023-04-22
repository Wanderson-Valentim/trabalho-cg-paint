#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define MAX_LINES 50
#define MAX_POINTS 50
#define MAX_POLYGONS 30
#define MAX_POINTS_POLYGON 10

int menuOption;

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
    if(numPoints == MAX_POINTS){
        // op. invalida fzr alguma coisa
    }
    else{
        numPoints += 1;
    }
}

void removePoint(float x, float y){
    if(numPoints == 0){
        // op. invalida fzr alguma coisa
    }
    else{
        numPoints -= 1;
    }
}

void addLine(){
    if(numLines == MAX_LINES){
        // op. invalida fzr alguma coisa
    }
    else{
        numLines += 1;
    }
}

void removeLine(){
    if(numLines == 0){
        // op. invalida fzr alguma coisa
    }
    else{
        numLines -= 1;
    }
}

void addPolygon(){
    if(numPolygons == MAX_POLYGONS){
        // op. invalida fzr alguma coisa
    }
    else{
        numPolygons += 1;
    }
}

void removePolygon(){
    if(numPolygons == 0){
        // op. invalida fzr alguma coisa
    }
    else{
        numPolygons -= 1;
    }
}

/* ------------- Funções de matrizes -------------*/
void multiplyMatrix(){
}

void sumMatrix(){
}

/* ------------- Funções de desenho de objetos -------------*/
void drawPoint() {

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

/* ------------- Funções de eventos -------------*/
void menuEvents(int option) {
    menuOption = option;
    switch(option) {
        case 1:
            //criar ponto
            break;
        case 2:
            //criar linha
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
        case 6:
            //escala
            break;
        case 7:
            //selecao
            break;
    }
}

void mouseEvents(int button, int state, int x, int y){
    printf("%d %d\n", button, menuOption);
    fflush(stdout); // força a saída do buffer de stdout para poder usar o printf em tempo real
    glutPostRedisplay();
}

void createMenu() {
    int menuId = glutCreateMenu(menuEvents);

    glutAddMenuEntry("Ponto", 1);
    glutAddMenuEntry("Linha", 2);
    glutAddMenuEntry("Poligono", 3);
    glutAddMenuEntry("Rotacionar objeto", 4);
    glutAddMenuEntry("Transladar objeto", 5);
    glutAddMenuEntry("Escala de objeto", 6);
    glutAddMenuEntry("Seleção de objeto", 7);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Coloca as funções de inicialização
// Inicializa os parâmetros do rendering
int init(void){
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

int main(int argc, char** argv){
    glutInit(&argc,argv); //inicializa o GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // configura o modo de display
    glutInitWindowSize(500, 500); // configura a largura e altura da janela de exibição
    glutInitWindowPosition(200, 200); // posição da janela na tela
    glutCreateWindow("Paint bugado"); // cria a janela de exibição

    createMenu();
    glutMouseFunc(mouseEvents);

    init();
    glutDisplayFunc(display); // estabele a função "display como a função callback de exibição"
    glutMainLoop(); // mostre tudo e espere

    return 0;
}

