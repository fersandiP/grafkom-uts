#include "glm.h"

#define GAP 5

GLuint mainWindow;
GLuint subWindow[8];
GLuint subWidth = 256, subHeight = 256;

void display();
void subDisplay();
void mainReshape();

void display()
{
    glClearColor(0.8, 0.8, 0.8, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

void subDisplay()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers();
}

void mainReshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, height, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    subWidth = (width - GAP * 5) / 4.0;
    subHeight = (height - GAP * 3) / 2.0;

    for (int i = 0; i < 8; i++)
    {
        glutSetWindow(subWindow[i]);
        glutPositionWindow(GAP + (i % 4) * (subWidth + GAP), GAP + (i > 3) * (subHeight + GAP));
        glutReshapeWindow(subWidth, subHeight);
    }
}

int main(int argc, char *argv[])
{
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(1024 + GAP * 5, 512 + GAP * 3);
    glutInitWindowPosition(50, 50);
    glutInit(&argc, argv);

    mainWindow = glutCreateWindow("UTS GRAFKOM SOAL 2");
    glutDisplayFunc(display);
    glutReshapeFunc(mainReshape);


    for (int i = 0; i < 8; i++)
    {
        subWindow[i] = glutCreateSubWindow(mainWindow,
                                           GAP + (i % 4) * (subWidth + GAP),
                                           GAP + (GAP + subHeight) * (i > 3),
                                           subWidth, subHeight);
        glutDisplayFunc(subDisplay);
    }

    glutMainLoop();

    return 0;
}