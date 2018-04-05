#include <stdlib.h>
#include "glm.h"

#define GAP 5

typedef struct _cell
{
    GLfloat eye[3];
    GLfloat at[3];
    GLfloat up[3];

} cell;

cell lookAt[6] = {
    {{0.0, 0.0, 2.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}},
    {{-2.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}},
    {{2.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}},
    {{0.0, 0.0, -2.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}},
    {{0.0, 2.0, 1.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}},
    {{0.0, -2.0, 1.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}};

int num = 0, state = 0, status = 0;
GLfloat theta;
GLboolean rotate = GL_TRUE;
GLuint mainWindow;
GLuint subWindow[6];
GLuint subWidth = 384, subHeight = 256;
GLMmodel *model = NULL;

void display();
void redisplayAll();
void subDisplay();
void mainReshape(int width, int height);
void subReshape(int width, int height);
void drawmodel();
void redisplayAll();
void mainMenu(int value);
void mainKeyboard(unsigned char key, int x, int);

void redisplayAll()
{
    for (int i = 0; i < 6; i++)
    {
        glutSetWindow(subWindow[i]);
        glutPostRedisplay();
        subReshape(subWidth, subHeight);
    }
}

void mainKeyboard(unsigned char key, int x, int y)
{
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
        }
    }
    redisplayAll();
}

void mainMenu(int value)
{
    switch (value)
    {
    case 'r':
        rotate = !rotate;
        break;
    }
    redisplayAll();
}

void drawmodel(void)
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

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

void subDisplay()
{
    if (rotate)
    {
        state = (state + 1) % 1001;
        theta = 360.0 * state / 1000;
    }

    glClearColor(0.0, 0.8, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(theta, 0.0f, 1.0f, 0.0f);
    drawmodel();
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

int main(int argc, char *argv[])
{
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(subWidth * 3 + GAP * 5, subHeight * 2 + GAP * 3);
    glutInitWindowPosition(50, 50);
    glutInit(&argc, argv);

    mainWindow = glutCreateWindow("CAR CCTV");
    glutDisplayFunc(display);
    glutReshapeFunc(mainReshape);
    glutKeyboardFunc(mainKeyboard);

    for (int i = 0; i < 6; i++)
    {
        subWindow[i] = glutCreateSubWindow(mainWindow,
                                           GAP + (i % 3) * (subWidth + GAP),
                                           GAP + (GAP + subHeight) * (i >= 3),
                                           subWidth, subHeight);
        glutDisplayFunc(subDisplay);
        glutReshapeFunc(subReshape);
        glutKeyboardFunc(mainKeyboard);
        glutCreateMenu(mainMenu);
        glutAddMenuEntry("Toggle Rotation", 'r');
        glutAttachMenu(GLUT_RIGHT_BUTTON);
    }
    glutIdleFunc(redisplayAll);
    glutMainLoop();

    return 0;
}