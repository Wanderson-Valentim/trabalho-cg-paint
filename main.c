#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define MAX_LINES 50
#define MAX_POINTS 50
#define MAX_POLYGONS 30
#define MAX_POINTS_POLYGON 10

#define TOLERANCE 10.0

#define WIDTH 960.0
#define HEIGHT 582.0

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
    int numPoints;
}Polygon;

/* ------------- Definindo variaveis globais -------------*/
int numClicksToDrawLine = 0;
Point clicksToDrawLine[2];

int menuOption = 1;
int menuClick = 0;

int blockPolygonCreation = 0;

float mouseStartPosition[] = {0, 0};

/* ------------- Definindo variaveis dos objetos -------------*/
int numPoints = 0;
Point points[MAX_POINTS];

int numLines = 0;
Line lines[MAX_LINES];

int numPolygons = 0;
Polygon polygons[MAX_POLYGONS];

/* ------------- Definindo variaveis dos objetos selecionaveis -------------*/
int numSelectedPoints = 0;
int selectedPoints[MAX_POINTS];

int numSelectedLines = 0;
int selectedLines[MAX_LINES];

int numSelectedPolygons = 0;
int selectedPolygons[MAX_POLYGONS];

/* ------------- Definindo funções de adiciona e remove objetos -------------*/
void addPoint(float x, float y){
    if(numPoints == MAX_POINTS){
        // op. invalida fzr alguma coisa
    }
    else{
        Point point;
        point.x = x;
        point.y = y;
        points[numPoints] = point;
        numPoints += 1;
    }
}

void removePoint(float x, float y){
    /*if(numPoints == 0){
        // op. invalida fzr alguma coisa
    }
    else{
        int indexElement = 0

        while(indexElement < numPoints || (x != points.x && y != points.y)){
            indexElement++;
        }

        if(indexElement < numPoints){
            for(int i = indexElement; i < numPoints-1; i++){
                points[indexElement] = points[indexElement+1];
            }

            numPoints -= 1;
        }
    }*/
}

void addLine(Point start, Point end){
    if(numLines == MAX_LINES){
        // op. invalida fzr alguma coisa
    }
    else{
        Line line;
        line.start = start;
        line.end = end;
        lines[numLines] = line;
        numLines += 1;
    }
}

void removeLine(Point start, Point end){
    /*if(numLines == 0){
        // op. invalida fzr alguma coisa
    }
    else{
        int indexElement = 0

        while(indexElement < numLines || ()){
            indexElement++;
        }

        if(indexElement < numLines){
            for(int i = indexElement; i < numLines-1; i++){
                lines[indexElement] = points[indexElement+1];
            }

            numLines -= 1;
        }
    }*/
}

void addPolygon(){
    printf("%d",numPolygons);
    fflush(stdout);

    if(numPolygons == MAX_POLYGONS){
        // op. invalida fzr alguma coisa
    }
    else{
        Polygon polygon;
        polygon.numPoints = 0;
        numPolygons += 1;
    }
}

void addPointOnPolygon(Point point){
    if(polygons[numPolygons-1].numPoints == MAX_POINTS_POLYGON){
        addPolygon();
    }
    int numPointsPolygon = polygons[numPolygons-1].numPoints;
    polygons[numPolygons-1].points[numPointsPolygon] = point;
    polygons[numPolygons-1].numPoints += 1;
}

void removePolygon(){
    if(numPolygons == 0){
        // op. invalida fzr alguma coisa
    }
    else{
        //numPolygons -= 1;
    }
}

/* ------------- Funções de matrizes -------------*/
/*void multiplyMatrix(int matrix1[][], int matrix2[][], int result[][]) {

}*/

void sumMatrix(){
}

/* ------------- Funções auxiliares -------------*/
int getCode(Point point, float mouseX, float mouseY){
    int code = 0;
    //Verifica se está a esquerda
    if(point.x < mouseX - TOLERANCE) code += 8;
    //Verifica se está a direita
    if(point.x > mouseX + TOLERANCE) code += 4;

    float my = HEIGHT - mouseY;
    float py = HEIGHT - point.y;

    //Verifica se está abaixo
    if(py < my - TOLERANCE) code += 2;
    //Verifica se está acima
    if(py > my + TOLERANCE) code += 1;

    return code;
}

/* ------------- Funções de seleção -------------*/


int pickPoint(float pointX, float pointY, float mouseX, float mouseY){
    if(mouseX <= pointX + TOLERANCE && mouseX >= pointX - TOLERANCE){
        float my = HEIGHT - mouseY;
        float py = HEIGHT - pointY;
        if(my <= py + TOLERANCE && my >= py - TOLERANCE) return 1;
    }
    return 0;
}

