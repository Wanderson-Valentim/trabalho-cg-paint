#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINES 50
#define MAX_POINTS 50
#define MAX_POLYGONS 1000
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
    Point center;
}Line;

typedef struct{
    Point points[MAX_POINTS_POLYGON];
    int numPoints;
    Point center;
}Polygon;

/* ------------- Definindo variaveis globais -------------*/
int numClicksToDrawLine = 0;
Point clicksToDrawLine[2];

int menuOption = 0;
int menuClick = 0;

//PARA O CTRL
bool ctrlPressed = false;

int blockPolygonCreation = false;

float mouseStartPosition[] = {0, 0};

/* ------------- Definindo variaveis dos objetos -------------*/

//Point points[MAX_POINTS];
//Line lines[MAX_LINES];
//Polygon polygons[MAX_POLYGONS];

int numPoints,numLines,numPolygons = 0;

Point *points = NULL;
Line *lines = NULL;

//Polygon *polygons = NULL;
Polygon polygons[MAX_POLYGONS];

/* ------------- Definindo variaveis dos objetos selecionaveis -------------*/
int numSelectedPoints = 0;
int selectedPoints[MAX_POINTS];

int numSelectedLines = 0;
int selectedLines[MAX_LINES];

int numSelectedPolygons = 0;
int selectedPolygons[MAX_POLYGONS];

/* ------------- Definindo funções de adiciona e remove objetos -------------*/
int compareIntegers(const void *a, const void *b) {
    int x = *(const int*)a;
    int y = *(const int*)b;
    return y - x;
}

/* ------------- Funções de remover -------------*/
void removePoint(){
    qsort(selectedPoints, numSelectedPoints, sizeof(int), compareIntegers);
    if(numPoints>0){
      for (int i = 0; i < numSelectedPoints; i++) {
            int selectedIndex = selectedPoints[i];
            for (int j = selectedIndex; j < numPoints - 1; j++) {
                points[j] = points[j + 1];
                points[j] = points[j + 1];

            }
            numPoints--;
        }
    }
    Point *temp = realloc(points, (numPoints) * sizeof(Point));
    points=temp;
    numSelectedPoints = 0;
}

void removeLine(){
    qsort(selectedLines, numSelectedLines, sizeof(int), compareIntegers);
    if(numLines>0){
      for (int i = 0; i < numSelectedLines; i++) {
            int selectedIndex = selectedLines[i];
            for (int j = selectedIndex; j < numLines - 1; j++) {
                lines[j] = lines[j + 1];
                lines[j] = lines[j + 1];

            }
            numLines--;
        }
    }
    Line *temp = realloc(lines, (numLines) * sizeof(Line));
    lines=temp;
    numSelectedLines = 0;
}

void removePoygons(){
    qsort(selectedPolygons, numSelectedPolygons, sizeof(int), compareIntegers);
    if(numPolygons>0){
      for (int i = 0; i < numSelectedPolygons; i++) {
            int selectedIndex = selectedPolygons[i];
            for (int j = selectedIndex; j < numPolygons - 1; j++) {
                polygons[j] = polygons[j + 1];
                polygons[j] = polygons[j + 1];

            }
            numPolygons--;
        }

    }

    //Line *temp = realloc(lines, (numLines) * sizeof(Line));
    //lines=temp;

    numSelectedPolygons = 0;
}


/* ------------- Funções de adicionar -------------*/
void addPoint(float x, float y){

    if(points==NULL){
        //deve ser alocado
        points = calloc(1, sizeof(Point));
    }else{
        //deve ser realocado
        Point *temp = realloc(points, (numPoints+1) * sizeof(Point));
        points=temp;
    }

    Point point;
    point.x = x;
    point.y = y;
    points[numPoints] = point;
    numPoints++;

}

void addLine(Point start, Point end){
    if(lines==NULL){
        //deve ser alocado
        lines = calloc(1, sizeof(Line));
    }else{
        //deve ser realocado
        Line *temp = realloc(lines, (numLines+1) * sizeof(Line));
        if (temp == NULL) {
            printf("Erro: não foi possível alocar memória."); //n tem mais espaco na memoria
            exit(1);
        }else{
            lines=temp;
        }
    }

    Line line;
    line.start = start;
    line.end = end;
    Point center;
    line.center = center;
    lines[numLines] = line;
    numLines += 1;
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
            glVertex2f(points[i].x, HEIGHT - points[i].y);
        }
    glEnd();
}

