#include <gl/freeglut.h>
#include <cmath>
using namespace std;

//declare global variables
//anle of rotation for the camera direction
float angle = 0.0;
//actual vector representing the cameras direction
float lx = 0.0f, lz = -1.0f;
//xz position of the camera
float x = 0.0f, z = 5.0f;

//key states. values will be zero when no key is begin pressed
float deltaAngle = 0.0f;
float deltaMove = 0.0f;

float xOrigin = -1;

void mouseButton(int button, int state, int x, int y)
{
	//only start motion if the left button is pressed
	if (button ==GLUT_LEFT_BUTTON)
	{
		//when button released
		if (state == GLUT_UP)
		{
			angle += deltaAngle;
			xOrigin = -1;
		}
		else
		{
			xOrigin = x;
		}
	}
}

void mouseMove(int x, int y)
{
	if (xOrigin >=0)
	{
		//update deltaangle
		deltaAngle = (x - xOrigin)* 0.001f;

		//update cameras direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);

	}
}
void drawSnowMan()
{
	glColor3f(1.0f, 1.0f, 1.0f);

	//draw body
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	//draw head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);

	//draw eyes
	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	//draw nose
	glColor3f(1.0f, 0.5f, 0.5f);
	glutSolidCone(0.08f, 0.5f, 10, 2);
}
void computePos(float deltaMove)
{
	x += deltaMove * lx * 0.1f;
	z += deltaMove *lz * 0.1f;
}

void computeDir(float deltaAngle)
{
	angle += deltaAngle;
	lx = sin(angle);
	lz = -cos(angle);
}


void renderScene(void)
{
	if (deltaMove)
	{
		computePos(deltaMove);
	}
	if (deltaAngle)
	{
		computeDir(deltaAngle);
	}
	//clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//reset transformations
	glLoadIdentity();
	//set camera
	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);


	//draw ground
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f, 100.0f);
		glVertex3f(-100.0f, 0.0f, 100.0f);
		glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();

	//draw 36 snowmen
	for (int i = -3; i < 3; i++)
	{
		for (int j = -3; j < 3; j++)
		{
			glPushMatrix();
			glTranslatef(i*10.0, 0, j*10.0);
			drawSnowMan();
			glPopMatrix();
		}
	}
	glutSwapBuffers();
}

void pressKey(int key, int xx, int yy)
{
	
	switch (key)
	{
	case GLUT_KEY_LEFT:
		deltaAngle = -0.01f;
		break;
	case GLUT_KEY_RIGHT:
		deltaAngle = 0.01f;
		break;
	case GLUT_KEY_UP:
		deltaMove = 0.5f;
		break;
	case GLUT_KEY_DOWN:
		deltaMove = -0.5f;
		break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT: deltaAngle = 0.0f; break;
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN: deltaMove = 0; break;
	}
}

void changeSize(int w, int h)
{
	//prevent a divide by zero, when window is too short
	//you cant make a window of zero width
	if (h == 0)
	{
		h = 1;
	}
	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45, ratio, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
}
//function to execute when normal key is pressed
void processNormalKeys(unsigned char key, int x, int y)
{
	if (key == 27)
	{
		exit(0);
	}
}

int main(int argc, char **argv)
{
	// init GLUT and create window

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(250, 100);
	glutInitWindowSize(720, 720);
	glutCreateWindow("SNOWMAN GLOBE - GLUT Tutorial");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);

	glutSpecialUpFunc(releaseKey);

	//mousebutton callbacks
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing cycle
	glutMainLoop();
	return 0;
}