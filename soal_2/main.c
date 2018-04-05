#include <stdlib.h>
#include "glm.h"

#define GAP 4

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

int num = 0, state = 0, status = 0, speed = 5;
GLfloat theta;
GLboolean rotate = GL_TRUE;
GLuint mainWindow;
GLuint subWindow[6];
GLuint subWidth = 384, subHeight = 256;
GLMmodel *model = NULL;

void mainDisplay();
void redisplayAll();
void subDisplay();
void mainReshape(int width, int height);
void subReshape(int width, int height);
void drawmodel();
void redisplayAll();
void mainMenu(int value);
void mainKeyboard(unsigned char key, int x, int);
void drawRectangle(int x, int y, int width, int height);
void drawWindowMark();

/*
 Fungsi-fungsi berikut berguna untuk mouse function pada setiap sub-sub window
*/
void f1(int b, int s, int x, int y) {
	if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN) {
		mainKeyboard('1', 0, 0); //Trigger event keyboard angka 1
	}
}
void f2(int b, int s, int x, int y) {
	if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN) {
		mainKeyboard('2', 0, 0);//Trigger event keyboard angka 2
	}
}
void f3(int b, int s, int x, int y) {
	if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN) {
		mainKeyboard('3', 0, 0);//Trigger event keyboard angka 3
	}
}
void f4(int b, int s, int x, int y) {
	if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN) {
		mainKeyboard('4', 0, 0);//Trigger event keyboard angka 4
	}
}
void f5(int b, int s, int x, int y) {
	if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN) {
		mainKeyboard('5', 0, 0);//Trigger event keyboard angka 4
	}
}
void f6(int b, int s, int x, int y) {
	if (b == GLUT_LEFT_BUTTON && s == GLUT_DOWN) {
		mainKeyboard('6', 0, 0);//Trigger event keyboard angka 5
	}
}

void(*mouseFunction[6])(int, int, int, int) = { f1,f2,f3,f4,f5,f6 };

/*
Fungsi berikut berguna untuk mengambar border pada active window
*/
void drawWindowMark()
{
	// mengitung posisi x&y berdasarkan status
    int x = (status % 3) * (subWidth + GAP); 
    int y = (status >= 3) * (subHeight + GAP); 
    drawRectangle(x, y, subWidth + 2 * GAP, subHeight + 2 * GAP);
}


/*
Fungsi ini adalah fungsi untuk menggambar rectangle
*/
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

/*
Fungsi ini berguna untuk mereload display
*/
void redisplayAll()
{
    glutSetWindow(mainWindow);
    glutPostRedisplay();
    for (int i = 0; i < 6; i++)
    {
        glutSetWindow(subWindow[i]);
        glutPostRedisplay();
        subReshape(subWidth, subHeight);
    }
}

/*
Fungsi ini adalah callback function dari keyboard event
*/
void mainKeyboard(unsigned char key, int x, int y)
{

    if (key >= 'A' && key <= 'Z')
    {
        key = key - 'A' + 'a'; //convert huruf besar ke huruf kecil
    }
    if (key >= '1' && key <= '6')
    {
		//kalau keynya adalah key pergantian status, maka status diganti
        status = key - '1'; 
    }
    else
    {	//key a,s,d,q,e untuk mengubah eye
		//key <,> untuk mengubah speed rotasi
		//key r untuk toggle rotasi
		//key x untuk quit
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


/*
Fungsi ini adalah callback function untuk menu
*/
void mainMenu(int value)
{
	//value dari menu adalah character yg sudah didefined untuk keyboard,
	//jadi tinggal meneruskan saja valuenya ke fungsi keyboard event
    mainKeyboard((char)value, 0, 0);
    redisplayAll();
}

/*
Fungsi untuk untuk menggambar model object yang berotasi
*/
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

/*
Fungsi ini adalah fungsi display yang di bind ke main window
*/
void mainDisplay()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawWindowMark();
    glutSwapBuffers();
}

/*
Fungsi ini adalah fungsi display yang di bind ke sub window
*/
void subDisplay()
{	//kalau rotate aktif maka state akan diincrease sesuai speed
    if (rotate)
    {
        state = (state + speed) % 5001;
        theta = 360.0 * state / 5000;
    }

    glClearColor(0.0, 0.8, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glRotatef(theta, 0.0f, 1.0f, 0.0f);
    drawmodel();
    glPopMatrix();
    glutSwapBuffers();
}

/*
Fungsi ini adalah fungsi reshape yang di bind ke sub window
*/
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

/*
Fungsi ini adalah fungsi reshape yang di bind ke main window
*/
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
		//kalkulasi x,y posisi saat redisplay
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

    mainWindow = glutCreateWindow("UTS Grafkom");
    glutDisplayFunc(mainDisplay);
    glutReshapeFunc(mainReshape);
    glutKeyboardFunc(mainKeyboard);

    for (int i = 0; i < 6; i++)
    {
		//kalkulasi x,y posisi saat membuat subwindow
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
    glutIdleFunc(redisplayAll); //idle function selalu mereload display sehingga terbuat animasi
    glutMainLoop();

    return 0;
}