int pickLine(Point startPoint, Point endPoint, float mouseX, float mouseY){
    float xMax, xMin, yMax, yMin, xStart, yStart, xEnd, yEnd;
    int andOperation;
    int iteration = 0;

    Point newStartPoint;
    newStartPoint.x = startPoint.x;
    newStartPoint.y = startPoint.y;

    xMax = mouseX + TOLERANCE;
    xMin = mouseX - TOLERANCE;
    yMax = mouseY + TOLERANCE;
    yMin = mouseY - TOLERANCE;

    while(1){
        int startPointCode = getCode(newStartPoint, mouseX, mouseY);

        int endPointCode = getCode(endPoint, mouseX, mouseY);

        //Caso um dos dois seja 0 significa que pelo menos um dos pontos está na área do click
        if(startPointCode == 0 || endPointCode == 0) return 1;

        andOperation = startPointCode & endPointCode;

        //Se o AND for diferente de 0 significa que a reta está totalmente a esquerda, a direita, abaixo ou acima
        if(andOperation != 0) return 0;

        //Caso não trivial
        xStart = newStartPoint.x;
        yStart = newStartPoint.y;
        xEnd = endPoint.x;
        yEnd = endPoint.y;
        //Se o ponto inicial estiver a esquerda
        if((startPointCode & 8) == 8){
            newStartPoint.y = yStart + (xMin - xStart)*((yEnd - yStart)/(xEnd - xStart));
            newStartPoint.x = xMin;
        }
        //Se o ponto inicial estiver a direita
        else if((startPointCode & 4) == 4){
            newStartPoint.y = yStart + (xMax - xStart)*((yEnd - yStart)/(xEnd - xStart));
            newStartPoint.x = xMax;
        }
        //Se o ponto inicial estiver abaixo
        else if((startPointCode & 2) == 2){
            newStartPoint.x = xStart + (yMin - yStart)*((xEnd - xStart)/(yEnd - yStart));
            newStartPoint.y = yMin;
        }
        //Se o ponto inicial estiver acima
        else if((startPointCode & 1) == 1){
            newStartPoint.x = xStart + (yMax - yStart)*((xEnd - xStart)/(yEnd - yStart));
            newStartPoint.y = yMax;
        }
    }
}

/* ------------- Funções de desenho de objetos -------------*/
void drawPoints() {
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_POINTS);
        for(int i = 0; i < numPoints; i++){
            glVertex2f(points[i].x, 582 - points[i].y);
        }
    glEnd();
}

void drawLines(){
    glColor3f(1.0, 1.0, 0.0);
    for(int i = 0; i < numLines; i++){
        glBegin(GL_LINES);
            glVertex2f(lines[i].start.x, 582 - lines[i].start.y);
            glVertex2f(lines[i].end.x, 582 - lines[i].end.y);
        glEnd();
    }
}

void drawPolygons(){
    glColor3f(0.0, 1.0, 0.0);
    for(int i = 0; i < numPolygons; i++){
        glBegin(GL_POLYGON);
            for(int j = 0; j < polygons[i].numPoints; j++){
                glVertex2f(polygons[i].points[j].x, 582 - polygons[i].points[j].y);
            }
        glEnd();
    }
}

 void rotate(){ // teste para um angulo de 45 graus

     float angle = 45.0 * 3.14159 / 180.0; // converte para radianos
     float c;
     float s;
     //c  = cos(angle); // eu comentei pq n ta reconhecendo no meu pc essas funcao de cos e sin
     //s = sin(angle); //essas funcao do capeta não estão funcionando ne da biblioteca math.h?

     GLfloat matrix[] = {
         c, -s, 0, 0,
         s, c, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1
     };

    glPushMatrix();// coloca a matriz atual na pilha
    glMultMatrixf(matrix);//mutiplica a matriz de rotacao pela matriz atual
    glBegin(GL_LINES);
    //glVertex2f(lines[pos].start.x, 582 - lines[pos].start.y); // no caso o pos seria a posicao do vetor quando o algoritmo de selecao achar qual a linha que foi selecionada
    //glVertex2f(lines[pos].end.x, 582 - lines[pos].end.y);

    glEnd();
    glPopMatrix();
 }


void translatePoint(float x, float y){
    if(x == 0.0){
        for(int i = 0; i < numSelectedPoints; i++){
            points[selectedPoints[i]].y += y;
        }
    }
    else if(y == 0.0){
        for(int i = 0; i < numSelectedPoints; i++){
            points[selectedPoints[i]].x += x;
        }
    }
    else{
        for(int i = 0; i < numSelectedPoints; i++){
            points[selectedPoints[i]].x += x;
            points[selectedPoints[i]].y += y;
        }
    }
}

