#include <stdlib.h>
#include "glm.h"

#define GAP 5

int num = 0;

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
    {{0.0, -2.0, 1.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}}
    };

GLuint mainWindow;
GLuint subWindow[6];
GLuint subWidth = 384, subHeight = 256;
GLMmodel *model = NULL;

void display();
void subDisplay();
void mainReshape();
void subReshape();
void drawmodel();

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
    glClearColor(0.8, 0.8, 0.8, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

void subDisplay()
{
    glClearColor(0.6, 0.6, 0.6, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawmodel();
    glutSwapBuffers();
}

void subReshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)width / height, 1.0, 256.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0.6, 0.6, 0.6, 0.0);
    gluLookAt(lookAt[num].eye[0], lookAt[num].eye[1], lookAt[num].eye[2],
              lookAt[num].at[0], lookAt[num].at[1], lookAt[num].at[2],
              lookAt[num].up[0], lookAt[num].up[1], lookAt[num].up[2]);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    if(++num==6){
        num=0;
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

    mainWindow = glutCreateWindow("UTS GRAFKOM SOAL 2");
    glutDisplayFunc(display);
    glutReshapeFunc(mainReshape);

    for (int i = 0; i < 6; i++)
    {
        subWindow[i] = glutCreateSubWindow(mainWindow,
                                           GAP + (i % 3) * (subWidth + GAP),
                                           GAP + (GAP + subHeight) * (i >= 3),
                                           subWidth, subHeight);
        glutDisplayFunc(subDisplay);
        glutReshapeFunc(subReshape);
    }

    glutMainLoop();

    return 0;
}