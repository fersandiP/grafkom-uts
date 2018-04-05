#include "function.h"

cell lookAt[6] = {
    {{0.0, 0.0, 2.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}},
    {{-2.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}},
    {{2.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}},
    {{0.0, 0.0, -2.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}},
    {{0.0, 2.0, 1.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}},
    {{0.0, -2.0, 1.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}};

int num = 0, state = 0, status = 0, speed = 5;
GLfloat theta;
GLboolean rotate = GL_TRUE;
GLuint mainWindow;
GLuint subWindow[6];
GLuint subWidth = 384, subHeight = 256;
GLMmodel *model = NULL;
void(*mouseFunction[6])(int, int, int, int) = {f1, f2, f3, f4, f5, f6};

void f1(int b, int s, int x, int y)
{
    if (b == GLUT_LEFT_BUTTON, s == GLUT_DOWN)
    {
        mainKeyboard('1', 0, 0);
    }
}
void f2(int b, int s, int x, int y)
{
    if (b == GLUT_LEFT_BUTTON, s == GLUT_DOWN)
    {
        mainKeyboard('2', 0, 0);
    }
}
void f3(int b, int s, int x, int y)
{
    if (b == GLUT_LEFT_BUTTON, s == GLUT_DOWN)
    {
        mainKeyboard('3', 0, 0);
    }
}
void f4(int b, int s, int x, int y)
{
    if (b == GLUT_LEFT_BUTTON, s == GLUT_DOWN)
    {
        mainKeyboard('4', 0, 0);
    }
}
void f5(int b, int s, int x, int y)
{
    if (b == GLUT_LEFT_BUTTON, s == GLUT_DOWN)
    {
        mainKeyboard('5', 0, 0);
    }
}
void f6(int b, int s, int x, int y)
{
    if (b == GLUT_LEFT_BUTTON, s == GLUT_DOWN)
    {
        mainKeyboard('6', 0, 0);
    }
}

void drawWindowMark()
{
    int x = (status % 3) * (subWidth + GAP);
    int y = (status >= 3) * (subHeight + GAP);
    drawRectangle(x, y, subWidth + 2 * GAP, subHeight + 2 * GAP);
}

void drawRectangle(int x, int y, int width, int height)
{
    int x1 = x;
    int y1 = y;
    int x2 = x + width;
    int y2 = y + height;
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glVertex2i(x1, y1);
    glVertex2i(x1, y2);
    glVertex2i(x2, y2);
    glVertex2i(x2, y1);
    glEnd();
}
void redisplayAll()
{
    for (int i = 0; i < 6; i++)
    {
        glutSetWindow(subWindow[i]);
        glutPostRedisplay();
        subReshape(subWidth, subHeight);
    }
    glutSetWindow(mainWindow);
    glutPostRedisplay();
}



void mainKeyboard(unsigned char key, int x, int y)
{

    if (key >= 'A' && key <= 'Z')
    {
        key = key - 'A' + 'a';
    }
    if (key >= '1' && key <= '6')
    {
        status = key - '1';
    }
    else
    {
        switch (key)
        {
        case 'a':
            lookAt[status].eye[0] -= 0.1;
            break;
        case 'd':
            lookAt[status].eye[0] += 0.1;
            break;
        case 'w':
            lookAt[status].eye[1] += 0.1;
            break;
        case 's':
            lookAt[status].eye[1] -= 0.1;
            break;
        case 'q':
            lookAt[status].eye[2] += 0.1;
            break;
        case 'e':
            lookAt[status].eye[2] -= 0.1;
            break;
        case 'r':
            rotate = !rotate;
            break;
        case '<':
            speed = (speed - 1) > 0 ? speed - 1 : 1;
            break;
        case '>':
            speed = (speed + 1) <= 10 ? speed + 1 : 10;
            break;
        case 'x':
            exit(0);
            break;
        }
    }
    redisplayAll();
}

void mainMenu(int value)
{
    mainKeyboard((char)value, 0, 0);
    redisplayAll();
}

void drawModel(void)
{
    if (!model)
    {
        model = glmReadOBJ("data/porsche.obj");
        if (!model)
            exit(0);
        glmUnitize(model);
        glmFacetNormals(model);
        glmVertexNormals(model, 90.0);
    }

    glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);
}

void mainDisplay()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawWindowMark();
    glutSwapBuffers();
}

void subDisplay()
{
    if (rotate)
    {
        state = (state + speed) % 5001;
        theta = 360.0 * state / 5000;
    }

    glClearColor(0.0, 0.8, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(theta, 0.0f, 1.0f, 0.0f);
    drawModel();
    glPopMatrix();
    glutSwapBuffers();
}

void subReshape(int width, int height)
{
    glClearColor(0.0, 0.8, 0.0, 0.0);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)width / height, 1.0, 256.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(lookAt[num].eye[0], lookAt[num].eye[1], lookAt[num].eye[2],
              lookAt[num].at[0], lookAt[num].at[1], lookAt[num].at[2],
              lookAt[num].up[0], lookAt[num].up[1], lookAt[num].up[2]);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    if (++num == 6)
    {
        num = 0;
    }
}

void mainReshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    subWidth = (width - GAP * 4) / 3.0;
    subHeight = (height - GAP * 3) / 2.0;

    for (int i = 0; i < 6; i++)
    {
        glutSetWindow(subWindow[i]);
        glutPositionWindow(GAP + (i % 3) * (subWidth + GAP), GAP + (i >= 3) * (subHeight + GAP));
        glutReshapeWindow(subWidth, subHeight);
    }
}