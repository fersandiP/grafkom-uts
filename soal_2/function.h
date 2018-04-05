#include <stdlib.h>
#include "glm.h"

#define GAP 4

typedef struct _cell
{
    GLfloat eye[3];
    GLfloat at[3];
    GLfloat up[3];

} cell;

void mainDisplay();
void redisplayAll();
void subDisplay();
void mainReshape(int width, int height);
void subReshape(int width, int height);
void drawModel();
void redisplayAll();
void mainMenu(int value);
void mainKeyboard(unsigned char key, int x, int);
void drawRectangle(int x, int y, int width, int height);
void drawWindowMark();
void f1(int b, int s, int x, int y);
void f2(int b, int s, int x, int y);
void f3(int b, int s, int x, int y);
void f4(int b, int s, int x, int y);
void f5(int b, int s, int x, int y);
void f6(int b, int s, int x, int y);

int num;
GLfloat theta;
GLboolean rotate;
GLuint mainWindow;
GLuint subWindow[6];
GLuint subWidth, subHeight;
GLMmodel *model;
void(*mouseFunction[6])(int, int, int, int);