void drawLines(){
    glColor3f(1.0, 1.0, 0.0);
    for(int i = 0; i < numLines; i++){
        glBegin(GL_LINES);
            glVertex2f(lines[i].start.x, HEIGHT - lines[i].start.y);
            glVertex2f(lines[i].end.x, HEIGHT - lines[i].end.y);
        glEnd();
    }
}

void drawPolygons(){
    glColor3f(0.0, 1.0, 0.0);
    for(int i = 0; i < numPolygons; i++){
        glBegin(GL_POLYGON);
            for(int j = 0; j < polygons[i].numPoints; j++){
                glVertex2f(polygons[i].points[j].x, HEIGHT - polygons[i].points[j].y);
            }
        glEnd();
    }
}

void drawTriangle(float mouseX, float mouseY){
    float tolerance = 20.0;
    Point p1, p2, p3;
    p1.x = mouseX;
    p1.y = mouseY - tolerance;
    p2.x = mouseX + tolerance;
    p2.y = mouseY + tolerance;
    p3.x = mouseX - tolerance;
    p3.y = mouseY + tolerance;

    addPolygon();
    addPointOnPolygon(p1);
    addPointOnPolygon(p2);
    addPointOnPolygon(p3);

    polygons[numPolygons-1].center.x = mouseX;
    polygons[numPolygons-1].center.y = mouseY;
}

void drawSquare(float mouseX, float mouseY){
    float tolerance = 20.0;
    Point p1, p2, p3, p4;
    p1.x = mouseX - tolerance;
    p1.y = mouseY + tolerance;
    p2.x = mouseX + tolerance;
    p2.y = mouseY + tolerance;
    p3.x = mouseX + tolerance;
    p3.y = mouseY - tolerance;
    p4.x = mouseX - tolerance;
    p4.y = mouseY - tolerance;

    addPolygon();
    addPointOnPolygon(p1);
    addPointOnPolygon(p2);
    addPointOnPolygon(p3);
    addPointOnPolygon(p4);

    polygons[numPolygons-1].center.x = mouseX;
    polygons[numPolygons-1].center.y = mouseY;
}

/* ------------- Funções de transformações em objetos -------------*/
void rotatePoint(float angle){
    float convertedAngle = (angle * 3.14159) / 180.0; // converte para radianos
    float xr, yr, xStart, yStart, rxStart, ryStart;
    xr = 0.0;
    yr = HEIGHT;

    for(int i = 0; i < numSelectedPoints; i++){
        xStart = points[selectedPoints[i]].x;
        yStart = points[selectedPoints[i]].y;

        // Calcula as coordenadas relativas ao centro da linha
        rxStart = xStart - xr;
        ryStart = yStart - yr;

        // Rotação é aplicada no ponto
        points[selectedPoints[i]].x = xr + rxStart*cos(convertedAngle) - ryStart*sin(convertedAngle);
        points[selectedPoints[i]].y = yr + rxStart*sin(convertedAngle) + ryStart*cos(convertedAngle);
    }
}

void rotateLine(float angle){
    float convertedAngle = (angle * 3.14159) / 180.0; // converte para radianos
    float xr, yr, xStart, yStart, xEnd, yEnd, rxStart, ryStart, rxEnd, ryEnd;

    for(int i = 0; i < numSelectedLines; i++){
        xStart = lines[selectedLines[i]].start.x;
        yStart = lines[selectedLines[i]].start.y;
        xEnd = lines[selectedLines[i]].end.x;
        yEnd = lines[selectedLines[i]].end.y;
        xr = lines[selectedLines[i]].center.x;
        yr = lines[selectedLines[i]].center.y;

        // Calcula as coordenadas relativas ao centro da linha
        rxStart = xStart - xr;
        ryStart = yStart - yr;
        rxEnd = xEnd - xr;
        ryEnd = yEnd - yr;

        // Rotação é aplicada em cada ponto da reta
        lines[selectedLines[i]].start.x = xr + rxStart*cos(convertedAngle) - ryStart*sin(convertedAngle);
        lines[selectedLines[i]].start.y = yr + rxStart*sin(convertedAngle) + ryStart*cos(convertedAngle);
        lines[selectedLines[i]].end.x = xr + rxEnd*cos(convertedAngle) - ryEnd*sin(convertedAngle);
        lines[selectedLines[i]].end.y = yr + rxEnd*sin(convertedAngle) + ryEnd*cos(convertedAngle);
    }
}

void translatePoint(float x, float y){
    for(int i = 0; i < numSelectedPoints; i++){
        points[selectedPoints[i]].x += x;
        points[selectedPoints[i]].y += y;
    }
}

