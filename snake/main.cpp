#include <time.h>
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

bool choice = false;
enum moves {
    LEFT, RIGHT, UP, DOWN
};
const int SCALE = 25;

int N=30,M=20;
const int WIDTH = SCALE * N;
const int HEIGHT = SCALE * M;

int dir; //направление змейки
int num=4; //текущие кол-во квадратов змейки

struct
{ int x;
    int y;}  s[100];

class Fructs
{
public:
	int x,y;
//появление нового фрукта
    void New()
    {  x=rand() % N;
        y=rand() % M;   }
//вывод на экран
    void DrawApple()
    {   glColor3f(0.0,1.0,0.0); //задаем цвета
        glRectf(x*SCALE,y*SCALE,(x+1)*SCALE,(y+1)*SCALE); }
    
} m[1];

void DrawSnake()
{
    glColor3f(0.0,0.0,1.0); //задаем цвета
    for (int i=0;i<num;i++)
    {glRectf(s[i].x*SCALE, s[i].y*SCALE, (s[i].x+0.95)*SCALE, (s[i].y+0.95)*SCALE); }
}

void Tick() //изменение головного элемента
{
//перемещение тела
    for (int i=num;i>0;--i)
    {s[i].x=s[i-1].x;
        s[i].y=s[i-1].y;}

//перемещение головы
    if (choice)
    {
//ходит по 90 градусов
    int a=0;
    int b=0;
    for (int i=0;i<num;i++)
    {
        while (a==0 && b==0)
        {
            a=m[i].x-s[0].x;
            b=m[i].y-s[0].y;
            if(a>0) { s[0].x+=1;}
            else if (b>0 ){s[0].y+=1;}
            else if (a<0) {s[0].x-=1;}
            else if (b<0) {s[0].y-=1;}
       }
    }
    }
    /*for (int i=0;i<1;i++)
    {do {
        b=m[i].y-s[0].y;
        if(b>0) s[0].y+=1;
        else s[0].y-=1;
    }while (b==0);
    }


    for (int i=0;i<1;i++)
    {while (a==0){
        a=m[i].x-s[0].x;
        if(a>0) s[0].x+=1;
        else s[0].x-=1;
    }
    }
*/
    else{
    if (dir==UP) s[0].y+=1;
    if (dir==LEFT) s[0].x-=1;
    if (dir==RIGHT) s[0].x+=1;
    if (dir==DOWN) s[0].y-=1;
    }
//проверка наложения координат яблока и головы
    for (int i=0;i<1;i++)
        if ((s[0].x==m[i].x) && (s[0].y==m[i].y))
        {
            num++;//увеличение хвоста
            m[i].New(); //создание нового яблока
        }
    
    if (s[0].x>N) dir=LEFT;  if (s[0].x<0) dir=RIGHT;
    if (s[0].y>M) dir=DOWN;  if (s[0].y<0) dir=UP;
    
    for (int i=1;i<num;i++)
    if (s[0].x==s[i].x && s[0].y==s[i].y)  num=i;
}


void DrawField() //поле
{
    glColor3f(0.0,0.0,0.0);
    glBegin(GL_LINES); //указываем что будем рисовать
    for (int i=0; i<WIDTH; i+=SCALE)
    {
        glVertex2f(i,0);
        glVertex2f(i,HEIGHT);
    }
    for (int j=0; j<HEIGHT; j+=SCALE)
    {
        glVertex2f(0,j);
        glVertex2f(WIDTH,j);
    }
    glEnd();
}


void display() {
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    DrawField(); //вызов функции рисования поля
    
    DrawSnake();

    for (int i=0;i<1;i++)
        m[i].DrawApple();
    
    glFlush(); //вывод на экран
    //glutSwapBuffers();
}


void KeyboardEvent(int key, int a, int b) //управление змейкой
{
    switch(key)
    {
        case 27:
			exit(0); // выход из приложения по нажатию ESC
			break;
        case 101 : 	dir=UP; break;
        case 102 :  dir=RIGHT; break;
        case 100 :  dir=LEFT; break;
        case 103 :  dir=DOWN; break;
    }
}

void timer(int = 0)
{
    display();
    Tick();
    glutTimerFunc(100,timer,0);
}

int main(int argc, char **argv) {
    
	srand((unsigned)time(0));
	
	for (int i=0;i<1;i++) {
        m[i].New();
    //начальная позиция
        s[i].x=10;
        s[i].y=10;
    }
	
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize (WIDTH, HEIGHT);
    glutCreateWindow ("Snake");
    glClearColor(1.0,1.0,0.7,1.0);  //цвет фона
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,WIDTH,0,HEIGHT);
    
    glutDisplayFunc (display);
    glutSpecialFunc(KeyboardEvent);
    glutTimerFunc(10,timer,0); //обявление таймера
    
    glutMainLoop(); //основной цикл программы
}