void translateLine(float x, float y){
    if(x == 0.0){
        for(int i = 0; i < numSelectedLines; i++){
            lines[selectedLines[i]].start.y += y;
            lines[selectedLines[i]].end.y += y;
        }
    }
    else if(y == 0.0){
        for(int i = 0; i < numSelectedLines; i++){
            lines[selectedLines[i]].start.x += x;
            lines[selectedLines[i]].end.x += x;
        }
    }
    else{
        for(int i = 0; i < numSelectedLines; i++){
            lines[selectedLines[i]].start.x += x;
            lines[selectedLines[i]].start.y += y;
            lines[selectedLines[i]].end.x += x;
            lines[selectedLines[i]].end.y += y;
        }
    }
}

void scale(){
}

/* ------------- Funções de eventos -------------*/
void menuEvents(int option) {
    menuOption = option;
    menuClick = 1;
}

void mouseEvents(int button, int state, int x, int y){
    float convertedX = x;
    float convertedY = y;

    if(menuClick != 1){
        switch(menuOption) {
            case 1:
                if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
                    addPoint(convertedX, convertedY);
                }
                break;
            case 2:
                if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
                    Point point;
                    point.x = convertedX;
                    point.y = convertedY;
                    clicksToDrawLine[numClicksToDrawLine] = point;
                    numClicksToDrawLine += 1;

                    if(numClicksToDrawLine == 2){
                        addLine(clicksToDrawLine[0], clicksToDrawLine[1]);
                        numClicksToDrawLine = 0;
                    }
                }
                break;
            case 3:
                if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
                    if(blockPolygonCreation == 0){
                        addPolygon();
                        blockPolygonCreation = 1;
                    }
                    Point point;
                    point.x = convertedX;
                    point.y = convertedY;
                    addPointOnPolygon(point);
                }
                if(button == GLUT_MIDDLE_BUTTON && state == GLUT_UP){
                    blockPolygonCreation = 0;
                }
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
                if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
                    int wasSelected = 0;
                    mouseStartPosition[0] = convertedX;
                    mouseStartPosition[1] = convertedY;
                    numSelectedPoints = 0;
                    numSelectedLines = 0;
                    numSelectedPolygons = 0;

                    //Verifica se algum ponto foi selecionado
                    for(int i = 0; i < numPoints; i++){
                        wasSelected = pickPoint(points[i].x, points[i].y, convertedX, convertedY);

                        if(wasSelected == 1){

                            selectedPoints[numSelectedPoints] = i;
                            numSelectedPoints += 1;
                        }
                    }


                    //Verifica se alguma reta foi selecionado
                    for(int i = 0; i < numLines; i++){
                        wasSelected = pickLine(lines[i].start, lines[i].end, convertedX, convertedY);

                        if(wasSelected == 1){
                            selectedLines[numSelectedLines] = i;
                            numSelectedLines += 1;
                        }
                    }

                     /*
                    //Verifica se algum poligono foi selecionado
                    for(int i = 0; i < MAX_POLYGONS; i++){
                        wasSelected = //função de selecionar poligono

                        if(wasSelected == 1){

                        }
                    }
                    */
                }
                if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
                    //printf("%f, %f\n",convertedX, convertedY);
                    translatePoint(convertedX - mouseStartPosition[0], convertedY - mouseStartPosition[1]);
                    translateLine(convertedX - mouseStartPosition[0], convertedY - mouseStartPosition[1]);
                    glutPostRedisplay();
                    //fflush(stdout);
                }
                break;
        }
    }
    /*printf("%d %d %d\n", button, menuOption, numPoints);
    fflush(stdout); // força a saída do buffer de stdout para poder usar o printf em tempo real*/
    menuClick = 0;
    glutPostRedisplay();
}

void keyboardEvents(unsigned char key, int x, int y){
    if(menuOption == 4){
        //rotaciona
    }
    else if(menuOption == 5){
        float translateX = 0.0;
        float translateY = 0.0;

        if(key == 'w' || key == 'W') translateY = -1.0;
        else if(key == 'd' || key == 'D') translateX = 1.0;
        else if(key == 'a' || key == 'A') translateX = -1.0;
        else if(key == 's' || key == 'S') translateY = 1.0;

        translatePoint(translateX, translateY);

        glutPostRedisplay();
    }
    else if(menuOption == 6){
        //escala
    }
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
    gluOrtho2D(0.0, WIDTH, 0.0, HEIGHT);
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawPolygons();
    drawLines();
    drawPoints();

    glutSwapBuffers();
}

int main(int argc, char** argv){
    glutInit(&argc,argv); //inicializa o GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // configura o modo de display
    glutInitWindowSize(960, 582); // configura a largura e altura da janela de exibição
    glutInitWindowPosition(200, 200); // posição da janela na tela
    glutCreateWindow("Paint bugado"); // cria a janela de exibição

    createMenu();
    glutKeyboardFunc(keyboardEvents);
    glutMouseFunc(mouseEvents);

    init();
    glutDisplayFunc(display); // estabele a função "display como a função callback de exibição"
    glutMainLoop(); // mostre tudo e espere

    return 0;
}