void translateLine(float x, float y){
    for(int i = 0; i < numSelectedLines; i++){
        lines[selectedLines[i]].start.x += x;
        lines[selectedLines[i]].start.y += y;
        lines[selectedLines[i]].end.x += x;
        lines[selectedLines[i]].end.y += y;
        lines[selectedLines[i]].center.x = (lines[selectedLines[i]].start.x + lines[selectedLines[i]].end.x)/2;
        lines[selectedLines[i]].center.y = (lines[selectedLines[i]].start.y + lines[selectedLines[i]].end.y)/2;
    }
}

void scaleLine(float xScale, float yScale){
    float xf, yf, xStart, yStart, xEnd, yEnd, rxStart, ryStart, rxEnd, ryEnd;

    for(int i = 0; i < numSelectedLines; i++){
        xStart = lines[selectedLines[i]].start.x;
        yStart = lines[selectedLines[i]].start.y;
        xEnd = lines[selectedLines[i]].end.x;
        yEnd = lines[selectedLines[i]].end.y;
        xf = lines[selectedLines[i]].center.x;
        yf = lines[selectedLines[i]].center.y;

        // Escala é aplicada em cada ponto da reta
        lines[selectedLines[i]].start.x = xf + xScale * xStart - xf * xScale;
        lines[selectedLines[i]].start.y = yf + yScale * yStart - yf * yScale;
        lines[selectedLines[i]].end.x = xf + xScale * xEnd - xf * xScale;
        lines[selectedLines[i]].end.y = yf + yScale * yEnd - yf * yScale;
    }
}

/* ------------- Funções de auxiliares de eventos -------------*/
void callCtrlZ(){

  switch(menuOption){
  
             case 1:
                //memset(points, 0, sizeof(points));
                 if(numPoints!=0){
                    numPoints--;
                    points = realloc(points, (numPoints) * sizeof(Point));
                 }
                 break;
             case 2:
                 if(numLines!=0){
                    numLines--;
                    lines = realloc(lines, (numLines) * sizeof(Line));
                 }
                break;
             case 3:
                if(numPolygons!=0){
                    numPolygons--;
                }
                break;
         }

        glutPostRedisplay();
}

//d é 1 ou -1, e indica se a escala do objeto vai ser no sentido horario ou anti-horario
void rotateObjects(float d){

    rotatePoint(d * 1.5);
    rotateLine(d * 1.5);
    glutPostRedisplay();
    
}

//d é 1 ou -1, e indica se a escala do objeto vai ser positiva ou negativa
void scaleObjects(float d){

    scaleLine(d * 1.2, d * 1.2);
    glutPostRedisplay();
    
}

//remover pontos linhas e polygonos selecionados
void deleteObjects(){

    removePoint();
    removeLine();
    removePoygons();
    glutPostRedisplay();
    
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
            case 5:
                if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
                
                    drawTriangle(convertedX, convertedY);
                    glutPostRedisplay();
                    
                }
                break;
            case 6:
                if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
                
                    drawSquare(convertedX, convertedY);
                    glutPostRedisplay();
                    
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
    printf("%c",key);
    fflush(stdout);
    
    switch(key){
    
        case 26:
            //chama funcao para remover o ultimo criado
            callCtrlZ();
            break;
        case 'd':
            //chama funcao apra deltar
            deleteObjects(); // lines,points,polygons
            break;
    }
}

void specialEvents(unsigned char key, int x, int y){

    switch(key){
        case GLUT_KEY_LEFT:
            rotateObjects(-1.0);
            break;
        case GLUT_KEY_RIGHT:
            rotateObjects(1.0);
            break;
        case GLUT_KEY_UP:
            scaleObjects(1.0);
            break;
        case GLUT_KEY_DOWN:
            scaleObjects(-1.0);
            break;
    }

}

void createMenu() {

    //SUBMENU DO  MENU PRINCIPAL 
    int subMenuId = glutCreateMenu(menuEvents);
    glutAddMenuEntry("Triangulo", 5);
    glutAddMenuEntry("Quadrado", 6);

    //MENU PRINCIPAL
    int menuId = glutCreateMenu(menuEvents);
    glutAddMenuEntry("Ponto", 1);
    glutAddMenuEntry("Linha", 2);
    glutAddSubMenu("Poligono", subMenuId);
    glutAddMenuEntry("Selecionar objeto", 4);
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

    // desenha todos o spoligons do vetor polygons
    drawPolygons(); 
    // desenha todas as linhas do vetor lines
    drawLines();
    // desenha todos o pontos do vetor points
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
    glutSpecialFunc(specialEvents);

    init();
    glutDisplayFunc(display); // estabele a função "display como a função callback de exibição"
    glutMainLoop(); // mostre tudo e espere

    return 0;
}
