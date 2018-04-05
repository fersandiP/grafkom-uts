#include "function.h"


int main(int argc, char *argv[])
{
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(subWidth * 3 + GAP * 5, subHeight * 2 + GAP * 3);
    glutInitWindowPosition(50, 50);
    glutInit(&argc, argv);

    mainWindow = glutCreateWindow("CAR CCTV");
    glutDisplayFunc(mainDisplay);
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
        glutAddMenuEntry("Toggle Rotation (r)", 'r');
        glutAddMenuEntry("Increase Speed (>)", '>');
        glutAddMenuEntry("Decrease Speed (<)", '<');
        glutAddMenuEntry("Quit (x)", 'x');
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        glutMouseFunc(*mouseFunction[i]);
    }
    glutIdleFunc(redisplayAll);
    glutMainLoop();

    return 0;
}